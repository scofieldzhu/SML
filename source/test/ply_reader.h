#ifndef __ply_reader_h__
#define __ply_reader_h__

#include <glm/glm.hpp>
#include <QString>
#include <QVector>

namespace ply_reader
{
    bool LoadFile(const QString& file, QVector<glm::vec3>& result_points);
};

#endif