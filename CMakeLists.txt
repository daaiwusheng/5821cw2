cmake_minimum_required(VERSION 2.8)
project(5821)

set(CMAKE_CXX_STANDARD 11)

# 添加头文件
set(GLEW_H /usr/local/Cellar/glew/2.2.0_1/include/GL)
set(GLFW_H /usr/local/Cellar/glfw/3.3.4/include/GLFW)
set(GLAD_H /usr/local/include/)
set(GLAD_H /usr/local/include/glad)
set(KH_H /usr/local/include/KHR)
#包含上述头文件所在目录
include_directories(${GLEW_H} ${GLFW_H} ${GLAD_H} ${KH_H})

# 添加目标链接
set(GLEW_LINK /usr/local/Cellar/glew/2.2.0_1/lib/libGLEW.2.2.0.dylib)
set(GLFW_LINK /usr/local/Cellar/glfw/3.3.4/lib/libglfw.3.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/usr/local/Cellar/qt@5/5.15.2")


add_executable(5821 main.cpp
        AttributedObject.h
        AttributedObject.cpp
        ArcBall.h
        ArcBall.cpp
        ArcBallWidget.h
        ArcBallWidget.cpp
        Cartesian3.h
        Cartesian3.cpp
        Homogeneous4.h
        Homogeneous4.cpp
        Matrix4.h
        Matrix4.cpp
        Quaternion.h
        Quaternion.cpp
        RenderController.h
        RenderController.cpp
        RenderWidget.h
        RenderWidget.cpp
        RenderWindow.h
        RenderWindow.cpp
        src/glad.c)


find_package(Qt5 COMPONENTS
        Core
        REQUIRED)

find_package(Qt5Widgets REQUIRED)

# mac下这步很重要
if (APPLE)
    target_link_libraries(5821 "-framework OpenGL")
    target_link_libraries(5821 "-framework GLUT")
endif()

target_link_libraries(5821
        Qt5::Core
        )
target_link_libraries(5821
        Qt5::Widgets
        )

target_link_libraries(5821
        Qt5::Gui
        )