#include "glew/2.2.0_1/include/GL/glew.h"
#include "glfw/3.3.4/include/GLFW/glfw3.h"
#include <iostream>
#include <vector>

using namespace std;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    std::cout<<"ESC"<<mode;
}

const double esp = 1e-6;

int doubleCmp(double a, double b) {
    if (fabs(a-b)< esp)return 0;
    if (a<b)return -1;
    return 1;
}

class Point {
public:
    double x, y;
    Point(int xx, int yy):x(xx), y(yy){}
};

void setPixel(Point p) {
    // cout<<p.x<<","<<p.y<<endl;
    glPointSize(4);
    glBegin(GL_POINTS);
    glVertex2d(p.x, p.y);
    glEnd();
    glFlush();
}


vector<int> binomialCoeffs(int n) {
    vector<int> C(n, 1);

    for(int i=0;i<n;i++) {
        C[i]=1;
        for(int j=i+1;j<n;j++) {
            C[i]*=j;
        }

        for(int j=1;j<n-i;j++) {
            C[i]/=j;
        }
    }

    return C;
}

/*
 p(u) = sum(0,k) p
 */

void bezier(vector<Point> points) {
    vector<int>C = binomialCoeffs(points.size());

    for(int i=0;i<=1000;i++) {
        Point p(0,0);
        double u = i*1.0/1000;
        for(int j=0;j<points.size();j++) {
            double x = C[j]*pow(u, j)*pow(1-u, points.size()-1-j);
            p.x += points[j].x*x;
            p.y += points[j].y*x;
        }
        setPixel(p);
    }
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
    //glViewport(0, 0, 400, 400);
    gluOrtho2D(-300, 300.0, -300, 300.0);
    //循环，直到用户关闭窗口
    cout<<123<<endl;
    while (!glfwWindowShouldClose(window)) {
        /*******轮询事件*******/
        glfwPollEvents();
        // cout<<456<<endl;
        //选择清空的颜色RGBA
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);

        vector<Point> points({
            {-100,-40},
            {-50,200},
            {10,-200},
            {100,40}
        });
        // 画边框

        glColor3f(1,1, 1);
        glLineWidth (4);
        glBegin(GL_LINE_STRIP);
        for(auto p:points) {
            glVertex2d(p.x, p.y);
        }
        glEnd();
        glFlush();

        glColor3f(1, 0, 0); //Red
        bezier(points);

        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);
        //break;
    }
    glfwTerminate();
    return 0;
}
