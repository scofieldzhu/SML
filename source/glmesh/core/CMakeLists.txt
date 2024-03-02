
set(PROJECT_NAME "glmesh.core")

project(${PROJECT_NAME})

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(
    ${PROJECT_NAME}
    shared
    ${SOURCE_FILES}
)



