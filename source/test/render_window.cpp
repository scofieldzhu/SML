#include "render_window.h"
#include <iostream>
#include <QDir>
#include <QResizeEvent>
#include <QApplication>
#include <glbinding/gl/gl.h>
#include <glbinding/Version.h>
#include <glbinding-aux/ContextInfo.h>
#include <glbinding-aux/types_to_string.h>
#include <globjects/logging.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace gl;

RenderWindow::RenderWindow(QApplication & app, QSurfaceFormat & format)
    :WindowQt(app, format)
{
}

RenderWindow::~RenderWindow()
{
}

bool RenderWindow::initializeGL() 
{
    globjects::init(getProcAddress);

    std::cout << std::endl
        << "OpenGL Version:  " << glbinding::aux::ContextInfo::version() << std::endl
        << "OpenGL Vendor:   " << glbinding::aux::ContextInfo::vendor() << std::endl
        << "OpenGL Renderer: " << glbinding::aux::ContextInfo::renderer() << std::endl << std::endl;

    globjects::DebugMessage::enable();

#ifdef __APPLE__
    globjects::Shader::clearGlobalReplacements();
    globjects::Shader::globalReplace("#version 140", "#version 150");

    globjects::debug() << "Using global OS X shader replacement '#version 140' -> '#version 150'" << std::endl;
#endif

    corner_buffer_ = globjects::Buffer::create();
    program_ = globjects::Program::create();
    vao_ = globjects::VertexArray::create();

    std::string data_path = QDir::currentPath().toStdString();
    std::string shader_v_path = data_path + "/shader.vert";        
    vertex_shader_source_ = globjects::Shader::sourceFromFile(shader_v_path);
    vertex_shader_template_ = globjects::Shader::applyGlobalReplacements(vertex_shader_source_.get());
    vertex_shader_ = globjects::Shader::create(GL_VERTEX_SHADER, vertex_shader_template_.get());

    std::string shader_f_path = data_path + "/shader.frag";
    fragment_shader_source_ = globjects::Shader::sourceFromFile(shader_f_path);
    fragment_shader_template_ = globjects::Shader::applyGlobalReplacements(fragment_shader_source_.get());
    fragment_shader_ = globjects::Shader::create(GL_FRAGMENT_SHADER, fragment_shader_template_.get());

    program_->attach(vertex_shader_.get(), fragment_shader_.get());

    corner_buffer_->setData(std::array<glm::vec2, 4>{ {
            glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(0, 1), glm::vec2(1, 1) } }, GL_STATIC_DRAW);

    vao_->binding(0)->setAttribute(0);
    vao_->binding(0)->setBuffer(corner_buffer_.get(), 0, sizeof(glm::vec2));
    vao_->binding(0)->setFormat(2, GL_FLOAT);
    vao_->enable(0);

    return true;
}

void RenderWindow::deinitializeGL() 
{
    corner_buffer_.reset(nullptr);
    program_.reset(nullptr);
    vertex_shader_source_.reset(nullptr);
    vertex_shader_template_.reset(nullptr);
    vertex_shader_.reset(nullptr);
    fragment_shader_source_.reset(nullptr);
    fragment_shader_template_.reset(nullptr);
    fragment_shader_.reset(nullptr);
    vao_.reset(nullptr);
}

void RenderWindow::resizeGL(QResizeEvent * event) 
{
    glViewport(0, 0, event->size().width(), event->size().height());
}

void RenderWindow::paintGL() 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program_->use();
    vao_->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void RenderWindow::keyPressEvent(QKeyEvent * event) 
{
    makeCurrent();

    switch (event->key())
    {
    case Qt::Key_F5:
        vertex_shader_source_->reload();
        fragment_shader_source_->reload();
        updateGL();
        break;
    case Qt::Key_Escape:
        qApp->quit();
        break;
    default:
        break;
    }
    doneCurrent();
}