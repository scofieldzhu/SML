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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "mesh_cloud.h"
using namespace gl;

constexpr size_t kMaxVertexDataSize = 1024 * 1024 * 500;

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
    return true;
}

void RenderWindow::loadMeshCloud(MeshCloudSPtr mesh_cloud)
{
    cur_mesh_cloud_ = mesh_cloud;
    if(mesh_cloud->vertex_list.empty()){
        globjects::debug() << "Mesh cloud contain a empty vertex list!" << std::endl;
        return;
    }
    const gl::GLsizeiptr kDataSize = mesh_cloud->vertex_list.size() * sizeof(glm::vec3);
    corner_buffer_->setData(kDataSize, static_cast<gl::GLvoid*>(mesh_cloud->vertex_list.data()), GL_STATIC_DRAW);
    vao_->binding(0)->setAttribute(0);
    vao_->binding(0)->setBuffer(corner_buffer_.get(), 0, sizeof(glm::vec3));
    vao_->binding(0)->setFormat(2, GL_FLOAT);
    vao_->enable(0);
    updateGL();
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
    if(cur_mesh_cloud_ && cur_mesh_cloud_->isNonNull())
        vao_->drawArrays(GL_TRIANGLE_STRIP, 0, cur_mesh_cloud_->vertex_list.size() / 3);
}

void RenderWindow::keyPressEvent(QKeyEvent * event) 
{
    makeCurrent();
    switch (event->key()){
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