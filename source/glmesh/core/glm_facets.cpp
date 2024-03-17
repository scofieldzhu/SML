/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: glm_facets.cpp 
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

#include "glm_facets.h"
#include "glm_memory_block.h"

GLMESH_NAMESPACE_BEGIN

size_t glmFacets::calcByteSize() const
{
    return sizeof(uint32_t) * indicesCount();
}

size_t glmFacets::indicesCount() const
{
    size_t total_indices_count = 0;
    for(const auto& ft : data)
        total_indices_count += ft.indices.size();
    return total_indices_count;
}

glmMemoryBlockPtr glmFacets::allocMemoryBlock()const
{
    size_t total_size = calcByteSize();
    if(total_size == 0)
        return nullptr;
    auto data_block = std::make_shared<glmMemoryBlock>(total_size);
    auto cur_data = data_block->blockData();
    for(auto& ft : data){
        memcpy(cur_data, static_cast<const uint32_t*>(ft.indices.data()), ft.byteSize());
        cur_data += ft.byteSize();
    }
    return data_block;
}

GLMESH_NAMESPACE_END