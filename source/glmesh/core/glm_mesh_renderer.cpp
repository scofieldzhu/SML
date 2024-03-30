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
    {glmDisplayMode::kPoint, GL_POINTS},
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

void glmMeshRenderer::setBackgroudBottomColor(const glm::vec3& color)
{
    bkg_bottom_color_ = color;
}

void glmMeshRenderer::setBackgroudTopColor(const glm::vec3& color)
{
    bkg_top_color_ = color;
}

void glmMeshRenderer::loadMeshCloud(glmMeshPtr mesh_cloud)
{
    if(mesh_cloud.get() == cur_mesh_cloud_.get()){
        spdlog::warn("The same mesh cloud has been loaded!");
        return;
    }
    cur_mesh_cloud_ = mesh_cloud;
    if(mesh_cloud->valid()){
        spdlog::warn("Mesh cloud contain a empty vertex list!");
        return;
    }
    program_->use();
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

    buffer_ = nullptr; //release old buffer
    buffer_ = glmBuffer::New(GL_ARRAY_BUFFER);
    uint32_t vbs = mesh_cloud->calcByteSizeOfVertices();
    uint32_t cbs = mesh_cloud->calcByteSizeOfColors();
    uint32_t nbs = mesh_cloud->calcByteSizeOfNormals();
    uint32_t total_byte_size = vbs + cbs + nbs;
    buffer_->allocate(total_byte_size, nullptr, GL_DYNAMIC_STORAGE_BIT);
    buffer_->allocateSub(0, vbs, mesh_cloud->vertices.data());
    if(cbs)
        buffer_->allocateSub(vbs, cbs, mesh_cloud->colors.data());
    if(nbs)
        buffer_->allocateSub(vbs + cbs, nbs, mesh_cloud->normals.data());
    
    vao_ = nullptr; //release old vao
    vao_ = std::make_shared<glmVertexArray>();
    vao_->bindCurrent();
    vao_->bindBuffer(*buffer_);
    
    indices_buffer_ = nullptr; //release old indices buffer
    if(mesh_cloud->existFacetData()){        
        indices_buffer_ = glmBuffer::New(GL_ELEMENT_ARRAY_BUFFER);
        auto indices_data_mb = mesh_cloud->allocMemoryOfFacets();
        indices_buffer_->allocate(static_cast<uint32_t>(indices_data_mb->size()), indices_data_mb->blockData(), GL_DYNAMIC_STORAGE_BIT);
        vao_->bindBuffer(*indices_buffer_);
    }
    vao_->getAttrib(0)->setPointer(3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    vao_->getAttrib(0)->enable();
    if(mesh_cloud->colors.empty()){
        program_->setUniformInt("use_vcolor", 0);
        program_->setUniformVec4("user_color", user_color_);
    }else{
        program_->setUniformInt("use_vcolor", 1);        
        vao_->getAttrib(1)->setPointer(4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vbs));
        vao_->getAttrib(1)->enable();
    }
    if(mesh_cloud->normals.empty()){
        program_->setUniformInt("use_vnormal", 0);
        program_->setUniformVec3("user_normal", user_normal_);
    }else{
        program_->setUniformInt("use_vnormal", 1);        
        vao_->getAttrib(2)->setPointer(3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vbs + cbs));
        vao_->getAttrib(2)->enable();
    }    
}

void glmMeshRenderer::setUserColor(const glm::vec4 &color)
{
    user_color_ = color;
    program_->use();
    program_->setUniformVec4("user_color", user_color_);
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

    bkg_program_ = glmShaderProgram::New();
    if(!bkg_program_->addShaderSource(ShaderSource::kBkgVertexShaderSource, GL_VERTEX_SHADER))
        return false;
    if(!bkg_program_->addShaderSource(ShaderSource::kBkgFragmentShaderSource, GL_FRAGMENT_SHADER))
        return false;
    if(!bkg_program_->link())
        return false;
    bkg_program_->use();
    float bkg_rt_vertices[] = 
    {
        -1.0f, -1.0f, 0.0f,  bkg_bottom_color_[0], bkg_bottom_color_[1], bkg_bottom_color_[2], 
        1.0f, -1.0f, 0.0f,  bkg_bottom_color_[0], bkg_bottom_color_[1], bkg_bottom_color_[2],
        -1.0f,  1.0f, 0.0f,  bkg_top_color_[0], bkg_top_color_[1], bkg_top_color_[2],
        1.0f,  1.0f, 0.0f,  bkg_top_color_[0], bkg_top_color_[1], bkg_top_color_[2]
    };
    bkg_vertex_buffer_ = glmBuffer::New(GL_ARRAY_BUFFER);
    bkg_vertex_buffer_->allocate(sizeof(bkg_rt_vertices), bkg_rt_vertices, 0);
    bkg_vao_ = glmVertexArray::New();
    bkg_vao_->bindCurrent();
    bkg_vao_->bindBuffer(*bkg_vertex_buffer_);
    bkg_vao_->getAttrib(0)->setPointer(3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(0));
    bkg_vao_->getAttrib(0)->enable();
    bkg_vao_->getAttrib(1)->setPointer(3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), BUFFER_OFFSET(3 * sizeof(float)));
    bkg_vao_->getAttrib(1)->enable();

    program_ = glmShaderProgram::New();
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
    bkg_program_.reset();
    bkg_vertex_buffer_.reset();
    bkg_vao_.reset();
    program_.reset();
    buffer_.reset();
    indices_buffer_.reset();
    vao_.reset();
}

void glmMeshRenderer::setModelMat(const glm::mat4& mat)
{
    model_ = mat; 
    program_->use();
    program_->setUniformMatrix4fv("model", model_);
}

void glmMeshRenderer::setCameraFovy(float fovy) 
{
    fovy_ = fovy;
    program_->use();
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
    if(bkg_program_){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        bkg_program_->use();
        bkg_vao_->bindCurrent();
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  
    }
    if(vao_){
        program_->use();
        vao_->bindCurrent();
        auto gl_mode = stDisplayModeDict[display_mode_];
        if(gl_mode == GL_POINTS){
            glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(cur_mesh_cloud_->vertices.size()));
        }else{
            glPolygonMode(GL_FRONT_AND_BACK, gl_mode);
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
}

void glmMeshRenderer::resize(float width, float height)
{
    spdlog::info("glmMeshRenderer resize!");
    render_size_ = {width, height};
    if(program_ == nullptr)
        return;
    glViewport(0, 0, (int)width, (int)height);
    win_aspect_ = width / height;
    projection_ = glm::perspective(glm::radians(fovy_), win_aspect_, near_plane_dist_, far_plane_dist_);
    program_->use();
    program_->setUniformMatrix4fv("projection", projection_);
}

GLMESH_NAMESPACE_END