/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: glm_mesh.h 
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

#ifndef __glm_mesh_h__
#define __glm_mesh_h__

#include "glmesh/core/glm_memory_block.h"
#include "glmesh/core/glm_export.h"

GLMESH_NAMESPACE_BEGIN

struct GLMESH_API glmMesh
{
    using IndexType = uint32_t;
    static constexpr size_t kIndexTypeSize = sizeof(IndexType);
    static constexpr IndexType kPolyRestartIndex = 0xFFFFFFFF;    
    using VertexType = glm::vec3;
    using VertexListType = std::vector<VertexType>;
    using ColorType = glm::vec4;
    using ColorListType = std::vector<ColorType>;
    using TriangleFacetType = glm::u32vec3;
    using TriangleFacetList = std::vector<TriangleFacetType>;
    using PolyFacetType = std::vector<IndexType>;
    using PolyFacetListType = std::vector<PolyFacetType>;
    bool valid()const{ return vertices.empty(); }
    glmMemoryBlockPtr allocMemoryOfFacets();
    glmBoundingBox calcBoundingBox()const;
    glm::vec3 calcCenterPoint()const;
    uint32_t calcVertexBufferByteSize()const; 
    uint32_t calcColorBufferByteSize()const; 
    uint32_t calcIndiceCount()const;
    bool isTriangulated()const{ return !triangle_facets.empty(); }
    bool existFacetData()const{ return triangle_facets.size() || poly_facets.size(); }
    VertexListType vertices;
    ColorListType colors;
    TriangleFacetList triangle_facets;
    PolyFacetListType poly_facets;
};

GLMESH_NAMESPACE_END

#endif