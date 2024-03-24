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
#include <QFileDialog>
#include <spdlog/spdlog.h>
#include <glmesh/core/glm_mesh.h>
#include <glmesh/core/glm_mesh_renderer.h>
#include <glmesh/core/glm_trackball.h>
#include "ply_reader.h"

MainWindow::MainWindow(QApplication& app, QSurfaceFormat& sf)
    :QMainWindow(nullptr, Qt::WindowFlags())
{
    setMinimumSize(1024, 480);
    setWindowTitle("3D mesh surface rendering control window");
    ui.setupUi(this);
    
    ren_window_ = new RenderWindow(ui.centralwidget, Qt::WindowFlags());
    ren_window_->setFormat(sf);
    auto hbl = dynamic_cast<QHBoxLayout*>(ui.centralwidget->layout());
    hbl->insertWidget(0, ren_window_);
    hbl->update();
    ui.rotate_key_cb->addItem("No Button");
    ui.rotate_key_cb->addItem("Left Button");
    ui.rotate_key_cb->addItem("Right Button");
    ui.rotate_key_cb->addItem("Middle Button");
    ui.rotate_key_cb->setCurrentIndex(0);
    connect(ui.rotate_key_cb, SIGNAL(currentIndexChanged(int)), this, SLOT(onRotationButtonChanged(int)));

    connect(ui.actionLoad_Mesh_Data, SIGNAL(triggered(bool)), this, SLOT(onMenuItemSlot_LoadMeshData(bool)));
    ui.actionDM_Points->setChecked(true);
    connect(ui.actionDM_Points, SIGNAL(triggered(bool)), this, SLOT(onMenuItemSlot_DM_Points(bool)));
    connect(ui.actionDM_Wire, SIGNAL(triggered(bool)), this, SLOT(onMenuItemSlot_DM_Wire(bool)));
    connect(ui.actionDM_Facet, SIGNAL(triggered(bool)), this, SLOT(onMenuItemSlot_DM_Facet(bool)));
}

MainWindow::~MainWindow()
{
}

void MainWindow::onRotationButtonChanged(int index)
{
    ren_window_->trackball()->bindRotationToMouseButton(static_cast<glmesh::glmMouseButton>(index));
}

void MainWindow::onMenuItemSlot_DM_Points(bool checked)
{
    if(ren_window_->existMeshData()){
        ren_window_->renderer()->setDispalyMode(glmesh::glmDisplayMode::kPoint);
        ui.actionDM_Points->setChecked(true);
        ui.actionDM_Wire->setChecked(false);
        ui.actionDM_Facet->setChecked(false);
    }
}

void MainWindow::onMenuItemSlot_DM_Facet(bool checked)
{
    if(ren_window_->existMeshData()){
        ren_window_->renderer()->setDispalyMode(glmesh::glmDisplayMode::kFacet);
        ui.actionDM_Facet->setChecked(true);
        ui.actionDM_Wire->setChecked(false);
        ui.actionDM_Points->setChecked(false);
    }
}

void MainWindow::onMenuItemSlot_DM_Wire(bool checked)
{    
    if(ren_window_->existMeshData()){
        ren_window_->renderer()->setDispalyMode(glmesh::glmDisplayMode::kWire);
        ui.actionDM_Wire->setChecked(true);
        ui.actionDM_Facet->setChecked(false);
        ui.actionDM_Points->setChecked(false);
    }
}

void MainWindow::onMenuItemSlot_LoadMeshData(bool checked)
{
    spdlog::info("Load mesh data");

    QString file_name = QFileDialog::getOpenFileName(this, "Open file", last_mesh_dir_.isEmpty() ? QCoreApplication::applicationDirPath() : last_mesh_dir_, "PLY files (*.ply)");
    if(file_name.isEmpty()){
        return; // user cancel
    }
    last_mesh_dir_ = QFileInfo(file_name).absoluteDir().absolutePath();
    glmesh::glmMeshPtr mesh_cloud = std::make_shared<glmesh::glmMesh>();
    ply_reader::LoadFile(file_name, *mesh_cloud, false);
    ren_window_->loadMeshCloud(mesh_cloud);

    //update mesh information label
    ui.mesh_info_label->setText(QString(" Vertex count: %1 \n Color count: %2 \n Triangle facet count: %3 \n Polygon facet count:%4")
        .arg(mesh_cloud->vertices.size())
        .arg(mesh_cloud->colors.size())
        .arg(mesh_cloud->triangle_facets.size())
        .arg(mesh_cloud->poly_facets.size()));
}
