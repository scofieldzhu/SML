
find_package(globjects)
find_package(glbinding REQUIRED)
#find_package(cpplocate REQUIRED)
find_package(Qt5Core    5.1)
find_package(Qt5Gui     5.1)
find_package(Qt5Widgets 5.1)
find_package(PCL CONFIG REQUIRED)

# Enable automoc
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
set_property(GLOBAL PROPERTY AUTOMOC_FOLDER CMakeAutomocTargets)

# ENABLE CMP0020: Automatically link Qt executables to qtmain target on Windows.
cmake_policy(SET CMP0020 NEW)

# # Target name
set(target glmesh.test)

# # Exit here if required dependencies are not met
if (NOT Qt5Core_FOUND)
    message(STATUS "${target} skipped: Qt5 not found")
    return()
else()
    message(STATUS "${target}")
endif()


file(GLOB SOURCE_FILES 
    "*.h"
    "*.hpp"
    "*.cpp"
    "*.vert"
    "*.fraq"
    )

# Build executable
add_executable(${target}
    ${SOURCE_FILES}
)

target_include_directories(${target}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} # for UIC generated headers
)

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    ${PCL_LIBRARIES}
    globjects::globjects
    # cpplocate::cpplocate
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
    spdlog::spdlog
)

# # 
# # Compile definitions
# # 

# target_compile_definitions(${target}
#     PRIVATE
#     ${DEFAULT_COMPILE_DEFINITIONS}
#     GLM_ENABLE_EXPERIMENTAL
# )


# # 
# # Compile options
# # 

# target_compile_options(${target}
#     PRIVATE
#     ${DEFAULT_COMPILE_OPTIONS}
# )


# # 
# # Linker options
# # 

# target_link_libraries(${target}
#     PRIVATE
#     ${DEFAULT_LINKER_OPTIONS}
# )


# #
# # Target Health
# #

# perform_health_checks(
#     ${target}
#     ${sources}
# )


# # 
# # Deployment
# # 

# # Executable
# install(TARGETS ${target}
#     RUNTIME DESTINATION ${INSTALL_EXAMPLES} COMPONENT examples_qt
#     BUNDLE  DESTINATION ${INSTALL_EXAMPLES} COMPONENT examples_qt
# )
