cmake_minimum_required(VERSION 3.5)

project(LAN_Voice_Chat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FILE(GLOB_RECURSE LibFiles "include/*.h")
add_custom_target(_LAN_Voice_Chat_include SOURCES ${LibFiles})
FILE(GLOB_RECURSE Models "Resources/*")
add_custom_target(LAN_Voice_Chat_models SOURCES ${Models})

file(GLOB_RECURSE Sources "source/*.cpp")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DL_DEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DL_DEBUG")
# set(CMAKE_CXX_FLAGS "-DL_DEBUG")

if(MSVC)
    add_compile_options(/bigobj)
endif(MSVC)

add_executable(LAN_Voice_Chat ${Sources} Main.cpp)

add_subdirectory("${PROJECT_SOURCE_DIR}/L_Utility"
                 "${PROJECT_BINARY_DIR}/L_Utility"
)
add_subdirectory("${PROJECT_SOURCE_DIR}/L_Variable"
                 "${PROJECT_BINARY_DIR}/L_Variable"
)
add_subdirectory("${PROJECT_SOURCE_DIR}/L_Sound"
                 "${PROJECT_BINARY_DIR}/L_Sound"
)
add_subdirectory("${PROJECT_SOURCE_DIR}/L_Network"
                 "${PROJECT_BINARY_DIR}/L_Network"
)

target_include_directories(LAN_Voice_Chat PUBLIC
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/L_Utility/include"
                            "${PROJECT_SOURCE_DIR}/L_Variable/include"
                            "${PROJECT_SOURCE_DIR}/L_Sound/include"
                            "${PROJECT_SOURCE_DIR}/L_Network/include"
)


target_link_libraries(LAN_Voice_Chat PUBLIC
    L_Network
    L_Sound
    L_Variable
    L_Utility
)

add_library(OpenAL STATIC IMPORTED)

set_target_properties(OpenAL PROPERTIES IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/../Third_Party/OpenAL32.lib")

target_link_libraries(LAN_Voice_Chat PUBLIC
    OpenAL
)

if (WIN32)
    target_link_libraries(LAN_Voice_Chat PUBLIC
        user32
        gdi32
        shell32
    )
endif (WIN32)

if (UNIX)
    target_link_libraries(LAN_Voice_Chat PUBLIC
        pthread
        GL
        X11
        dl
    )
endif (UNIX)
