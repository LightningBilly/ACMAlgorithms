#include "glew/2.2.0_1/include/GL/glew.h"
#include "glfw/3.3.4/include/GLFW/glfw3.h"
#include <iostream>
using namespace std;


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

void LineDDA( Point p1, Point p2) {
    /*
    for(int i=0;i<100;i++ ) {
        setPixel(p1);
        p1.x++;
        p1.y++;
    }
     */

    float dx = p2.x-p1.x, dy = p2.y-p1.y;
    if (dx==0 && dy==0) {
        setPixel(p1);
        return;
    }

    float step = abs(dx);
    if(abs(dy)>abs(dx)) step = abs(dy);
    dx /= step;
    dy/=step;
    setPixel(p1);
    float x=p1.x, y=p1.y;
    for(int i=0; i<step;i++) {
        x += dx;
        y+=dy;
        setPixel(Point(x,y));
    }

}


/*
 * 简单Bresenham 算法
 * 只处理|m|>1
 */
void LineBres_2(Point p1, Point p2) {
    if(p2.y<p1.y) {
        Point t = p2;
        p2=p1;
        p1=t;
    }

    int deltaX = abs(p2.x-p1.x), deltaY = abs(p2.y-p1.y);
    int p0 = 2*deltaX-deltaY;
    int twDeltaX = 2*deltaX, twDeltaXmTwoDeltaY = 2*deltaX-2*deltaY;
    int step = 0;
    if(deltaX>0) step = (p2.x-p1.x) / deltaX;
    setPixel(p1);
    for (; p1.y < p2.y;) {
        p1.y++;
        if(p0<0) {
            p0+=twDeltaX;
        } else {
            p0+=twDeltaXmTwoDeltaY;
            p1.x+=step;
        }
        setPixel(p1);
        cout<<p1.x<<","<<p1.y<<endl;
    }

}

/*
 * 简单Bresenham 算法
 * 只处理|m|<=1
 */
void LineBres_1(Point p1, Point p2) {
    if(p2.x<p1.x) {
        Point t = p2;
        p2=p1;
        p1=t;
    }

    int deltaX = abs(p2.x-p1.x), deltaY = abs(p2.y-p1.y);
    if(deltaX<deltaY) {
        LineBres_2(p1, p2);
        return;
    }
    int p0 = 2*deltaY-deltaX;
    int twDeltaY = 2*deltaY, twDeltaYmTwoDeltaX = 2*deltaY-2*deltaX;
    int step = 0;
    if(deltaY>0) step = (p2.y-p1.y) / deltaY;
    setPixel(p1);
    for (; p1.x < p2.x;) {
        p1.x++;
        if(p0<0) {
            p0+=twDeltaY;
        } else {
            p0+=twDeltaYmTwoDeltaX;
            p1.y+=step;
        }
        setPixel(p1);
        cout<<p1.x<<","<<p1.y<<endl;
    }

}


int main(void) {
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

        LineBres_1(Point(0,0),Point(100,70));
        LineBres_1(Point(0,0),Point(100,-70));
        LineBres_1(Point(0,0),Point(70,100));
        LineBres_1(Point(0,0),Point(-70,100));
        LineBres_1(Point(0,0),Point(-100,70));
        LineBres_1(Point(0,0),Point(-70,-100));
        LineBres_1(Point(0,0),Point(-100,-70));
        LineBres_1(Point(0,0),Point(70,-100));

        LineBres_1(Point(0,0),Point(0,100));
        LineBres_1(Point(0,0),Point(0,-100));
        LineBres_1(Point(0,0),Point(100,0));
        LineBres_1(Point(0,0),Point(-100,0));
        LineBres_1(Point(0,0),Point(0,0));



        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);
        //break;
    }
    glfwTerminate();
    return 0;
}
