#include "glew/2.2.0_1/include/GL/glew.h"
#include "glfw/3.3.4/include/GLFW/glfw3.h"
#include <iostream>
#include <cmath>
using namespace std;

#define PI acos(-1)

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

/*
 * 使用圆的参数方程画圆
 * x = x0+r*cos(∂)
 * y = y0+r*sin(∂)
 * 需要调用三角函数，非常耗时
 */
void CircleSimple(Point p1, double r ) {
    for(double i= 0.0;i<360;i+=0.01) {
        setPixel(Point(p1.x+r*cos(i/180*PI), p1.y+r*sin(i/180*PI)));
    }

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

        cout<<pp<<"-"<<start.x<<"-"<<start.y<<endl;

        setCirclePixel(start, center);
    }

}



int main(void) {
    cout<<acos(-1)<<endl;
    // return 0;
    //初始化GLFW库
    if (!glfwInit())
        return -1;
    //创建窗口以及上下文
    GLFWwindow *window = glfwCreateWindow(400, 400, "hello world", NULL, NULL);
    if (!window) {
        //创建失败会返回NULL
        glfwTerminate();
    }

    //建立当前窗口的上下文
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback); //注册回调函数
    //glViewport(0, 0, 400, 400);
    gluOrtho2D(-200, 200.0, -200, 200.0);
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

        // CircleSimple(Point(0,0), 100);

        //circleMidPoint(Point(0,0), 50);
        circleMidPoint(Point(0,10), 50);



        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);
        //break;
    }

    glfwTerminate();
    return 0;
}


/*
        //开始画一个三角形
        glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); //Red
        glVertex3f(0.5, 0, 0);

        glVertex3f(-1, -1, 0);

        glVertex3f(1, -1, 0);
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
//LineDDA(Point(0,0),Point(60,100));
//LineDDA(Point(0,0),Point(100,60));
