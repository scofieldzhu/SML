/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: glm_mesh_renderer.cpp 
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

#include "glm_mesh_renderer.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <spdlog/spdlog.h>
#include <glad/glad.h>
#include "glm_mesh.h"
#include "glm_buffer.h"
#include "glm_vertex_array.h"
#include "glm_vertex_array_attrib.h"
#include "glm_shader_program.h"
#include "glm_shader_source.h"
#include "glm_memory_block.h"

GLMESH_NAMESPACE_BEGIN

namespace{
  std::map<glmDisplayMode, GLuint> stDisplayModeDict = 
  {
    {glmDisplayMode::kPoint, GL_POINT},
    {glmDisplayMode::kWire, GL_LINE},
    {glmDisplayMode::kFacet, GL_FILL}
  };
} 

glmMeshRenderer::glmMeshRenderer()
{
}

glmMeshRenderer::~glmMeshRenderer()
{
    destroy();
}

void glmMeshRenderer::loadMeshCloud(glmMeshPtr mesh_cloud)
{
    cur_mesh_cloud_ = mesh_cloud;
    if(mesh_cloud->valid()){
        spdlog::warn("Mesh cloud contain a empty vertex list!");
        return;
    }
    auto boundingbox = cur_mesh_cloud_->calcBoundingBox();
    glm::vec3 center_point = boundingbox.calcCenter();
    float diagonal_len = boundingbox.calcDiagonalLength();
    spdlog::info("diagonal_len:{}", diagonal_len);
    //translate center of point cloud to origin point {0.0, 0.0, 0.0}
    model_ = glm::translate(glm::mat4(1.0), -center_point);
    program_->setUniformMatrix4fv("model", model_);
    eye_ = {0.0f, 0.0f, diagonal_len};
    view_  = glm::lookAt(eye_, focal_point_, viewup_);
    program_->setUniformMatrix4fv("view", view_);
    far_plane_dist_ = 1.6f * diagonal_len;
    projection_ = glm::perspective(glm::radians(fovy_), win_aspect_, near_plane_dist_, far_plane_dist_);
    program_->setUniformMatrix4fv("projection", projection_);

    buffer_ = std::make_shared<glmBuffer>(GL_ARRAY_BUFFER);
    const uint32_t kTotalSize = sizeof(glm::vec3) * static_cast<uint32_t>(mesh_cloud->vertex_pts.size());
    buffer_->allocate(kTotalSize, mesh_cloud->vertex_pts.data(), 0);

    vao_ = std::make_shared<glmVertexArray>();
    vao_->bindCurrent();
    vao_->bindBuffer(*buffer_);
    if(mesh_cloud->facets.valid()){
        indices_buffer_ = std::make_shared<glmBuffer>(GL_ELEMENT_ARRAY_BUFFER);
        auto indices_memory_block = mesh_cloud->facets.allocMemoryBlock();
        indices_buffer_->allocate(static_cast<uint32_t>(indices_memory_block->size()), indices_memory_block->blockData(), 0);
        vao_->bindBuffer(*indices_buffer_);
    }
    vao_->getAttrib(0)->setPointer(3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    vao_->getAttrib(0)->enable();
}

bool glmMeshRenderer::initialize(float width, float height)
{
    if(initialized_){
        spdlog::warn("Already initialized!");
        return true;
    }
    if(!gladLoadGL()){
        spdlog::error("gladLoadGL failed!");
        return false;
    }
    program_ = std::make_shared<glmShaderProgram>();
    if(!program_->addShaderSource(ShaderSource::kVertexShaderSource, GL_VERTEX_SHADER))
        return false;
    if(!program_->addShaderSource(ShaderSource::kFragmentShaderSource, GL_FRAGMENT_SHADER))
        return false;
    if(!program_->link())
        return false;
    program_->use();

    model_ = glm::identity<glm::mat4>();
    program_->setUniformMatrix4fv("model", model_);
    view_  = glm::lookAt(eye_, focal_point_, viewup_);
    program_->setUniformMatrix4fv("view", view_);
    win_aspect_ = width / height;
    projection_ = glm::perspective(glm::radians(fovy_), win_aspect_, near_plane_dist_, far_plane_dist_);
    program_->setUniformMatrix4fv("projection", projection_);
    initialized_ = true;
    return true;
}

void glmMeshRenderer::destroy()
{
    program_.reset();
    buffer_.reset();
    vao_.reset();
}

void glmMeshRenderer::setModelMat(const glm::mat4& mat)
{
    model_ = mat; 
    program_->setUniformMatrix4fv("model", model_);
}

void glmMeshRenderer::setCameraFovy(float fovy) 
{
    fovy_ = fovy;
    projection_ = glm::perspective(glm::radians(fovy_), win_aspect_, near_plane_dist_, far_plane_dist_);
    program_->setUniformMatrix4fv("projection", projection_);
}

void glmMeshRenderer::setDispalyMode(glmDisplayMode m)
{
    display_mode_ = m;
}

void glmMeshRenderer::render()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    if(vao_){
        vao_->bindCurrent();
        auto gl_mode = stDisplayModeDict[display_mode_];
        if(gl_mode == GL_POINT){
            glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(cur_mesh_cloud_->vertex_pts.size()));
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, gl_mode);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(cur_mesh_cloud_->facets.indicesCount()), GL_UNSIGNED_INT, 0);
        }
    }
    spdlog::info("Render called!");
}

void glmMeshRenderer::resize(float width, float height)
{
    render_size_ = {width, height};
    if(program_ == nullptr)
        return;
    glViewport(0, 0, (int)width, (int)height);
    win_aspect_ = width / height;
    projection_ = glm::perspective(glm::radians(fovy_), win_aspect_, near_plane_dist_, far_plane_dist_);
    program_->setUniformMatrix4fv("projection", projection_);
    render();
}

GLMESH_NAMESPACE_END