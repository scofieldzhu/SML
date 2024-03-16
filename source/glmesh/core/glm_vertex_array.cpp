/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: glm_vertex_array.cpp 
 *  Copyright (c) 2024-2024 scofieldzhu
 *  
 *  MIT License
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 */

#include "glm_vertex_array.h"
#include "glad/glad.h"

GLMESH_NAMESPACE_BEGIN

glmVertexArray::glmVertexArray()
{
    glGenVertexArrays(1, &id_);
}

glmVertexArray::~glmVertexArray()
{
}

void glmVertexArray::bindCurrent()
{
    glBindVertexArray(id_);
}

void glmVertexArray::bindBuffer(uint32_t buffer_id)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer_id);
}

glmVertexArrayAttrib* glmVertexArray::getAttrib(uint32_t index)
{
    auto it = attrib_map_.find(index);
    if(it != attrib_map_.end()){
        return (*it).second.get();
    }
    auto attrib = std::make_unique<glmVertexArrayAttrib>(index);
    auto attrib_pointer = attrib.get();
    attrib_map_.insert({index, std::move(attrib)});
    return attrib_pointer;
}

GLMESH_NAMESPACE_END
