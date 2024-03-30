/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: glm_mesh_renderer.h 
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

#ifndef __glm_mesh_renderer_h__
#define __glm_mesh_renderer_h__

#include <glm/gtx/quaternion.hpp>
#include "glmesh/core/glm_base_type.h"
#include "glmesh/core/glm_export.h"
#include "glmesh/core/glm_object.h"

GLMESH_NAMESPACE_BEGIN

class GLMESH_API glmMeshRenderer : public glmObject<glmMeshRenderer>
{
public:
    void setBackgroudTopColor(const glm::vec3& color);
    const auto& backgroudTopColor()const{ return bkg_top_color_; }
    void setBackgroudBottomColor(const glm::vec3& color);
    const auto& backgroudBottomColor()const{ return bkg_bottom_color_; }
    auto currentMeshCloud()const{ return cur_mesh_cloud_; }
    void loadMeshCloud(glmMeshPtr mesh_cloud);
    void setUserColor(const glm::vec4& color);
    bool initialize(float width, float height);
    void destroy();
    void resize(float width, float height);
    glm::vec2 renderSize()const{ return render_size_; }
    void setModelMat(const glm::mat4& mat);
    glm::mat4 modelMat(){ return model_; }
    float cameraFovy()const{ return fovy_; }
    void setCameraFovy(float fovy);
    void setDispalyMode(glmDisplayMode m);
    auto displayMode()const;
    void resetView();
    void render();
    glmMeshRenderer();
    ~glmMeshRenderer();

private:
    glm::vec3 bkg_top_color_ = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 bkg_bottom_color_ = glm::vec3(0.5f, 0.5f, 1.0f);
    glm::vec2 render_size_;
    glm::vec4 user_color_ = glm::vec4(1.0, 1.0, 0.0, 1.0);
    glm::vec3 user_normal_ = glm::vec3(0.0f, 0.0f, 1.0f);
    glmMeshPtr cur_mesh_cloud_;
    glmBufferPtr buffer_;
    glmBufferPtr indices_buffer_;
    glmVertexArrayPtr vao_;
    glmShaderProgramPtr program_;
    glmShaderProgramPtr bkg_program_;
    glmVertexArrayPtr bkg_vao_;
    glmBufferPtr bkg_vertex_buffer_;
    glm::mat4 model_;
    glm::mat4 view_;
    glm::vec3 eye_ = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 focal_point_ = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 viewup_ = glm::vec3(0.0f, 1.0f, 0.0f);
    float win_aspect_ = 1.0f;
    float near_plane_dist_ = 0.0f;
    float far_plane_dist_ = 2.0f;
    float fovy_ = 45.0f;
    glm::mat4 projection_;
    glmDisplayMode display_mode_ = glmDisplayMode::kPoint;
    bool initialized_ = false;
};

GLMESH_NAMESPACE_END

#endif