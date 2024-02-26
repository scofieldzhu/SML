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

class RenderWindow : public WindowQt
{
public:
    bool initializeGL() override;
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
};

#endif