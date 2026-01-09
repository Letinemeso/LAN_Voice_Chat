cmake_minimum_required(VERSION 3.5)

project(LAN_Voice_Chat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DL_DEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -DL_DEBUG")

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

add_subdirectory("${PROJECT_SOURCE_DIR}/LVC_Common/"
                 "${PROJECT_BINARY_DIR}/LVC_Common/"
)

add_subdirectory("${PROJECT_SOURCE_DIR}/LVC_Server/"
                 "${PROJECT_BINARY_DIR}/LVC_Server/"
)
add_subdirectory("${PROJECT_SOURCE_DIR}/LVC_Client/"
                 "${PROJECT_BINARY_DIR}/LVC_Client/"
)
