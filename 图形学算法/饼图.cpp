#include "glew/2.2.0_1/include/GL/glew.h"
#include "glfw/3.3.4/include/GLFW/glfw3.h"
#include <iostream>
#include <cmath>

using namespace std;

#define PI acos(-1)


GLsizei winWidth = 600, winHeight = 500;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    std::cout<<"ESC"<<mode;
}



class Point {
public:
    int x, y;
    Point(int xx, int yy):x(xx), y(yy){}
};

void setPixel(Point p) {
    // cout<<p.x<<","<<p.y<<endl;
    glBegin(GL_POINTS);
    glVertex2i(p.x, p.y);
    glEnd();
    glFlush();
}

void setCirclePixel(Point p, Point center) {

    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if(i*j==0) continue;
            setPixel(Point(p.x*i+center.x, p.y*j+center.y));
            setPixel(Point(p.y*i+center.x, p.x*j+center.y));
        }

    }
}

/*
 * 中点法做圆
 * p0 = 5/4-4
 * p<0, x++ y不变，p += 2x+1
 * p>=0, x++,y++, p += 2x+1 - 2y;
 */

void circleMidPoint(Point center, int r) {
    int p = 1-r;

    Point start(0,r);
    setCirclePixel(start, center);

    while(start.x<=start.y) {
        start.x++;
        int pp = p;
        if(p<0) {
            p+=2*start.x+1;
        } else {
            start.y--;
            p+=2*start.x+1 -2*start.y;
        }

        // cout<<pp<<"-"<<start.x<<"-"<<start.y<<endl;

        setCirclePixel(start, center);
    }

}

void pieChart() {
    circleMidPoint(Point(winWidth/2,winHeight/2), winWidth/4);

    GLfloat values[12] =  {10,7,13,5,13,14,3,16,5,3,17,8};

    GLfloat dataSum = 0;
    for (int i = 0; i < 12; ++i) {
        dataSum+=values[i];
    }
    GLdouble preSum = 0;
    for (int j = 0; j < 12; ++j) {
        preSum += values[j];
        int x = winWidth/2 + winWidth/4*cos(2*PI*preSum/dataSum);
        int y = winHeight/2 + winWidth/4*sin(2*PI*preSum/dataSum);
        glBegin(GL_LINES);
        glVertex2i(winWidth/2, winHeight/2);
        glVertex2i(x,y);
        glEnd();
    }
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

        pieChart();

        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);
    }
    glfwTerminate();

    return 0;
}
