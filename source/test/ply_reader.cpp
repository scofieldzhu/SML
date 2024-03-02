#include "ply_reader.h"
#include <QFile>
#include <QDebug>
#include <pcl/io/ply_io.h>
#include <pcl/point_types.h> 
#include <pcl/point_cloud.h> 
#include <spdlog/spdlog.h>
using namespace pcl;

bool ply_reader::LoadFile(const QString& filename, VertexList& vertices)
{
    PointCloud<PointXYZ>::Ptr cloud(new PointCloud<PointXYZ>);
    PLYReader reader;
    if(reader.read(filename.toLocal8Bit().toStdString().c_str(), *cloud) == -1){
        SPDLOG_ERROR("Read cloud data from ply file:'{}' failed!", filename.toLocal8Bit().toStdString());
        return false;
    }
    vertices.clear();
    for(const auto& pt : *cloud)
        vertices.push_back({pt.x, pt.y, pt.z});
    SPDLOG_INFO("Read {} vertexes from cloud file:{} successfully!"), filename.toLocal8Bit().toStdString());
    return true;
}