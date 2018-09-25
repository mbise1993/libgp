set(TARGET_NAME libgp)

add_definitions(-DLIBGP_LIB)

# Compile options
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "MSVC")
	#set(compileOptions /wd4251 /wd4275 /wd4996)
endif()

# Add include directories
include_directories(${CMAKE_CURRENT_LIST_DIR})
include_directories(${PROJECT_BINARY_DIR})

# Collect files
file(GLOB_RECURSE headerFiles *.h)
file(GLOB_RECURSE sourceFiles *.cpp)

add_library(${TARGET_NAME} STATIC ${headerFiles} ${sourceFiles})

target_compile_options(${TARGET_NAME} PUBLIC ${compileOptions})

# Enable cotire
if(COMMAND cotire)
    set_target_properties(${TARGET_NAME} PROPERTIES COTIRE_ADD_UNITY_BUILD FALSE)
    cotire(${TARGET_NAME})
endif()