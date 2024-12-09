cmake_minimum_required(VERSION 3.16)

project(depthai_test LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# Tìm kiếm thư viện OpenCV
find_package(OpenCV REQUIRED)

# Tìm kiếm thư viện DepthAI (Cập nhật đường dẫn cài đặt nếu cần thiết)
find_package(depthai CONFIG REQUIRED PATHS /home/phulinh/depthai-core/build/install/lib/cmake/depthai)


add_executable(depthai_test
  main.cpp
  PipelineManager.h PipelineManager.cpp
  LogicManager.h LogicManager.cpp
  DisplayManager.h DisplayManager.cpp
)
# Liên kết các thư viện cần thiết
target_link_libraries(${PROJECT_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Core
  PRIVATE ${OpenCV_LIBS}
  PRIVATE depthai::opencv
)

include(GNUInstallDirs)
install(TARGETS depthai_test
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
