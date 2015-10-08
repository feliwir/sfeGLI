set(SFEGLI_DEMO "Demo")

set(SFEGLI_DEMO_SRC
    main.cpp
)
source_group("Sources" FILES ${SFEGLI_DEMO_SRC})

add_executable(
    ${SFEGLI_DEMO}
    ${SFEGLI_DEMO_SRC}
)

if (SFEGLI_BUILD_STATIC)
    set(SFML_STATIC_LIBRARIES TRUE)
    find_package (SFML 2.3 COMPONENTS graphics window system REQUIRED)
    message(STATUS "SFML libraries: ${SFML_LIBRARIES}")
    message(STATUS "Additional dependencies: ${SFML_DEPENDENCIES}")

    set_target_properties(${SFEGLI_DEMO} PROPERTIES
        COMPILE_DEFINITIONS SFML_STATIC
        COMPILE_DEFINITIONS SFEGLI_STATIC)
    target_link_libraries(
        ${SFEGLI_DEMO}
        ${SFEGLI_LIB}
        ${SFML_LIBRARIES}
        ${SFML_DEPENDENCIES})
else()
    target_link_libraries(
        ${SFEGLI_DEMO}
        ${SFEGLI_LIB}
        ${SFML_LIBRARIES}
        ${SFML_DEPENDENCIES}
    )
endif()

if (MACOSX)
    set_target_properties(${SFEGLI_DEMO} PROPERTIES 
                          BUILD_WITH_INSTALL_RPATH 1 
                          INSTALL_RPATH "@executable_path/")
endif()