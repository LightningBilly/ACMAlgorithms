#include "glew/2.2.0_1/include/GL/glew.h"
#include "glfw/3.3.4/include/GLFW/glfw3.h"
#include <iostream>
#include <cmath>

using namespace std;

#define PI acos(-1)


GLsizei winWidth = 600, winHeight = 500;
GLint xRaster = 25, yRaster = 150;
GLubyte label [36] = {'J','a','n',   'F','e','b',   'M','a','r',
                      'A','p','r',   'M','a','y',   'J','u','n',
                      'J','u','l',   'A','u','g',   'S','e','p',
                      'O','c','t',   'N','o','v',   'D','e','c'
                      };
GLint dataValue [12] = {420, 342, 324, 310, 262, 185,
                        190, 196, 217, 240, 312, 438};


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    std::cout<<"ESC"<<mode;
}



void init() {
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 600, 0, 500);
}

void lineGraph(){
    GLint month, k;
    GLint x = 30;

    glColor3f(0.0, 1.0, 1.0);
    glLineWidth(10);
    glBegin(GL_LINE_STRIP);
    for (int k = 0; k < 12; ++k) {
        glVertex2i(x+k*50, dataValue[k]);
    }
    glEnd();

    glColor3f(1,0,0);
    for (int k = 0; k < 12; ++k) {
        glRasterPos2i(xRaster+k*50, dataValue[k]-4);
        
    }
}


void barChart(){
    GLint month, k;
    GLint x = 30;

    glColor3f(1, 0.0, 0.0);
    for (int k = 0; k < 12; ++k) {
        glRecti(20+k*50,165,40+k*50, dataValue[k]);
    }
}

void winReshapeFcn(GLint newWidth, GLint newHeight) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, GLdouble(newWidth), 0.0, GLdouble(newHeight));
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(void) {
    //初始化GLFW库
    if (!glfwInit())
        return -1;
    //创建窗口以及上下文
    GLFWwindow *window = glfwCreateWindow(winWidth, winHeight, "hello world", NULL, NULL);
    if (!window) {
        //创建失败会返回NULL
        glfwTerminate();
    }

    //建立当前窗口的上下文
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback); //注册回调函数
    //glViewport(0, 0, 400, 400);
    gluOrtho2D(0, winWidth,0, winHeight);

    //循环，直到用户关闭窗口
    cout<<123<<endl;
    while (!glfwWindowShouldClose(window)) {
        /*******轮询事件*******/
        glfwPollEvents();
        // cout<<456<<endl;
        //选择清空的颜色RGBA
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        // glColor3f(0,0, 0);
        glMatrixMode(GL_PROJECTION);

        lineGraph();
        barChart();

        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return 0;
}
