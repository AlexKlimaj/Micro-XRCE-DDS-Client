// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*! 
 * @file Shape.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _Shape_H_
#define _Shape_H_

#include <string.h>
#include <stdlib.h>
#include <micrortps/client/client.h>
#include <microcdr/microcdr.h>

/*!
 * @brief This class represents the structure ShapeType defined by the user in the IDL file.
 * @ingroup SHAPE
 */
typedef struct ShapeType
{
    char* m_color;
    int32_t m_x;
    int32_t m_y;
    int32_t m_shapesize;
} ShapeType;

bool serialize_Shape_topic(MicroBuffer* writer, const AbstractTopic* topic_structure)
{
    ShapeType* topic = (ShapeType*) topic_structure->topic;
    serialize_uint32_t(writer, strlen(topic->m_color) + 1);
    serialize_array_char(writer, topic->m_color, strlen(topic->m_color) + 1);
    serialize_int32_t(writer, topic->m_x);
    serialize_int32_t(writer, topic->m_y);
    serialize_int32_t(writer, topic->m_shapesize);

    return true;
}

bool deserialize_Shape_topic(MicroBuffer* reader, AbstractTopic* topic_structure)
{
    ShapeType* topic = malloc(sizeof(ShapeType));
    uint32_t size = 0;
    deserialize_uint32_t(reader, &size);
    topic->m_color = malloc(size);
    deserialize_array_char(reader, topic->m_color, size);
    deserialize_int32_t(reader, &topic->m_x);
    deserialize_int32_t(reader, &topic->m_y);
    deserialize_int32_t(reader, &topic->m_shapesize);

    topic_structure->topic = topic;
    return true;
}

#endif // _Shape_H_