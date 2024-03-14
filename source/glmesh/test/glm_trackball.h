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

#include "glm_win_event_handler.h"

class glmTrackball : public glmWinEventHandler
{
public:
    void handleEvent(const glmWinEvent& event) override;
    glmTrackball();
    ~glmTrackball();

private:
    void handleLeftButtonPressed(const glmWinEvent& event);
    void handleLeftButtonReleased(const glmWinEvent& event);
    void handleRightButtonPressed(const glmWinEvent& event);
    void handleRightButtonReleased(const glmWinEvent& event);
    void handleMiddleButtoPressed(const glmWinEvent& event);
    void handleMiddleButtoReleased(const glmWinEvent& event);
    void handleMouseMove(const glmWinEvent& event);
    void handleMouseEvent(const glmWinEvent& event);
    void handleKeyboardEvent(const glmWinEvent& event);
    bool left_button_pressed_ = false;
};

#endif