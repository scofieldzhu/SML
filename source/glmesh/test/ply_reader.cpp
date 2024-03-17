/* 
 *  glmesh is a mesh data render library base on QOpengl.
 *  glmesh provides object-oriented interfaces to the OpenGL API (3.0 and higher). 
 *  It reduces the amount of OpenGL code required for rendering and facilitates 
 *  coherent OpenGL.
 *  
 *  File: ply_reader.cpp 
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

#include "ply_reader.h"
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h> 
#include <pcl/point_cloud.h> 
#include <pcl/PolygonMesh.h>
#include <spdlog/spdlog.h>
using namespace pcl;
using namespace glmesh;

bool ply_reader::LoadFile(const QString& filename, glmesh::glmMesh& result_mesh)
{
    {
        PointCloud<PointXYZ>::Ptr cloud(new PointCloud<PointXYZ>);
        PLYReader reader;
        if(reader.read(filename.toLocal8Bit().toStdString().c_str(), *cloud) == -1){
            SPDLOG_ERROR("Read point data from ply file:'{}' failed!", filename.toLocal8Bit().toStdString());
            return false;
        }
        result_mesh.vertex_pts.clear();
        for(const auto& pt : *cloud)
            result_mesh.vertex_pts.push_back({pt.x, pt.y, pt.z});
        SPDLOG_INFO("Read {} vertexes from cloud file:{} successfully!", result_mesh.vertex_pts.size(), filename.toLocal8Bit().toStdString());        
    }
    {
        pcl::PolygonMesh mesh;
        if (pcl::io::loadPLYFile(filename.toLocal8Bit().toStdString().c_str(), mesh) == -1) {
            SPDLOG_ERROR("Read mesh data from ply file:'{}' failed!", filename.toLocal8Bit().toStdString());
            return true;
        }
        result_mesh.facets.clear();
        for(const auto& vt : mesh.polygons){
            glmFacet ft;
            for(const auto& id : vt.vertices)
                ft.indices.push_back(id);   
            result_mesh.facets.push_back(std::move(ft));
        }
    }
    return true;
}