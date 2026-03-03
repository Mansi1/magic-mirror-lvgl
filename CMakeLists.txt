cmake_minimum_required(VERSION 3.10)
project(MagicMirror)

# Force the DRM driver to be active
add_definitions(-DUSE_DRM=1)

# Fix for the "drm_mode.h" not found error
include_directories(/usr/include/libdrm)

# THE ROOMS THE COMPILER LOOKS IN
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/eez_src/src)  # <--- THIS IS THE MISSING LINK
include_directories(${PROJECT_SOURCE_DIR}/lvgl)
include_directories(${PROJECT_SOURCE_DIR}/lv_drivers)
include_directories(${PROJECT_SOURCE_DIR}/lv_drivers/display)

# THE FILES TO COMPILE
file(GLOB_RECURSE SOURCES
    "${PROJECT_SOURCE_DIR}/src/main.c"
    "${PROJECT_SOURCE_DIR}/eez_src/src/*.c"
    "${PROJECT_SOURCE_DIR}/lvgl/src/*.c"
    "${PROJECT_SOURCE_DIR}/lv_drivers/*.c"
)

add_executable(mirror_app ${SOURCES})

# THE LIBRARIES TO LINK
target_link_libraries(mirror_app drm gbm input udev m)
