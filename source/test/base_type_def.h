#ifndef __base_type_def_h__
#define __base_type_def_h__

#include <glm/glm.hpp>
#include <vector>
#include <memory>

using VertexList = std::vector<glm::vec3>;

struct MeshCloud;
using MeshCloudSPtr = std::shared_ptr<MeshCloud>;

#endif