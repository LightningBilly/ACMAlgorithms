#include "glew/2.2.0_1/include/GL/glew.h"
#include "glfw/3.3.4/include/GLFW/glfw3.h"
#include <iostream>
#include <cmath>

using namespace std;

#define PI acos(-1)

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    std::cout << "ESC" << mode;
}


class Point {
public:
    int x, y;

    Point(int xx, int yy) : x(xx), y(yy) {}
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
 * x = x0+rx*cos(∂)
 * y = y0+ry*sin(∂)
 * 需要调用三角函数，非常耗时
 */
void EllipseSimple(Point center, double rx, double ry) {
    for (double i = 0.0; i < 360; i += 0.01) {
        setPixel(Point(center.x + rx * cos(i / 180 * PI), center.y + ry * sin(i / 180 * PI)));
    }

}

void setCirclePixel(Point p, Point center) {

    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (i * j == 0) continue;
            setPixel(Point(p.x * i + center.x, p.y * j + center.y));
            setPixel(Point(p.y * i + center.x, p.x * j + center.y));
        }

    }
}


void setEllipsePixel(Point p, Point center) {
    for (int i = -1; i < 2; ++i) {
        for (int j = -1; j < 2; ++j) {
            if (i * j == 0) continue;
            setPixel(Point(p.x * i + center.x, p.y * j + center.y));
            // setPixel(Point(p.y*i+center.x, p.x*j+center.y));
        }

    }
}

/*
 * 中点法做椭圆
 * 前半部署
 * p0 = Ry^2-Rx^2*Ry+0.25*Rx^2
 * p<0, x++ y不变，p += 2Ry^2*x+Ry^2, 2Ry^2*x 可以通过递推得到
 * p>=0, x++,y++, p += 2Ry^2*x+Ry^2 - 2Rx^2*y, 2Rx^2*y 可以通过递推得到;
 *
 * 后半部分
 * p0 = Ry^2(x+0.5)^2-Rx^2*(y-1)^2-Rx^2*Ry^2
 * p>0, y--, x不变，p += -2Rx^2*y+Rx^2, 2Rx^2*y 可以递推得到
 * p<=0, y--, x++, p += -2Rx^2*y+Rx^2 + 2Ry^2*x, 2Ry^2*x 可以通过递推得到;
 */

void ellipseMidPoint(Point center, int Rx, int Ry) {
    int Rx2 = Rx * Rx;
    int Ry2 = Ry * Ry;
    int twoRx2 = 2 * Rx2;
    int twoRy2 = 2 * Ry2;

    Point p(0, Ry);
    int px = 0;
    int py = twoRx2 * Ry;
    int p0 = Ry2 - (Rx2 * Ry) + (0.5 * Rx2);

    while (px < py) {
        p.x++;
        px += twoRy2;
        if (p0 < 0) {
            p0 += px + Ry2;
        } else {
            p.y--;
            py -= twoRx2;
            p0 += px + Ry2 - py;
        }

        setEllipsePixel(p, center);
    }

    int p1 = Ry2 * (p.x + 0.5) * (p.x + 0.5) + Rx2 * (p.y - 1) * (p.y - 1) - Rx2 * Ry2;

    while (p.y >= 0) {
        p.y--;
        py -= twoRx2;

        if (p1 > 0) {
            p1 += -py + Rx2;
        } else {
            p.x++;
            px += twoRy2;
            p1 += -py + Rx2 + px;
        }
        setEllipsePixel(p, center);
    }
}


int main(void) {
    cout << acos(-1) << endl;
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
    cout << 123 << endl;
    while (!glfwWindowShouldClose(window)) {
        /*******轮询事件*******/
        glfwPollEvents();
        // cout<<456<<endl;
        //选择清空的颜色RGBA
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        // glColor3f(0,0, 0);
        glMatrixMode(GL_PROJECTION);

        // EllipseSimple(Point(0,0), 155, 105);

        ellipseMidPoint(Point(0, 0), 150, 100);
        // circleMidPoint(Point(0,10), 50);



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
