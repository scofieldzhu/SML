/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: render_window.cpp 
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

#include "render_window.h"
#include <iostream>
#include <QDir>
#include <QResizeEvent>
#include <QApplication>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <spdlog/spdlog.h>
#include "glm_mesh.h"
#include <glad/glad.h>
#include "glm_buffer.h"
#include "glm_vertex_array.h"
#include "glm_vertex_array_attrib.h"
#include "glm_shader_program.h"
#include "glm_trackball.h"

#define BUFFER_OFFSET(a) ((void*)(a))

RenderWindow::RenderWindow(QApplication & app, QSurfaceFormat & format)
    :WindowQt(app, format),
    handler_register_(std::make_unique<glmWinEventHandlerPublisher>()),
    trackball_(std::make_unique<glmTrackball>())
{
    handler_register_->addHandler(trackball_.get());
}

RenderWindow::~RenderWindow()
{
}

void RenderWindow::mouseReleaseEvent(QMouseEvent* event)
{
    WindowQt::mouseReleaseEvent(event);
    spdlog::debug("mouseReleaseEvent:{}", (int)event->button());

    glmWinEvent target_event;
    target_event.source = glmWinEvent::ES_MOUSE_DEVICE;
    target_event.type = glmWinEvent::ET_RELEASE;
    target_event.event_button_id = event->button();
    target_event.pos = {event->pos().x(), event->pos().y()};
    target_event.extra_data = this;
    handler_register_->publish(target_event);
}

void RenderWindow::mouseMoveEvent(QMouseEvent* event)
{
    WindowQt::mouseMoveEvent(event);
    spdlog::debug("mouseMoveEvent:{}", (int)event->button());

    glmWinEvent target_event;
    target_event.source = glmWinEvent::ES_MOUSE_DEVICE;
    target_event.type = glmWinEvent::ET_MOVE;
    target_event.event_button_id = event->button();
    target_event.pos = {event->pos().x(), event->pos().y()};
    target_event.extra_data = this;
    handler_register_->publish(target_event);
}

void RenderWindow::applyModelRotate(const glm::quat& rotation)
{
    makeCurrent();
    model_ = model_ * glm::toMat4(rotation);
    program_->setUniformMatrix4fv("model", model_);
    updateGL();
    doneCurrent();
}

void RenderWindow::mousePressEvent(QMouseEvent *event)
{
    WindowQt::mousePressEvent(event);
    spdlog::debug("mousePressEvent:{}", (int)event->button());

    glmWinEvent target_event;
    target_event.source = glmWinEvent::ES_MOUSE_DEVICE;
    target_event.type = glmWinEvent::ET_PRESSE;
    target_event.event_button_id = event->button();
    target_event.pos = {event->pos().x(), event->pos().y()};
    target_event.extra_data = this;
    handler_register_->publish(target_event);
}

bool RenderWindow::initializeGL() 
{
    if(!gladLoadGL()){
        spdlog::error("gladLoadGL failed!");
        return false;
    }
    program_ = std::make_shared<glmShaderProgram>();
    if(!program_->addShaderFile("shader.vert", GL_VERTEX_SHADER))
        return false;
    if(!program_->addShaderFile("shader.frag", GL_FRAGMENT_SHADER))
        return false;
    if(!program_->link())
        return false;
    program_->use();

    model_ = glm::identity<glm::mat4>();
    program_->setUniformMatrix4fv("model", model_);
    view_  = glm::lookAt(eye_, focal_point_, viewup_);
    program_->setUniformMatrix4fv("view", view_);
    win_aspect_ = (float)width() / (float)height();
    projection_ = glm::perspective(fovy_, win_aspect_, near_plane_dist_, far_plane_dist_);
    program_->setUniformMatrix4fv("projection", projection_);
    return true;
}

void RenderWindow::loadMeshCloud(glmMeshPtr mesh_cloud)
{
    makeCurrent();
    cur_mesh_cloud_ = mesh_cloud;
    if(mesh_cloud->vertex_list.empty()){
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
    far_plane_dist_ = 1.6 * diagonal_len;
    projection_ = glm::perspective(fovy_, win_aspect_, near_plane_dist_, far_plane_dist_);
    program_->setUniformMatrix4fv("projection", projection_);

    buffer_ = std::make_shared<glmBuffer>();
    const GLuint kTotalSize = kVertexSize * mesh_cloud->vertex_list.size();
    buffer_->allocate(kTotalSize, mesh_cloud->vertex_list.data(), 0);
    vao_ = std::make_shared<glmVertexArray>();
    vao_->bindCurrent();
    vao_->bindBuffer(buffer_->id());
    vao_->getAttrib(0)->setPointer(3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    vao_->getAttrib(0)->enable();

    glm::mat4 final_mat = projection_ * view_ * model_;
    glm::vec4 transformed_point = final_mat * glm::vec4(center_point, 1.0);
    spdlog::info("focal_point:[{},{}, {}] transformed point:[{},{}, {}]", 
        center_point[0], center_point[1], center_point[2], 
        transformed_point[0]/transformed_point[3], transformed_point[1]/transformed_point[3], transformed_point[2]/transformed_point[3]
    );

    updateGL();
    doneCurrent();
}

void RenderWindow::deinitializeGL() 
{
    program_.reset();
    buffer_.reset();
    vao_.reset();
}

void RenderWindow::resizeGL(QResizeEvent * event) 
{
    makeCurrent();
    const auto& new_size = event->size();
    glViewport(0, 0, new_size.width(), new_size.height());
    trackball_->setWindowSize(new_size.width(), new_size.height());
    win_aspect_ = (float)new_size.width() / (float)new_size.height();
    spdlog::trace("new_w:{} new_h:{} cur_w:{} cur_h:{}", new_size.width(), new_size.height(), width(), height());
    projection_ = glm::perspective(fovy_, win_aspect_, near_plane_dist_, far_plane_dist_);
    program_->setUniformMatrix4fv("projection", projection_);
    doneCurrent();
}

void RenderWindow::paintGL() 
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    if(vao_){
        vao_->bindCurrent();
        glDrawArrays(GL_POINTS, 0, cur_mesh_cloud_->vertex_list.size());
    }
    spdlog::info("PaintGL called!");
}

void RenderWindow::keyPressEvent(QKeyEvent * event) 
{
    makeCurrent();
    switch (event->key()){
        case Qt::Key_F5:
            // vertex_shader_source_->reload();
            // fragment_shader_source_->reload();
            updateGL();
            break;
        case Qt::Key_Escape:
            qApp->quit();
            break;
        default:
            break;
    }
    doneCurrent();
}
