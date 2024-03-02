#ifndef __ply_reader_h__
#define __ply_reader_h__

#include "base_type_def.h"
#include <QString>

namespace ply_reader
{
    bool LoadFile(const QString& file, VertexList& result_points);
};

#endif