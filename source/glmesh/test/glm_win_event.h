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

#include "base_type_def.h"

struct glmWinEvent
{
    enum EventSource
    {
        ES_NULL,
        ES_MOUSE_DEVICE,
        ES_KEYBOARD
    };
    
    enum MouseButton
    {
        MB_NULL,
        MB_LEFT,
        MB_RIGHT,
        MB_MIDDLE
    };
    
    enum EventType
    {
        ET_NULL,
        ET_PRESSE,
        ET_DOUBLE_PRESS,
        ET_RELEASE,
        ET_MOVE,
        ET_WHEEL_SCROLL
    };
    
    enum ScrollDirection
    {
        SD_NULL,
        SD_DOWN,
        SD_UP
    };

    EventSource source = ES_NULL;
    EventType type = ET_NULL;
    int event_button_id = -1;
    ScrollDirection scroll_direction = SD_NULL;
    float scroll_angle = 0.0f;
    glm::vec2 pos{0.0f, 0.0f};
};

#endif