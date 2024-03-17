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
#include <QDir>
#include <QResizeEvent>
#include <QApplication>
#include <spdlog/spdlog.h>
#include "glmesh/core/glm_trackball.h"
#include "glmesh/core/glm_mesh_renderer.h"

using namespace glmesh;

RenderWindow::RenderWindow(QApplication & app, QSurfaceFormat & format)
    :WindowQt(app, format),
    renderer_(std::make_shared<glmMeshRenderer>()),
    handler_register_(std::make_unique<glmWinEventHandlerPublisher>()),
    trackball_(std::make_unique<glmTrackball>(renderer_))
{
    handler_register_->addHandler(trackball_.get());
    //renderer_->setDispalyMode(glmDisplayMode::kFacet);
}

RenderWindow::~RenderWindow()
{
    trackball_ = nullptr;
}

void RenderWindow::mouseReleaseEvent(QMouseEvent* event)
{
    WindowQt::mouseReleaseEvent(event);
    spdlog::debug("mouseReleaseEvent:{}", (int)event->button());

    makeCurrent();
    glmWinEvent target_event;
    target_event.source = glmWinEvent::ES_MOUSE_DEVICE;
    target_event.type = glmWinEvent::ET_RELEASE;
    target_event.event_button_id = event->button();
    target_event.pos = {event->pos().x(), event->pos().y()};
    target_event.extra_data = this;
    handler_register_->publish(target_event);
    updateGL();
    doneCurrent();
}

void RenderWindow::mouseMoveEvent(QMouseEvent* event)
{
    WindowQt::mouseMoveEvent(event);
    spdlog::debug("mouseMoveEvent:{}", (int)event->button());

    makeCurrent();
    glmWinEvent target_event;
    target_event.source = glmWinEvent::ES_MOUSE_DEVICE;
    target_event.type = glmWinEvent::ET_MOVE;
    target_event.event_button_id = event->button();
    target_event.pos = {event->pos().x(), event->pos().y()};
    target_event.extra_data = this;
    handler_register_->publish(target_event);
    updateGL();
    doneCurrent();
}

void RenderWindow::wheelEvent(QWheelEvent* event)
{
    WindowQt::wheelEvent(event);
    spdlog::debug("wheelEvent");

    makeCurrent();
    glmWinEvent target_event;
    target_event.source = glmWinEvent::ES_MOUSE_DEVICE;
    target_event.type = glmWinEvent::ET_WHEEL_SCROLL;
    target_event.event_button_id = glmWinEvent::MB_MIDDLE;
    target_event.scroll_delta = event->angleDelta().y() / 120.0f;
    target_event.pos = {event->pos().x(), event->pos().y()};
    target_event.extra_data = this;
    handler_register_->publish(target_event);
    updateGL();
    doneCurrent();
}

void RenderWindow::mousePressEvent(QMouseEvent* event)
{
    WindowQt::mousePressEvent(event);
    spdlog::debug("mousePressEvent:{}", (int)event->button());

    makeCurrent();
    glmWinEvent target_event;
    target_event.source = glmWinEvent::ES_MOUSE_DEVICE;
    target_event.type = glmWinEvent::ET_PRESSE;
    target_event.event_button_id = event->button();
    target_event.pos = {event->pos().x(), event->pos().y()};
    target_event.extra_data = this;
    handler_register_->publish(target_event);
    updateGL();
    doneCurrent();
}

bool RenderWindow::initializeGL() 
{
    return renderer_->initialize(width(), height());
}

void RenderWindow::loadMeshCloud(glmMeshPtr mesh_cloud)
{
    makeCurrent();
    renderer_->loadMeshCloud(mesh_cloud);
    updateGL();
    doneCurrent();
}

void RenderWindow::deinitializeGL() 
{
    renderer_->destroy();
}

void RenderWindow::resizeGL(QResizeEvent* event) 
{
    makeCurrent();
    const auto& new_size = event->size();
    renderer_->resize(new_size.width(), new_size.height());

    glmWinEvent target_event;
    target_event.source = glmWinEvent::ES_WIN;
    target_event.type = glmWinEvent::ET_RESIZE;
    target_event.win_size = {(float)new_size.width(), (float)new_size.height()};
    target_event.extra_data = this;
    handler_register_->publish(target_event);
    updateGL();
    doneCurrent();
}

void RenderWindow::paintGL() 
{
    renderer_->render();
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
