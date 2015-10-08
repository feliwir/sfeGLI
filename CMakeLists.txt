cmake_minimum_required (VERSION 2.8.12)

project(sfeGLI)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

set (SFEGLI_LIB sfeGLI)
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)

set (SOURCES_DIR "src")
set (PUBLIC_HEADERS "include")
set (PRIVATE_HEADERS "src")

#################################################################################################################
# ================================================ SFML SETUP ================================================= #
#################################################################################################################
if (${CMAKE_HOST_WIN32})
    set (SFEGLI_BUILD_STATIC FALSE CACHE BOOL "TRUE to build static version of sfeGLI")
endif()

if (SFEGLI_BUILD_STATIC)
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

find_package (SFML 2.3 COMPONENTS graphics system REQUIRED)
message(STATUS "SFML libraries: ${SFML_LIBRARIES}")
message(STATUS "Additional dependencies: ${SFML_DEPENDENCIES}")

#################################################################################################################
# ================================================ GLI SETUP ================================================== #
#################################################################################################################
find_package (GLI REQUIRED)

#################################################################################################################
# ================================================ GLM SETUP ================================================== #
#################################################################################################################
find_package (GLM REQUIRED)

#################################################################################################################
# =============================================== sfeGLI SETUP ================================================ #
#################################################################################################################
file(GLOB SOURCE_FILES ${SOURCES_DIR}/[a-z,A-Z]*.[c,h]*)
file(GLOB HEADER_FILES RELATIVE ${PROJECT_SOURCE_DIR} "include/sfeGLI/[a-z,A-Z]*.[h]*")

add_definitions(-D__STDC_CONSTANT_MACROS -D__STDC_LIMIT_MACROS -DSFE_EXPORTS)

set(SFEGLI_INCLUDE_DIRS "${SFML_INCLUDE_DIR};${GLI_INCLUDE_DIR};${GLM_INCLUDE_DIRS};${PUBLIC_HEADERS};${PRIVATE_HEADERS}")
include_directories (${SFEGLI_INCLUDE_DIRS})
message (STATUS "Headers directories: ${SFML_INCLUDE_DIR} ${PUBLIC_HEADERS} ${PRIVATE_HEADERS}")

source_group("Public headers" FILES ${HEADER_FILES})
if (SFEGLI_BUILD_STATIC)
    add_library(${SFEGLI_LIB} STATIC ${SOURCE_FILES} ${HEADER_FILES})
else()
    add_library(${SFEGLI_LIB} SHARED ${SOURCE_FILES} ${HEADER_FILES})
endif()


if (SFEGLI_BUILD_STATIC)
    set_target_properties(${SFEGLI_LIB} PROPERTIES
        COMPILE_DEFINITIONS SFEGLI_STATIC
        COMPILE_DEFINITIONS SFML_STATIC)
    message(WARNING "You chose to build a static library of sfeGLI, this assumes that you will also statically link your software against SFML."
            "sfeGLI will be built with SFML_STATIC defined.")
else()
    message(STATUS "Link sfeGLI against: ${SFML_LIBRARIES} ${OTHER_LIBRARIES} ${SFML_DEPENDENCIES}")
    target_link_libraries(${SFEGLI_LIB} PRIVATE ${SFML_LIBRARIES} ${OTHER_LIBRARIES} ${SFML_DEPENDENCIES})
endif()

add_subdirectory(examples)

set(SFEGLI_BUILD_DOC FALSE CACHE BOOL "Set to true to build the documentation, requires Doxygen")
if(SFEGLI_BUILD_DOC)
    add_subdirectory(doxygen)
endif()

include(${CMAKE_SOURCE_DIR}/cmake/Packaging.cmake)