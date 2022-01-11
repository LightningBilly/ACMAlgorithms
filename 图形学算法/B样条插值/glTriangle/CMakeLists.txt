cmake_minimum_required(VERSION 3.15)
project(glTriangle)

set(CMAKE_CXX_STANDARD 14)

#设置一些OpenGL头文件所在目录的宏
set(GLEW_H /Library/Developer/CommandLineTools/usr/include/glew/2.2.0_1/include/GL)
set(GLFW_H /Library/Developer/CommandLineTools/usr/include/glfw/3.3.4/include/GLFW)
set(GLAD_H /Library/Developer/CommandLineTools/usr/include)
set(GLAD_H /Library/Developer/CommandLineTools/usr/include/glad)
set(KH_H /Library/Developer/CommandLineTools/usr/include/KHR)
#包含上述头文件所在目录
include_directories(${GLEW_H} ${GLFW_H} ${GLAD_H} ${KH_H})
include_directories( "/usr/local/include/eigen3" )

#设置OpenGL库所在位置，并链接
set(GLEW_LINK /Library/Developer/CommandLineTools/usr/include/glew/2.2.0_1/lib/libGLEW.2.2.0.dylib)
set(GLFW_LINK /Library/Developer/CommandLineTools/usr/include/glfw/3.3.4/lib/libglfw.3.3.dylib)
link_libraries(${OPENGL} ${GLEW_LINK} ${GLFW_LINK})

#设置要编译的c/cpp文件到可执行文件中，该exe包含两个文件glad.c,main.cpp,glTriangle
set(SOURCE_FILES "glad.c" "main.cpp")

add_executable(glTriangle main.cpp model.cpp model.h BSpline.h BSpline.cpp)


# mac下这步很重要
if (APPLE)
    target_link_libraries(glTriangle "-framework OpenGL")
    target_link_libraries(glTriangle "-framework GLUT")
endif()