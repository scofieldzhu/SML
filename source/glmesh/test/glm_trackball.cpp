/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: glm_trackball.cpp 
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

#include "glm_trackball.h"
#include <spdlog/spdlog.h>

glmTrackball::glmTrackball()
{
}

glmTrackball::~glmTrackball()
{
}

void glmTrackball::handleLeftButtonPressed(const glmWinEvent& event)
{
    left_button_pressed_ = true;
    spdlog::debug("handleLeftButtonPressed! mouse_pos:[{}, {}].", event.pos.x, event.pos.y);
}

void glmTrackball::handleMiddleButtoReleased(const glmWinEvent& event)
{
    spdlog::debug("handleMiddleButtoReleased! mouse_pos:[{}, {}].", event.pos.x, event.pos.y);
}

void glmTrackball::handleMouseMove(const glmWinEvent& event)
{
    spdlog::debug("handleMouseMove! mouse_pos:[{}, {}].", event.pos.x, event.pos.y);
}

void glmTrackball::handleMiddleButtoPressed(const glmWinEvent& event)
{
    spdlog::debug("handleMiddleButtoPressed! mouse_pos:[{}, {}].", event.pos.x, event.pos.y);
}

void glmTrackball::handleRightButtonReleased(const glmWinEvent& event)
{
    spdlog::debug("handleRightButtonReleased! mouse_pos:[{}, {}].", event.pos.x, event.pos.y);
}

void glmTrackball::handleRightButtonPressed(const glmWinEvent& event)
{
    spdlog::debug("handleRightButtonPressed! mouse_pos:[{}, {}].", event.pos.x, event.pos.y);
}

void glmTrackball::handleLeftButtonReleased(const glmWinEvent& event)
{
    left_button_pressed_ = false;
    spdlog::debug("handleLeftButtonReleased! mouse_pos:[{}, {}].", event.pos.x, event.pos.y);
}

void glmTrackball::handleMouseEvent(const glmWinEvent& event)
{
    switch(event.event_button_id){
        case glmWinEvent::MB_LEFT:
            if(event.type == glmWinEvent::ET_PRESSE){
                handleLeftButtonPressed(event);
                break;
            }
            if(event.type == glmWinEvent::ET_RELEASE){
                handleLeftButtonReleased(event);
                break;
            }
            break;

        case glmWinEvent::MB_MIDDLE:
            break;

        case glmWinEvent::MB_RIGHT:
            break;

        case glmWinEvent::MB_NULL:
            if(event.type == glmWinEvent::ET_MOVE){
                handleMouseMove(event);
                break;
            }
            break;
        
        default:
            break;
    }
}

void glmTrackball::handleKeyboardEvent(const glmWinEvent &event)
{
}

void glmTrackball::handleEvent(const glmWinEvent& event)
{
    if(event.source == glmWinEvent::ES_MOUSE_DEVICE){
        handleMouseEvent(event);
        return;
    }
    if(event.source == glmWinEvent::ES_KEYBOARD){
        handleKeyboardEvent(event);
        return;
    }
    spdlog::warn("Invalid event source:{} identified!", (int)event.source);
}
