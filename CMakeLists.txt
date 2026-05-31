cmake_minimum_required(VERSION 3.5...4.3.2)
project(Raytracer LANGUAGES CXX)

# === GLOBAL CONFIGURATION ===
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
find_package(PkgConfig QUIET)

# === INTERFACE TARGET (Common includes and flags) ===
add_library(raytracerGlobal INTERFACE)

target_compile_options(raytracerGlobal INTERFACE
    -Wall -Wextra -Werror -Wpedantic -Wold-style-cast -O3 -ffast-math
)

target_include_directories(raytracerGlobal INTERFACE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Math
    ${PROJECT_SOURCE_DIR}/include/Core
    ${PROJECT_SOURCE_DIR}/include/Materials
    ${PROJECT_SOURCE_DIR}/include/Primitives
    ${PROJECT_SOURCE_DIR}/include/Light
    ${PROJECT_SOURCE_DIR}/include/Exceptions
    ${PROJECT_SOURCE_DIR}/include/Factory
    ${PROJECT_SOURCE_DIR}/include/Loader
)

# === DEPENDENCIES: OpenMP ===
# Help CMake find Homebrew's libomp on macOS
if(APPLE)
    if(EXISTS "/opt/homebrew/opt/libomp")
        set(OpenMP_ROOT /opt/homebrew/opt/libomp)
    elseif(EXISTS "/usr/local/opt/libomp")
        set(OpenMP_ROOT /usr/local/opt/libomp)
    endif()
endif()

find_package(OpenMP QUIET)

if(NOT OpenMP_CXX_FOUND)
    message(STATUS "OpenMP not found, fetching from GitHub...")

    fetchcontent_declare(openmp
        GIT_REPOSITORY https://github.com/llvm/llvm-project.git
        GIT_TAG llvmorg-18.1.2
        SOURCE_SUBDIR openmp
        GIT_SHALLOW TRUE
    )

    set(LIBOMP_ENABLE_SHARED ON CACHE BOOL "" FORCE)
    set(LIBOMP_INSTALL_ALIASES OFF CACHE BOOL "" FORCE)
    fetchcontent_makeavailable(openmp)

    target_link_libraries(raytracerGlobal INTERFACE omp)
else()
    message(STATUS "OpenMP found")
    target_link_libraries(raytracerGlobal INTERFACE OpenMP::OpenMP_CXX)
endif()

# === DEPENDENCIES: LIBCONFIG++ ===
if(PKG_CONFIG_FOUND)
    pkg_check_modules(LIBCONFIGXX QUIET IMPORTED_TARGET libconfig++)
endif()

if(NOT LIBCONFIGXX_FOUND)
    message(STATUS "libconfig++ not found, fetching from GitHub...")

    set(CMAKE_POLICY_VERSION_MINIMUM "3.5" CACHE STRING "Minimum policy version" FORCE)

    set(BUILD_EXAMPLES OFF CACHE BOOL "Disable libconfig examples" FORCE)
    set(BUILD_TESTS OFF CACHE BOOL "Disable libconfig tests" FORCE)

    fetchcontent_declare(libconfig
        GIT_REPOSITORY https://github.com/hyperrealm/libconfig.git
        GIT_TAG v1.7.3
        GIT_SHALLOW TRUE
    )

    set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
    fetchcontent_makeavailable(libconfig)
    set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)

    include(CheckSymbolExists)
    check_symbol_exists(uselocale "locale.h" HAVE_USELOCALE)
    check_symbol_exists(newlocale "locale.h" HAVE_NEWLOCALE)
    check_symbol_exists(freelocale "locale.h" HAVE_FREELOCALE)

    foreach(tgt config config++)
        if(TARGET ${tgt})
            target_compile_options(${tgt} PRIVATE -w)
            if(HAVE_USELOCALE)
                target_compile_definitions(${tgt} PRIVATE HAVE_USELOCALE)
            endif()
            if(HAVE_NEWLOCALE)
                target_compile_definitions(${tgt} PRIVATE HAVE_NEWLOCALE)
            endif()
            if(HAVE_FREELOCALE)
                target_compile_definitions(${tgt} PRIVATE HAVE_FREELOCALE)
            endif()
        endif()
    endforeach()
else()
    message(STATUS "libconfig++ found via PkgConfig: ${LIBCONFIGXX_VERSION}")
endif()

# Add libconfig++ to the global interface target
if(TARGET PkgConfig::LIBCONFIGXX)
    target_link_libraries(raytracerGlobal INTERFACE PkgConfig::LIBCONFIGXX)
else()
    # Fallback pour la version FetchContent
    target_link_libraries(raytracerGlobal INTERFACE config++)
    target_include_directories(raytracerGlobal INTERFACE ${libconfig_SOURCE_DIR}/lib)
endif()

# === DEPENDENCIES: SFML ===
if(PKG_CONFIG_FOUND)
    pkg_check_modules(SFML QUIET IMPORTED_TARGET sfml-graphics)
