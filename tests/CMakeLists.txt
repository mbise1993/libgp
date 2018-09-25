set(TARGET_NAME tests)

# Collect files
file(GLOB_RECURSE headerFiles *.h)
file(GLOB_RECURSE sourceFiles *.cpp)

add_executable(${TARGET_NAME} ${headerFiles} ${sourceFiles})

target_link_libraries(${TARGET_NAME}
	PUBLIC
		libgp
		CONAN_PKG::catch2)

add_test(NAME ${TARGET_NAME} COMMAND ${TARGET_NAME} -o report.xml -r junit)

# Copy resources to build folder
file(COPY resources DESTINATION ${CMAKE_BINARY_DIR}/bin)