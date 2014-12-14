cmake_minimum_required (VERSION 2.8.6)
project (BongardGenerator) 

include(CheckCCompilerFlag)
include(CheckCXXCompilerFlag)

# The following is taken from CMakeLists.txt in the QuickStep project.
# Enable C++11 support if possible.
set(CPP11_SUPPORTED FALSE)
include(CheckCXXSourceCompiles)
if(NOT CPP11_SUPPORTED)
  if (MSVC_VERSION GREATER 1599)
    message("-- Your C++ compiler appears to be Microsoft Visual Studio 10 or better, which supports C++11 by default.")
    set(CPP11_SUPPORTED TRUE)
  endif()
endif()
if (NOT CPP11_SUPPORTED)
  CHECK_CXX_COMPILER_FLAG("-std=c++11" CXX_COMPILER_SUPPORTS_CPP11)
  if (CXX_COMPILER_SUPPORTS_CPP11)
    set(ORIGINAL_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    set(ORIGINAL_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS_CXX} -std=c++11")
    # This test program makes sure that the c++ compiler and standard library play nice together
    # with rvalue references.
    CHECK_CXX_SOURCE_COMPILES("
      #include <utility>
      #include <vector>
      #include <string>
      int main() {
        std::string str(\"txt\");
        std::vector<std::string> vec;
        vec.push_back(std::move(str));
        return 0;
      }
      " CPP11_COMPILES)
    if (CPP11_COMPILES)
      set(CPP11_SUPPORTED TRUE)
      message("Your C++ compiler appears to support C++11, setting flags to enable it.")
    else()
      set(CMAKE_CXX_FLAGS "${ORIGINAL_CXX_FLAGS}")
      set(CMAKE_REQUIRED_FLAGS "${ORIGINAL_REQUIRED_FLAGS}")
      message(WARNING "Your compiler has flags that enable C++11 support, but your C++ standard "
                      "library does not appear to support rvalue references. C++11 support will "
                      "be disabled.")
    endif()
  endif()
endif()
if(NOT CPP11_SUPPORTED)
  CHECK_CXX_COMPILER_FLAG("-std=c++0x" CXX_COMPILER_SUPPORTS_CPP0X)
  if (CXX_COMPILER_SUPPORTS_CPP0X)
    set(ORIGINAL_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    set(ORIGINAL_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS}")
    set(CMAKE_REQUIRED_FLAGS "${CMAKE_REQUIRED_FLAGS_CXX} -std=c++0x")
    # This test program makes sure that the c++ compiler and standard library play nice together
    # with rvalue references.
    CHECK_CXX_SOURCE_COMPILES("
      #include <utility>
      #include <vector>
      #include <string>
      int main() {
        std::string str(\"txt\");
        std::vector<std::string> vec;
        vec.push_back(std::move(str));
        return 0;
      }
      " CPP0X_COMPILES)
    if (CPP0X_COMPILES)
      set(CPP11_SUPPORTED TRUE)
      message(WARNING "Your C++ compiler appears to support pre-standardized C++11 (AKA C++0X). It "
                      "will be enabled, but you should consider upgrading to a more recent C++ "
                      "compiler that fully supports standard C++11.")
    else()
      set(CMAKE_CXX_FLAGS "${ORIGINAL_CXX_FLAGS}")
      set(CMAKE_REQUIRED_FLAGS "${ORIGINAL_REQUIRED_FLAGS}")
      message(WARNING "Your compiler has flags that enable pre-standardized C++11 (AKA C++0X) "
                      "support, but your C++ standard library does not appear to support rvalue "
                      "references. C++11 support will be disabled.")
    endif()
  endif()
endif()
if (NOT CPP11_SUPPORTED)
  message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif()

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.55.0 COMPONENTS system filesystem REQUIRED)
message("-- Boost Include: ${Boost_INCLUDE_DIRS}")
message("-- Boost Libraries: ${Boost_LIBRARY_DIRS}")

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(bongard_generator BongardGenerator.cpp BongardPicture.cpp Main.cpp)
target_link_libraries(bongard_generator ${Boost_LIBRARIES})
