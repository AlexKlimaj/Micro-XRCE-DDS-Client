#include <micrortps/client/profile/transport/udp_transport.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <string.h>
#include <errno.h>

#define UDP_TRANSPORT_MTU 512

intmax_t send_data(UDPTransport* transport, const void* buf, size_t len);
intmax_t recv_data(UDPTransport* transport, void** buf, size_t* len, int timeout);

struct UDPProperties
{
    uint8_t buffer[UDP_TRANSPORT_MTU];
    int socket_fd;
    struct sockaddr remote_addr;
    struct pollfd poll_fd;
};

int init_udp_transport(UDPTransport* transport, const char* ip, uint16_t port)
{
    int result = 0;

    transport->send_data = send_data;
    transport->recv_data = recv_data;

    // Socket initialization.
    transport->properties->socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (-1 == transport->properties->socket_fd)
    {
        result = -errno;
    }

    if (0 < result)
    {
        // Remote IP setup.
        struct sockaddr_in temp_addr;
        temp_addr.sin_family = AF_INET;
        temp_addr.sin_port = port;
        temp_addr.sin_addr.s_addr = inet_addr(ip);
        memset(temp_addr.sin_zero, '\0', sizeof(temp_addr.sin_zero));
        transport->properties->remote_addr = *((struct sockaddr *) &temp_addr);

        // Poll setup.
        transport->properties->poll_fd.fd = transport->properties->socket_fd;
        transport->properties->poll_fd.events = POLLIN;

        // Remote address filter.
        int connected = connect(transport->properties->socket_fd,
                                &transport->properties->remote_addr,
                                sizeof(transport->properties->remote_addr));
        if (-1 == connected)
        {
            result = -errno;
        }
    }

    return result;
}

intmax_t send_data(UDPTransport* transport, const void* buf, size_t len)
{
    intmax_t result = 0;

    int sent = send(transport->properties->socket_fd, buf, len, 0);
    result = (sent <= 0) ? (intmax_t)sent : (intmax_t)-errno;

    return result;
}

intmax_t recv_data(UDPTransport* transport, void** buf, size_t* len, int timeout)
{
    intmax_t result = 0;

    int poll_rv = poll(&transport->properties->poll_fd, 1, timeout);
    if (0 < poll_rv)
    {
        int received = recv(transport->properties->socket_fd,
                            (void*)transport->properties->buffer,
                            sizeof(transport->properties->buffer), 0);
        if (0 <= received)
        {
            *len = (size_t)received;
            *buf = (void*)transport->properties->buffer;
        }
        else
        {
            *len = 0;
            *buf = NULL;
            result = (intmax_t)-errno;
        }
    }
    else if (0 == poll_rv)
    {
        *len = 0;
        *buf = NULL;
    }
    else
    {
        *len = 0;
        *buf = NULL;
        result = (intmax_t)-errno;
    }

    return result;
}