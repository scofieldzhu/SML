
set(PROJECT_NAME "glmesh.core")

project(${PROJECT_NAME})

find_package(Qt5 COMPONENTS Core Widgets Opengl REQUIRED)
find_package(glm REQUIRED COMPONENTS glm::glm)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${PROJECT_NAME}
    SHARED
    ${SOURCE_FILES}
)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        GLMESH_EXPORT
    )
endif()

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} # for UIC generated headers
    ${GLM_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    Qt5::Core
    Qt5::Widgets
    spdlog::spdlog
    glm::glm
)



