/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: glm_vertex_array.h 
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

#ifndef __glm_vertex_array_h__
#define __glm_vertex_array_h__

#include <map>
#include "glmesh/core/glm_vertex_array_attrib.h"
#include "glmesh/core/glm_export.h"

GLMESH_NAMESPACE_BEGIN

class GLMESH_API glmVertexArray 
{
public:
    void bindCurrent();
    void bindBuffer(const glmBuffer& buffer);
    void unbindBuffer(const glmBuffer& buffer);
    uint32_t id()const{ return id_; }
    glmVertexArrayAttrib* getAttrib(uint32_t index);
    glmVertexArray& operator=(const glmVertexArray&) = delete;
    glmVertexArray();
    glmVertexArray(const glmVertexArray&) = delete;
    ~glmVertexArray();

private:
    uint32_t id_ = 0;
    using AttribUPtr = std::unique_ptr<glmVertexArrayAttrib>;
    std::map<uint32_t, AttribUPtr> attrib_map_;
};

GLMESH_NAMESPACE_END

#endif