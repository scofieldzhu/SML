/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: glm_win_event.h 
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

#ifndef __glm_win_event_h__
#define __glm_win_event_h__

#include "glmesh/core/glm_base_type.h"

GLMESH_NAMESPACE_BEGIN

enum class glmMouseButton
{
    kNone,
    kLeft,
    kRight,
    kMiddle
};

enum class glmEventSource
{
    kNull,
    kMouseDevice,
    kKeyboard,
    kWindow
};

enum class glmEventType
{
    kNull,
    kPress,
    kDoublePress,
    kRelease,
    kMove,
    kWheelScroll,
    kResize
};

struct glmWinEvent
{
    glmEventSource source = glmEventSource::kNull;
    glmEventType type = glmEventType::kNull;
    int event_button_id = -1;
    float scroll_delta = 0.0f;
    glm::vec2 pos{0.0f, 0.0f};
    glm::vec2 win_size{0.0f, 0.0f};
    void* extra_data = nullptr;
};

GLMESH_NAMESPACE_END

#endif