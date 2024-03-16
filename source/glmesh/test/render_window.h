/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: render_window.h 
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

#ifndef __render_window_h__
#define __render_window_h__

#include "WindowQt.h"
#include "glm_win_event_handler_publisher.h"

class glmTrackball;
class RenderWindow : public WindowQt
{
public:
    bool initializeGL() override;
    void loadMeshCloud(glmMeshPtr mesh_cloud);
    void deinitializeGL() override;
    void resizeGL(QResizeEvent* event) override;
    void paintGL() override;    
    void keyPressEvent(QKeyEvent* event) override;    
    glmWinEventHandlerPublisher* handlerRegister(){ return handler_register_.get(); }
    RenderWindow(QApplication& app, QSurfaceFormat& format);
    virtual ~RenderWindow();

protected:
    friend class glmTrackball;
    void applyModelRotate(const glm::quat& rotation);
    void applyFovyChanged();
    void mousePressEvent(QMouseEvent*) override;
    void mouseReleaseEvent(QMouseEvent*) override;
    void mouseMoveEvent(QMouseEvent*) override;
    void wheelEvent(QWheelEvent*) override;
    glmMeshPtr cur_mesh_cloud_;
    glmBufferPtr buffer_;
    glmVertexArrayPtr vao_;
    glmShaderProgramPtr program_;
    glm::mat4 model_;
    glm::mat4 view_;
    glm::vec3 eye_ = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 focal_point_ = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 viewup_ = glm::vec3(0.0f, 1.0f, 0.0f);
    float win_aspect_ = 1.0f;
    float near_plane_dist_ = 0.0f;
    float far_plane_dist_ = 2.0f;
    float fovy_ = glm::radians(20.0f);
    glm::mat4 projection_;
    std::unique_ptr<glmWinEventHandlerPublisher> handler_register_;
    std::unique_ptr<glmTrackball> trackball_;
};

#endif