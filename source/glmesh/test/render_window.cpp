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

#define AUTO_MAKE_CURRENT_CALL(CODE) { makeCurrent(); CODE; doneCurrent(); }

RenderWindow::RenderWindow(QWidget* parent, Qt::WindowFlags f)
    :QOpenGLWidget(parent, f),
    renderer_(std::make_shared<glmMeshRenderer>()),
    trackball_(std::make_unique<glmTrackball>(renderer_))
{
    addHandler(trackball_.get());
    //renderer_->setDispalyMode(glmDisplayMode::kFacet);
    setMouseTracking(true);
}

RenderWindow::~RenderWindow()
{
    clear();
    trackball_ = nullptr;
}

void RenderWindow::mouseReleaseEvent(QMouseEvent* event)
{
    QOpenGLWidget::mouseReleaseEvent(event);
    spdlog::debug("mouseReleaseEvent:{}", (int)event->button());

    glmWinEvent target_event;
    target_event.source = glmEventSource::kMouseDevice;
    target_event.type = glmEventType::kRelease;
    target_event.event_button_id = event->button();
    target_event.pos = {event->pos().x(), event->pos().y()};
    target_event.extra_data = this;
    publish(target_event);
    update();
}

void RenderWindow::mouseMoveEvent(QMouseEvent* event)
{
    QOpenGLWidget::mouseMoveEvent(event);
    spdlog::debug("mouseMoveEvent:{}", (int)event->button());

    glmWinEvent target_event;
    target_event.source = glmEventSource::kMouseDevice;
    target_event.type = glmEventType::kMove;
    target_event.event_button_id = event->button();
    target_event.pos = {event->pos().x(), event->pos().y()};
    target_event.extra_data = this;
    publish(target_event);
    update();
}

void RenderWindow::wheelEvent(QWheelEvent* event)
{
    QOpenGLWidget::wheelEvent(event);
    spdlog::debug("wheelEvent");

    glmWinEvent target_event;
    target_event.source = glmEventSource::kMouseDevice;
    target_event.type = glmEventType::kWheelScroll;
    target_event.event_button_id = (int)glmMouseButton::kMiddle;
    target_event.scroll_delta = event->angleDelta().y() / 120.0f;
    target_event.pos = {event->pos().x(), event->pos().y()};
    target_event.extra_data = this;
    publish(target_event);
    update();
}

void RenderWindow::mousePressEvent(QMouseEvent* event)
{
    QOpenGLWidget::mousePressEvent(event);
    spdlog::debug("mousePressEvent:{}", (int)event->button());

    glmWinEvent target_event;
    target_event.source = glmEventSource::kMouseDevice;
    target_event.type = glmEventType::kPress;
    target_event.event_button_id = event->button();
    target_event.pos = {event->pos().x(), event->pos().y()};
    target_event.extra_data = this;
    publish(target_event);
    update();
}

void RenderWindow::initializeGL() 
{
    AUTO_MAKE_CURRENT_CALL(renderer_->initialize(width(), height()))
}

void RenderWindow::loadMeshCloud(glmMeshPtr mesh_cloud)
{
    AUTO_MAKE_CURRENT_CALL(renderer_->loadMeshCloud(mesh_cloud))
    update();    
}

// void RenderWindow::deinitializeGL() 
// {
//     renderer_->destroy();
// }

//void RenderWindow::resizeGL(QResizeEvent* event) 
void RenderWindow::resizeGL(int w, int h) 
{
    const QSize& new_size = {w, h};
    AUTO_MAKE_CURRENT_CALL(renderer_->resize(new_size.width(), new_size.height()))
    
    glmWinEvent target_event;
    target_event.source = glmEventSource::kWindow;
    target_event.type = glmEventType::kResize;
    target_event.win_size = {(float)new_size.width(), (float)new_size.height()};
    target_event.extra_data = this;
    publish(target_event);
    update();
}

void RenderWindow::publish(const glmWinEvent& event)
{
    makeCurrent();
    glmWinEventHandlerPublisher::publish(event);
    doneCurrent();
}

void RenderWindow::paintGL() 
{
    QOpenGLWidget::paintGL();
    renderer_->render();
}

void RenderWindow::keyPressEvent(QKeyEvent * event) 
{
    makeCurrent();
    switch (event->key()){
        case Qt::Key_F5:
            // vertex_shader_source_->reload();
            // fragment_shader_source_->reload();
            paintGL();
            break;
        case Qt::Key_Escape:
            qApp->quit();
            break;
        default:
            break;
    }
    doneCurrent();
}
