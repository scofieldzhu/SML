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

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/base/StaticStringSource.h>
#include "WindowQt.h"
#include "base_type_def.h"

class RenderWindow : public WindowQt
{
public:
    bool initializeGL() override;
    void loadMeshCloud(MeshCloudSPtr mesh_cloud);
    void deinitializeGL() override;
    void resizeGL(QResizeEvent * event) override;
    void paintGL() override;    
    void keyPressEvent(QKeyEvent * event) override;    
    RenderWindow(QApplication & app, QSurfaceFormat & format);
    virtual ~RenderWindow();

protected:
    std::unique_ptr<globjects::Buffer> corner_buffer_;
    std::unique_ptr<globjects::Program> program_;
    std::unique_ptr<globjects::File> vertex_shader_source_;
    std::unique_ptr<globjects::AbstractStringSource> vertex_shader_template_;
    std::unique_ptr<globjects::Shader> vertex_shader_;
    std::unique_ptr<globjects::File> fragment_shader_source_;
    std::unique_ptr<globjects::AbstractStringSource> fragment_shader_template_;
    std::unique_ptr<globjects::Shader> fragment_shader_;
    std::unique_ptr<globjects::VertexArray> vao_;
    MeshCloudSPtr cur_mesh_cloud_;
};

#endif