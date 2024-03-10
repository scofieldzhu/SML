/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: WindowQt.cpp 
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

#include "WindowQt.h"

#include <QApplication>
#include <QDebug>
#include <QResizeEvent>
#include <QSurfaceFormat>
#include <QOpenGLContext>


WindowQt * WindowQt::s_getProcAddressHelper = nullptr;


QSurfaceFormat defaultFormat()
{
    QSurfaceFormat format;
    format.setProfile(QSurfaceFormat::CoreProfile);
#ifndef NDEBUG
    format.setOption(QSurfaceFormat::DebugContext);
#endif
    return format;
}

WindowQt::WindowQt(QApplication & app, const QSurfaceFormat & format)
: m_context(new QOpenGLContext)
, m_updatePending(false)
, m_initialized(false)
{
    if (!s_getProcAddressHelper)
    {
        s_getProcAddressHelper = this;
    }

    QSurfaceFormat f(format);
    f.setRenderableType(QSurfaceFormat::OpenGL);   
    setSurfaceType(OpenGLSurface);
    create();

    m_context->setFormat(format);
    if (!m_context->create())
    {
        qDebug() << "Could not create OpenGL context.";
        QApplication::quit();
    }

    QObject::connect(&app, &QGuiApplication::lastWindowClosed, [this]() {
        if (m_initialized)
        {
            makeCurrent();

            deinitializeGL();

            doneCurrent();
        }
    });
}

WindowQt::~WindowQt()
{
    // cannot deinitialize OpenGL here as it would require the call of a virtual member function
}

QOpenGLContext * WindowQt::context()
{
    return m_context.data();
}

void WindowQt::makeCurrent()
{
    m_context->makeCurrent(this);
}

void WindowQt::doneCurrent()
{
    m_context->doneCurrent();
}

void WindowQt::resizeEvent(QResizeEvent * event)
{
    resize(event);
    paint();
}

void WindowQt::exposeEvent(QExposeEvent * )
{
    paint();
}

void WindowQt::initialize()
{
    makeCurrent();

    m_initialized = initializeGL();

    doneCurrent();
}

void WindowQt::resize(QResizeEvent * event)
{
    if (!m_initialized)
        initialize();

    makeCurrent();

    QResizeEvent deviceSpecificResizeEvent(event->size() * devicePixelRatio(), event->oldSize() * devicePixelRatio());

    resizeGL(&deviceSpecificResizeEvent);

    doneCurrent();
}

void WindowQt::paint()
{
    if (!m_initialized)
        initialize();

    if (!isExposed())
        return;

    m_updatePending = false;

    makeCurrent();

    paintGL();

    m_context->swapBuffers(this);

    doneCurrent();
}

void WindowQt::updateGL()
{
    if (!m_updatePending)
    {
        m_updatePending = true;
        QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
    }
}

bool WindowQt::event(QEvent * event)
{
    switch (event->type())
    {
    case QEvent::UpdateRequest:
        paint();
        return true;

    case QEvent::Enter:
        enterEvent(event);
        return true;

    case QEvent::Leave:
        leaveEvent(event);
        return true;

    default:
        return QWindow::event(event);
    }
}

bool WindowQt::initializeGL()
{
    return false;
}

void WindowQt::deinitializeGL()
{
}

void WindowQt::resizeGL(QResizeEvent *)
{
}

void WindowQt::paintGL()
{
    updateGL();
}

void WindowQt::enterEvent(QEvent *)
{
}

void WindowQt::leaveEvent(QEvent *)
{
}

glbinding::ProcAddress WindowQt::getProcAddress(const char * name)
{
    if (!s_getProcAddressHelper || name == nullptr)
    {
        return nullptr;
    }

    const auto symbol = std::string(name);

    #if (QT_VERSION >= QT_VERSION_CHECK(5, 4, 0))
    const auto qtSymbol = QByteArray::fromStdString(symbol);
    #else
    const auto qtSymbol = QByteArray::fromRawData(symbol.c_str(), symbol.size());
    #endif
    return s_getProcAddressHelper->m_context->getProcAddress(qtSymbol);
}
