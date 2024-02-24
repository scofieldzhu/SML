
#include <iostream>
#include <algorithm>

#include <QApplication>
#include <QMainWindow>
#include <QResizeEvent>
#include <QSurfaceFormat>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glbinding/gl/gl.h>
#include <glbinding/Version.h>
#include <glbinding-aux/ContextInfo.h>
#include <glbinding-aux/types_to_string.h>

#include <globjects/globjects.h>
#include <globjects/base/File.h>
#include <globjects/logging.h>

#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/base/StaticStringSource.h>

#include <QDir>

#include "WindowQt.h"

using namespace gl;


class Window : public WindowQt
{
public:
    Window(QApplication & app, QSurfaceFormat & format)
    : WindowQt(app, format)
    {
    }

    virtual ~Window()
    {
    }

    virtual bool initializeGL() override
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

        m_cornerBuffer = globjects::Buffer::create();
        m_program = globjects::Program::create();
        m_vao = globjects::VertexArray::create();

        std::string data_path = QDir::currentPath().toStdString();
        std::string shader_v_path = data_path + "/shader.vert";        
        m_vertexShaderSource = globjects::Shader::sourceFromFile(shader_v_path);
        m_vertexShaderTemplate = globjects::Shader::applyGlobalReplacements(m_vertexShaderSource.get());
        m_vertexShader = globjects::Shader::create(GL_VERTEX_SHADER, m_vertexShaderTemplate.get());

        std::string shader_f_path = data_path + "/shader.frag";
        m_fragmentShaderSource = globjects::Shader::sourceFromFile(shader_f_path);
        m_fragmentShaderTemplate = globjects::Shader::applyGlobalReplacements(m_fragmentShaderSource.get());
        m_fragmentShader = globjects::Shader::create(GL_FRAGMENT_SHADER, m_fragmentShaderTemplate.get());

        m_program->attach(m_vertexShader.get(), m_fragmentShader.get());

        m_cornerBuffer->setData(std::array<glm::vec2, 4>{ {
                glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(0, 1), glm::vec2(1, 1) } }, GL_STATIC_DRAW);

        m_vao->binding(0)->setAttribute(0);
        m_vao->binding(0)->setBuffer(m_cornerBuffer.get(), 0, sizeof(glm::vec2));
        m_vao->binding(0)->setFormat(2, GL_FLOAT);
        m_vao->enable(0);

        return true;
    }

    virtual void deinitializeGL() override
    {
        m_cornerBuffer.reset(nullptr);
        m_program.reset(nullptr);
        m_vertexShaderSource.reset(nullptr);
        m_vertexShaderTemplate.reset(nullptr);
        m_vertexShader.reset(nullptr);
        m_fragmentShaderSource.reset(nullptr);
        m_fragmentShaderTemplate.reset(nullptr);
        m_fragmentShader.reset(nullptr);
        m_vao.reset(nullptr);
    }

    virtual void resizeGL(QResizeEvent * event) override
    {
        glViewport(0, 0, event->size().width(), event->size().height());
    }

    virtual void paintGL() override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_program->use();
        m_vao->drawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }


    virtual void keyPressEvent(QKeyEvent * event) override
    {
        makeCurrent();

        switch (event->key())
        {
        case Qt::Key_F5:
            m_vertexShaderSource->reload();
            m_fragmentShaderSource->reload();
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


protected:
    std::unique_ptr<globjects::Buffer> m_cornerBuffer;
    std::unique_ptr<globjects::Program> m_program;
    std::unique_ptr<globjects::File> m_vertexShaderSource;
    std::unique_ptr<globjects::AbstractStringSource> m_vertexShaderTemplate;
    std::unique_ptr<globjects::Shader> m_vertexShader;
    std::unique_ptr<globjects::File> m_fragmentShaderSource;
    std::unique_ptr<globjects::AbstractStringSource> m_fragmentShaderTemplate;
    std::unique_ptr<globjects::Shader> m_fragmentShader;
    std::unique_ptr<globjects::VertexArray> m_vao;
};


int main(int argc, char * argv[])
{
    QApplication app(argc, argv);

    QSurfaceFormat format;
#ifdef __APPLE__
    format.setVersion(3, 2);
    format.setProfile(QSurfaceFormat::CoreProfile);
#else
    format.setVersion(3, 1);
#endif
    format.setDepthBufferSize(16);

    std::unique_ptr<Window> glwindow(new Window(app, format));

    QMainWindow window;
    window.setMinimumSize(640, 480);
    window.setWindowTitle("globjects and Qt");
    window.setCentralWidget(QWidget::createWindowContainer(glwindow.release()));

    window.show();

    return app.exec();
}

