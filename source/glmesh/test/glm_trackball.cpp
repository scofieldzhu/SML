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
#include "render_window.h"

glmTrackball::glmTrackball()
{
}

glmTrackball::~glmTrackball()
{
}

glm::quat glmTrackball::rotate(const glm::vec2& start_pos, const glm::vec2& end_pos) const
{
    if(start_pos == end_pos){
        return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    }
    glm::vec3 start = mapToSphere(start_pos);
    glm::vec3 end = mapToSphere(end_pos);
    glm::vec3 axis = glm::cross(start, end);
    float angle = std::acos(std::min(1.0f, glm::dot(start, end)));
    return glm::angleAxis(angle, glm::normalize(axis));
}

glm::vec3 glmTrackball::mapToSphere(const glm::vec2& win_pos) const
{
    float x = (2.0 * win_pos.x - (float)width_) / (float)width_;
    float y = (height_ - 2.0 * win_pos.y) / (float)height_;
    float len_squared = x * x + y * y;
    if(len_squared <= 1.0f){ //lies inside of sphere 
        return glm::vec3(x, y, sqrt(1.0 - len_squared));
    }else{
        float length = std::sqrt(len_squared);
        return glm::vec3(x / length, y / length, 0.0f);
    }
}

void glmTrackball::handleLeftButtonPressed(const glmWinEvent& event)
{
    left_button_pressed_ = true;
    last_mouse_pos_ = event.pos;
    spdlog::debug("handleLeftButtonPressed! mouse_pos:[{}, {}].", event.pos.x, event.pos.y);
}

void glmTrackball::handleMiddleButtoReleased(const glmWinEvent& event)
{
    spdlog::debug("handleMiddleButtoReleased! mouse_pos:[{}, {}].", event.pos.x, event.pos.y);
}

void glmTrackball::handleMouseMove(const glmWinEvent& event)
{
    spdlog::debug("handleMouseMove! mouse_pos:[{}, {}].", event.pos.x, event.pos.y);
    if(left_button_pressed_){
        auto render_win = static_cast<RenderWindow*>(event.extra_data);
        glm::vec2 now_pos = event.pos;
        auto quad = rotate(last_mouse_pos_, now_pos);
        render_win->applyModelRotate(quad);
        last_mouse_pos_ = now_pos;
    }
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
            if(event.type == glmWinEvent::ET_WHEEL_SCROLL){
                handleWheelScroll(event);
                break;
            }
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

void glmTrackball::handleKeyboardEvent(const glmWinEvent& event)
{
}

void glmTrackball::handleWheelScroll(const glmWinEvent& event)
{
    auto render_win = static_cast<RenderWindow*>(event.extra_data);
    if(render_win){
        render_win->fovy_ -= event.scroll_delta;
        render_win->fovy_ = std::max(1.0f, std::min(render_win->fovy_, 45.0f));
        spdlog::debug("Fovy:{} delta:{}", render_win->fovy_, event.scroll_delta);
        render_win->applyFovyChanged();
    }
}

void glmTrackball::setWindowSize(uint32_t w, uint32_t h)
{
    width_ = w;
    height_ = h;
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
