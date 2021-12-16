#include "glew/2.2.0_1/include/GL/glew.h"
#include "glfw/3.3.4/include/GLFW/glfw3.h"
#include <iostream>
#include "model.h"
using namespace std;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    std::cout<<"ESC"<<mode;
}

double sx=0, sy=0;

void mouse_click(GLFWwindow* window, int button, int action, int mods) {
    cout<<button<<","<<action<<","<<mods<<endl;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    sx = xpos/300-1;
    sy = -(ypos/300-1);
    cout<<xpos/300-1<<","<<ypos/300-1<<endl;
    cout<<xpos<<","<<ypos<<endl;
}


int main(void) {
    //初始化GLFW库
    if (!glfwInit())
        return -1;
    //创建窗口以及上下文
    GLFWwindow *window = glfwCreateWindow(600, 600, "hello world", NULL, NULL);
    if (!window) {
        //创建失败会返回NULL
        glfwTerminate();
    }

    //建立当前窗口的上下文
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback); //注册回调函数
    glfwSetMouseButtonCallback(window, mouse_click);
    //glViewport(0, 0, 400, 400);
    //gluOrtho2D(-200, 200.0, -200, 200.0);
    //循环，直到用户关闭窗口
    cout<<123<<endl;
    char *path = "/Users/bytedance/CLionProjects/glTriangle/BR_Squirtle.txt";
    ObjectModel om(path);

    while (!glfwWindowShouldClose(window)) {
        /*******轮询事件*******/
        glfwPollEvents();
        // cout<<456<<endl;
        //选择清空的颜色RGBA
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        // glColor3f(0,0, 0);
        glMatrixMode(GL_PROJECTION);
        om.objDraw();
        glColor3f(1, 0, 0); //Red
        glPointSize(10);
        glBegin(GL_POINTS);

        glVertex3d(-0.7, -0.7, 0);
        glVertex3d(sx, sy, 0);
        glEnd();
        glFlush();


/*
        //开始画一个三角形
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); //Red
        glVertex3f(0.5, 0, 0.3);

        glVertex3f(-0.5, -0.5, 0);

        glVertex3f(0.5, -0.5, 0);
        //结束一个画图步骤
        glEnd();
        */

/*
        int point1[] = {50, 100};
        glLineWidth(4.0f);
        glBegin(GL_LINE_STRIP);
        glColor3f(100,100, 100);
        glVertex2i(10, 10);

        glColor3f(100,0, 0);
        glVertex2i(20, 20);

        glEnd();

        glBegin(GL_POINTS);
        glVertex2i(100,100);
        glEnd();
*/


        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
