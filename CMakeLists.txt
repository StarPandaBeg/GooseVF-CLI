cmake_minimum_required(VERSION 3.16)
project(GooseVFCLI LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

file(GLOB_RECURSE source_files
   "src/*.cpp"
)
add_executable(GooseVFCLI src/main.cpp ${source_files})

target_compile_features(GooseVFCLI PUBLIC cxx_std_17)
set_target_properties(GooseVFCLI PROPERTIES OUTPUT_NAME "goosevf-cli")

set_target_properties(GooseVFCLI PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
set_target_properties(GooseVFCLI PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(GooseVF
    GIT_REPOSITORY https://github.com/StarPandaBeg/GooseVF.git
    GIT_TAG main)
FetchContent_MakeAvailable(GooseVF)

target_include_directories(GooseVFCLI PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${GooseVF_SOURCE_DIR}/include
)
target_link_libraries(GooseVFCLI PRIVATE GooseVF)