cmake_minimum_required (VERSION 2.8.6)
project (BongardGenerator) 

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

# Check for 64-bit build flags.
set(CMAKE_REQUIRED_LIBRARIES "-m64")
CHECK_C_COMPILER_FLAG("-m64" C_COMPILER_HAS_M64)
CHECK_CXX_COMPILER_FLAG("-m64" CXX_COMPILER_HAS_M64)
unset(CMAKE_REQUIRED_LIBRARIES)

if(C_COMPILER_HAS_M64 AND CXX_COMPILER_HAS_M64)
  set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS} -m64")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -m64")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64")
  set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -m64")
  set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m64")
endif()

# Check for C++11 support.
CHECK_CXX_COMPILER_FLAG("-std=c++11" CXX_COMPILER_SUPPORTS_CPP11)
if (CXX_COMPILER_SUPPORTS_CPP11)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
  message(FATAL_ERROR "The compiler does not support C++11")
endif()

find_package(Boost 1.50.0 COMPONENTS system filesystem REQUIRED)
message("-- Boost Include: ${Boost_INCLUDE_DIRS}")
message("-- Boost Libraries: ${Boost_LIBRARY_DIRS}")

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(bongard_generator BongardGenerator.cpp BongardPicture.cpp Main.cpp)
target_link_libraries(bongard_generator ${Boost_LIBRARIES})
