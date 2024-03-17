/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: glm_base_type.h 
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

#ifndef __glm_base_type_h__
#define __glm_base_type_h__

#include <vector>
#include <memory>
#include <cstdint>
#include <numeric>
#include <glm/glm.hpp>
#include "glmesh/core/glm_nsp.h"

GLMESH_NAMESPACE_BEGIN

using VertexList = std::vector<glm::vec3>;

struct glmMesh;
using glmMeshPtr = std::shared_ptr<glmMesh>;

class glmBuffer;
using glmBufferPtr = std::shared_ptr<glmBuffer>;

class glmVertexArray;
using glmVertexArrayPtr = std::shared_ptr<glmVertexArray>;

class glmVertexArrayAttrib;
class glmShaderProgram;
using glmShaderProgramPtr = std::shared_ptr<glmShaderProgram>;

class glmMeshRenderer;
using glmMeshRendererPtr = std::shared_ptr<glmMeshRenderer>;

class glmWinEventHandler;

class glmTrackball;

struct glmBoundingBox
{
    glm::vec3 calcCenter()const{ return {(min[0] + max[0]) / 2.0, (min[1] + max[1]) / 2.0, (min[2] + max[2]) / 2.0}; }
    float calcDiagonalLength()const{ return glm::distance(min, max); }
    glm::vec3 min = glm::vec3(std::numeric_limits<float>::max());
    glm::vec3 max = glm::vec3(std::numeric_limits<float>::min());
};

GLMESH_NAMESPACE_END

#define BUFFER_OFFSET(a) ((void*)(a))

#endif