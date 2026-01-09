cmake_minimum_required(VERSION 3.5)

project(LVC_Common LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB_RECURSE LibFiles "include/*.h")
add_custom_target(_LVC_Common_include SOURCES ${LibFiles})

file(GLOB_RECURSE Sources "source/*.cpp")
add_library(LVC_Common STATIC ${Sources} )

target_include_directories(LVC_Common PUBLIC
                            "${PROJECT_BINARY_DIR}" 
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/../L_Utility/include"
                            "${PROJECT_SOURCE_DIR}/../L_Network/include"
)
