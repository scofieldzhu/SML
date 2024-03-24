/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: glm_trackball.h 
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

#ifndef __glm_trackball_h__
#define __glm_trackball_h__

#include <glm/gtx/quaternion.hpp>
#include "glmesh/core/glm_win_event_handler.h"
#include "glmesh/core/glm_export.h"

GLMESH_NAMESPACE_BEGIN

class GLMESH_API glmTrackball : public glmWinEventHandler
{
public:
    void bindRotationToMouseButton(glmMouseButton button);
    void handleEvent(const glmWinEvent& event) override;
    glmTrackball(glmMeshRendererPtr ren);
    ~glmTrackball();

private:
    glm::quat rotate(const glm::vec2& start_pos, const glm::vec2& end_pos)const;
    glm::vec3 mapToSphere(const glm::vec2& win_pos)const;
    void handleMouseMove(const glmWinEvent& event);
    void handleMouseEvent(const glmWinEvent& event);
    void handleKeyboardEvent(const glmWinEvent& event);
    void handleWheelScroll(const glmWinEvent& event);
    void handleWindowEvent(const glmWinEvent& event);
    void handleResize(const glmWinEvent& event);
    float width_ = 0.0f;
    float height_ = 0.0f;
    glm::vec2 tracking_mouse_pos_;
    glmMeshRendererPtr renderer_;
    glmMouseButton rotation_button_ = glmMouseButton::kNone;
    glmMouseButton current_pressed_button_ = glmMouseButton::kNone;
};

GLMESH_NAMESPACE_END

#endif