endif()

if(NOT SFML_FOUND)
    message(STATUS "SFML not found, fetching from GitHub...")

    set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_TEST_SUITE OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_AUDIO OFF CACHE BOOL "" FORCE)
    set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)

    fetchcontent_declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
        GIT_SHALLOW TRUE
    )

    fetchcontent_makeavailable(SFML)

    if(APPLE AND TARGET sfml-window)
        target_compile_options(sfml-window PRIVATE -Wno-incompatible-pointer-types)
    endif()
else()
    message(STATUS "SFML found via PkgConfig: ${SFML_VERSION}")
endif()

# Add SFML to the global interface target
if(TARGET PkgConfig::SFML)
    target_link_libraries(raytracerGlobal INTERFACE PkgConfig::SFML)
else()
    # SFML target from FetchContent
    target_link_libraries(raytracerGlobal INTERFACE sfml-graphics)
endif()

# === PROJECT CORE ===
file(GLOB_RECURSE CORE_FILES CONFIGURE_DEPENDS "source/*.cpp")
list(FILTER CORE_FILES EXCLUDE REGEX "source/Primitives/.*\\.cpp$")
list(APPEND CORE_FILES source/Primitives/APrimitive.cpp)
list(FILTER CORE_FILES EXCLUDE REGEX "source/Light/.*\\.cpp$")
list(FILTER CORE_FILES EXCLUDE REGEX "source/Materials/.*\\.cpp$")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

add_executable(raytracer ${CORE_FILES})
target_link_libraries(raytracer PRIVATE raytracerGlobal)
set_target_properties(raytracer PROPERTIES ENABLE_EXPORTS TRUE)

# === AFRACTAL STATIC LIB ===
add_library(afractal_static STATIC
    source/Primitives/AFractal.cpp
)
target_link_libraries(afractal_static PUBLIC raytracerGlobal)

# === PLUGINS ===
set(PLUGIN_OUTPUT_DIR ${PROJECT_SOURCE_DIR}/plugins)

function(add_raytracer_plugin target_name source_file)
    add_library(${target_name} MODULE ${source_file} ${ARGN})
    target_link_libraries(${target_name} PRIVATE raytracerGlobal)

    set_target_properties(${target_name} PROPERTIES
        PREFIX ""
        LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
    )

    if(APPLE)
        set_target_properties(${target_name} PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
    endif()
endfunction()

add_raytracer_plugin(raytracer_spheres source/Primitives/Sphere.cpp)
add_raytracer_plugin(raytracer_planes source/Primitives/Plane.cpp)
add_raytracer_plugin(raytracer_triangles source/Primitives/Triangle.cpp)
add_raytracer_plugin(raytracer_boxes source/Primitives/Box.cpp)
add_raytracer_plugin(raytracer_cylinders source/Primitives/Cylinder.cpp)
add_raytracer_plugin(raytracer_cones source/Primitives/Cone.cpp)
add_raytracer_plugin(raytracer_objs source/Primitives/Obj.cpp)
add_raytracer_plugin(raytracer_torus source/Primitives/Torus.cpp)
add_raytracer_plugin(raytracer_ambient source/Light/AmbientLight.cpp source/Light/ALight.cpp)
add_raytracer_plugin(raytracer_directionals source/Light/DirectionalLight.cpp source/Light/ALight.cpp)
add_raytracer_plugin(raytracer_pointlight source/Light/PointLight.cpp source/Light/ALight.cpp)
add_raytracer_plugin(raytracer_lambertians source/Materials/Lambertian.cpp)
add_raytracer_plugin(raytracer_phongs source/Materials/Phong.cpp)
add_raytracer_plugin(raytracer_textures source/Materials/Texture.cpp)
add_raytracer_plugin(raytracer_refractives source/Materials/Refractive.cpp)
add_raytracer_plugin(raytracer_reflectives source/Materials/Reflective.cpp)

add_raytracer_plugin(raytracer_mandelbulbs source/Primitives/Mandelbulb.cpp)
target_link_libraries(raytracer_mandelbulbs PRIVATE afractal_static)
add_raytracer_plugin(raytracer_sierpinskis source/Primitives/Sierpinski.cpp)
target_link_libraries(raytracer_sierpinskis PRIVATE afractal_static)
add_raytracer_plugin(raytracer_mengersponges source/Primitives/MengerSponge.cpp)
target_link_libraries(raytracer_mengersponges PRIVATE afractal_static)
add_raytracer_plugin(raytracer_mobiusstrips source/Primitives/MobiusStrip.cpp)
target_link_libraries(raytracer_mobiusstrips PRIVATE afractal_static)
add_raytracer_plugin(raytracer_tanglecubes source/Primitives/TangleCube.cpp)
target_link_libraries(raytracer_tanglecubes PRIVATE afractal_static)
