cmake_minimum_required(VERSION 3.5)

project(AngleCheckGUI LANGUAGES CXX)

find_package(wxWidgets COMPONENTS core base REQUIRED)

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
include( "${wxWidgets_USE_FILE}" )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(AngleCheckGUI cApp.h cApp.cpp cMain.h cMain.cpp imageanalyzer.h imageanalyzer.cpp basicDrawPlane.h basicDrawPlane.cpp renderTimer.h renderTimer.cpp)
target_link_libraries(AngleCheckGUI ${wxWidgets_LIBRARIES} ${OpenCV_LIBS})

