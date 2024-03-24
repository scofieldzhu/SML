/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: main_window.cpp 
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

#include "main_window.h"
#include <QBoxLayout>

MainWindow::MainWindow(QApplication& app, QSurfaceFormat& sf)
    :QMainWindow(nullptr, Qt::WindowFlags())
{
    setMinimumSize(1024, 480);
    setWindowTitle("3D mesh surface rendering control window");
    ui.setupUi(this);
    
    ren_window = new RenderWindow(ui.centralwidget, Qt::WindowFlags());
    ren_window->setFormat(sf);
    auto hbl = dynamic_cast<QHBoxLayout*>(ui.centralwidget->layout());
    hbl->insertWidget(0, ren_window);
    hbl->update();

    // QVBoxLayout* vbl = new QVBoxLayout();
    // vbl->setContentsMargins(0, 0, 0, 0);
    // vbl->setSpacing(0);
    // vbl->addWidget(QWidget::createWindowContainer(ren_window, ui.renshellwidget));
    // ui.renshellwidget->setLayout(vbl);
    // ui.renshellwidget->update();
}

MainWindow::~MainWindow()
{
}
