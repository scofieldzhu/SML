/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: glm_mesh_actor.cpp 
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

#include "glm_mesh_actor.h"
#include <spdlog/spdlog.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include "glm_shader_program.h"
#include "glm_misc.h"
#include "glm_buffer.h"
#include "glm_vertex_array.h"
#include "glm_vertex_array_attrib.h"
#include "glm_shader_source.h"
#include "glm_mesh.h"
#include "glm_mesh_renderer.h"
#include "glm_camera.h"

GLMESH_NAMESPACE_BEGIN

namespace{
  std::map<glmDisplayMode, GLuint> stDisplayModeDict = 
  {
    {glmDisplayMode::kPoint, GL_POINTS},
    {glmDisplayMode::kWire, GL_LINE},
    {glmDisplayMode::kFacet, GL_FILL}
  };

  constexpr size_t kMaxGLBufferSize = 50 * 1024 * 1024;
}

glmMeshActor::glmMeshActor(glmShaderProgramPtr prog)
    :program_(prog)
{
}

glmMeshActor::~glmMeshActor()
{
    destroy();
}

void glmMeshActor::setMeshCloud(glmMeshPtr mesh_cloud)
{
    if(mesh_cloud.get() == cur_mesh_cloud_.get()){
        spdlog::warn("The same mesh cloud has been loaded!");
        return;
    }
    cur_mesh_cloud_ = mesh_cloud;
    if(!mesh_cloud->valid()){
        spdlog::warn("Mesh cloud contain a empty vertex list!");
        return;
    }
}

void glmMeshActor::updateMeshCloud(glmMeshPtr mesh_cloud)
{

}

bool glmMeshActor::createSource(glmMeshRenderer* ren)
{
    if(!cur_mesh_cloud_->valid()){
        spdlog::error("Invalid mesh cloud data!!");
        return false;
    }
    auto mesh_byte_size = cur_mesh_cloud_->calcSize();
    if(mesh_byte_size > kMaxGLBufferSize){
        spdlog::error("Too large mesh byte size more than maximum value:{} of GL render!", kMaxGLBufferSize);
        return false;
    }
    auto boundingbox = cur_mesh_cloud_->calcBoundingBox();
    glm::vec3 center_point = cur_mesh_cloud_->calcCenterPoint();
    float diagonal_len = boundingbox.calcDiagonalLength();
    spdlog::info("diagonal_len:{} center_point:{}", diagonal_len, Vec3ToStr(center_point));
    matrix_ = glm::translate(glm::mat4(1.0), -center_point);
    auto ren_camera = ren->activeCamera();
    ren_camera->setModel(matrix_);
    ren_camera->setEye({0.0f, 0.0f, diagonal_len * 1.6f});
    ren_camera->setFarPlaneDist(ren_camera->eye()[2]);
    ren_camera->setWinAspect(ren->renderSize()[0] / ren->renderSize()[1]);

    buffer_ = nullptr; //release old buffer
    buffer_ = glmBuffer::New(GL_ARRAY_BUFFER);
    uint32_t vbs = cur_mesh_cloud_->calcByteSizeOfVertices();
    uint32_t cbs = cur_mesh_cloud_->calcByteSizeOfColors();
    uint32_t nbs = cur_mesh_cloud_->calcByteSizeOfNormals();
    uint32_t total_byte_size = vbs + cbs + nbs;
    buffer_->allocate(total_byte_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    buffer_->allocateSub(0, vbs, cur_mesh_cloud_->vertices.data());
    if(cbs)
        buffer_->allocateSub(vbs, cbs, cur_mesh_cloud_->colors.data());
    if(nbs)
        buffer_->allocateSub(vbs + cbs, nbs, cur_mesh_cloud_->normals.data());
    
    vao_ = nullptr; //release old vao
    vao_ = std::make_shared<glmVertexArray>();
    vao_->bindCurrent();
    vao_->bindBuffer(*buffer_);
    
    indices_buffer_ = nullptr; //release old indices buffer
    if(cur_mesh_cloud_->existFacetData()){        
        indices_buffer_ = glmBuffer::New(GL_ELEMENT_ARRAY_BUFFER);
        auto indices_data_mb = cur_mesh_cloud_->allocMemoryOfFacets();
        indices_buffer_->allocate(static_cast<uint32_t>(indices_data_mb->size()), indices_data_mb->blockData(), GL_DYNAMIC_STORAGE_BIT);
        vao_->bindBuffer(*indices_buffer_);
    }
    vao_->getAttrib(0)->setPointer(3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    vao_->getAttrib(0)->enable();
    if(!cur_mesh_cloud_->colors.empty()){              
        vao_->getAttrib(1)->setPointer(4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vbs));
        vao_->getAttrib(1)->enable();
    }
    if(!cur_mesh_cloud_->normals.empty()){       
        vao_->getAttrib(2)->setPointer(3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vbs + cbs));
        vao_->getAttrib(2)->enable();
    }    
    return true;
}

void glmMeshActor::setUserColor(const glm::vec4 &color)
{
    user_color_ = color;
}

void glmMeshActor::destroy()
{
    program_.reset();
    buffer_.reset();
    indices_buffer_.reset();
    vao_.reset();
}

void glmMeshActor::setDispalyMode(glmDisplayMode m)
{
    display_mode_ = m;
}

void glmMeshActor::draw(glmMeshRenderer* ren)
{
    if(vao_ == nullptr || renderers_.empty() || cur_mesh_cloud_ == nullptr || !cur_mesh_cloud_->valid()){
        return;
    }
    program_->use();
    vao_->bindCurrent();
    auto ren_camera = renderers_[0]->activeCamera();
    ren_camera->recalc();
    ren_camera->syncDataToShader(program_);
    program_->setUniformInt("primitive_type", 1);
    if(cur_mesh_cloud_->colors.empty()){
        program_->setUniformInt("use_vcolor", 0);
        program_->setUniformVec4("user_color", user_color_);
    }else{
        program_->setUniformInt("use_vcolor", 1);        
    }
    if(cur_mesh_cloud_->normals.empty()){
        program_->setUniformInt("use_vnormal", 0);
        program_->setUniformVec3("user_normal", user_normal_);
    }else{
        program_->setUniformInt("use_vnormal", 1);        
    } 
    auto gl_mode = stDisplayModeDict[display_mode_];
    glPolygonMode(GL_FRONT_AND_BACK, gl_mode);
    if(gl_mode == GL_POINTS){
        glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(cur_mesh_cloud_->vertices.size()));
    }else{            
        if(cur_mesh_cloud_->existFacetData()){
            if(cur_mesh_cloud_->isTriangulated()){
                glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(cur_mesh_cloud_->calcIndiceCount()), GL_UNSIGNED_INT, nullptr);
            }else{
                glEnable(GL_PRIMITIVE_RESTART);
                glPrimitiveRestartIndex(glmMesh::kPolyRestartIndex);
                glDrawElements(GL_TRIANGLE_STRIP, static_cast<GLsizei>(cur_mesh_cloud_->calcIndiceCount()), GL_UNSIGNED_INT, nullptr);
            }
        }
    }
}

GLMESH_NAMESPACE_END