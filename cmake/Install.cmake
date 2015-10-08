# define the install directory for miscellaneous files
if(WINDOWS)
    set(INSTALL_MISC_DIR . )
else()
    set(INSTALL_MISC_DIR "share/${SFEGLI_LIB}")
endif()

# Install sfeGLI library and headers
if (LINUX OR WINDOWS)
	install(DIRECTORY include
			DESTINATION .)

	install(TARGETS ${SFEGLI_LIB}
        RUNTIME DESTINATION bin COMPONENT binaries
        LIBRARY DESTINATION lib${LIB_SUFFIX} COMPONENT binaries
        ARCHIVE DESTINATION lib${LIB_SUFFIX} COMPONENT libraries)
else()
	install(TARGETS ${SFEGLI_LIB}
            FRAMEWORK
            DESTINATION ${CMAKE_INSTALL_FRAMEWORK_PREFIX}
            COMPONENT binaries)
endif()

install(FILES "${CMAKE_SOURCE_DIR}/License.txt" "${CMAKE_SOURCE_DIR}/ReadMe.txt" "${CMAKE_SOURCE_DIR}/Authors.txt"
        DESTINATION ${INSTALL_MISC_DIR}
        COMPONENT resources)