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
#include "glm_shader_program.h"
#include "glm_sphere_actor.h"
#include "glm_bkg_actor.h"
#include "glm_mesh_actor.h"

GLMESH_NAMESPACE_BEGIN

glmMeshRenderer::glmMeshRenderer()
{
    bkg_ = glmBkgActor::New();
    mesh_actor_ = glmMeshActor::New();
}

glmMeshRenderer::~glmMeshRenderer()
{
    destroy();
}

void glmMeshRenderer::setBackgroudBottomColor(const glm::vec3& color)
{
    auto bkg_actor = std::dynamic_pointer_cast<glmBkgActor>(bkg_);
    bkg_actor->setBottomColor(color);
}

glmMeshPtr glmMeshRenderer::currentMeshCloud() const
{
    return std::dynamic_pointer_cast<glmMeshActor>(mesh_actor_)->currentMeshCloud();
}

void glmMeshRenderer::setBackgroudTopColor(const glm::vec3& color)
{
    auto bkg_actor = std::dynamic_pointer_cast<glmBkgActor>(bkg_);
    bkg_actor->setTopColor(color);
}

void glmMeshRenderer::loadMeshCloud(glmMeshPtr mesh_cloud)
{
    std::dynamic_pointer_cast<glmMeshActor>(mesh_actor_)->loadMeshCloud(mesh_cloud);
}

void glmMeshRenderer::setUserColor(const glm::vec4 &color)
{
    std::dynamic_pointer_cast<glmMeshActor>(mesh_actor_)->setUserColor(color);
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

    bkg_->createSource();

    auto mesh_actor = std::dynamic_pointer_cast<glmMeshActor>(mesh_actor_);
    mesh_actor->setRendererSize(width, height);
    mesh_actor->createSource();

    initialized_ = true;

    sphere_ = glmSphereActor::New();
    sphere_->setShaderProgram(mesh_actor->program());
    sphere_->createSource();
    return true;
}

void glmMeshRenderer::destroy()
{
    bkg_.reset();
    mesh_actor_.reset();
}

void glmMeshRenderer::setModelMat(const glm::mat4& mat)
{
    std::dynamic_pointer_cast<glmMeshActor>(mesh_actor_)->setModelMat(mat);
}

glm::mat4 glmMeshRenderer::modelMat()
{
    return std::dynamic_pointer_cast<glmMeshActor>(mesh_actor_)->modelMat();
}

float glmMeshRenderer::cameraFovy() const
{
    return std::dynamic_pointer_cast<glmMeshActor>(mesh_actor_)->cameraFovy();
}

void glmMeshRenderer::setCameraFovy(float fovy) 
{
    std::dynamic_pointer_cast<glmMeshActor>(mesh_actor_)->setCameraFovy(fovy);
}

void glmMeshRenderer::setDispalyMode(glmDisplayMode m)
{
    std::dynamic_pointer_cast<glmMeshActor>(mesh_actor_)->setDispalyMode(m);
}

void glmMeshRenderer::render()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    bkg_->draw();
    if(sphere_){
        auto mesh_actor = std::dynamic_pointer_cast<glmMeshActor>(mesh_actor_);
        auto prog =  mesh_actor->program();
        prog->use();
        prog->setUniformInt("primitive_type", 0);
        prog->setUniformInt("use_vcolor", 1);
        //program_->setUniformVec4("user_color", user_color_);
        sphere_->draw();
    }
    mesh_actor_->draw();
}

void glmMeshRenderer::resize(float width, float height)
{
    spdlog::info("glmMeshRenderer resize!");
    glViewport(0, 0, (int)width, (int)height);
    std::dynamic_pointer_cast<glmMeshActor>(mesh_actor_)->setRendererSize(width, height);
}

GLMESH_NAMESPACE_END