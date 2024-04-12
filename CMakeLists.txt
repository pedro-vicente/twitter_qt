cmake_minimum_required(VERSION 3.16)
project(twitter_qt VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

#//////////////////////////
# list of source files 
#//////////////////////////

set(src ${src})
set(src ${src} main.cc)
set(src ${src} navigation.cc)
set(src ${src} navigation.h)
set(src ${src} twitter.h)

add_executable(navigation ${src})
target_link_libraries(navigation PRIVATE Qt6::Widgets)
set_target_properties(navigation PROPERTIES WIN32_EXECUTABLE ON MACOSX_BUNDLE ON)

add_executable(twitter twitter.h twitter.cc)


