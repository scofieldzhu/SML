#ifndef __mesh_cloud_h__
#define __mesh_cloud_h__

#include "base_type_def.h"

struct MeshCloud
{
    bool isNull()const{ return vertex_list.empty(); }
    bool isNonNull()const{ return !isNull(); }
    VertexList vertex_list;
};

#endif