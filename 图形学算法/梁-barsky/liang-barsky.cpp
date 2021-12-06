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
        // cout<<p1.x<<","<<p1.y<<endl;
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
        // cout<<p1.x<<","<<p1.y<<endl;
    }

}


/*
P0(X0, Y0), Pend(Xend, Yend)
∆x = Xend-X0, ∆y = Yend-Y0.
xWmin<= X0 + u*∆x <= xWmax (0<=u<=1),
yWmin<= Y0 + u*∆y <= yWmax (0<=u<=1),

拆解上述不等可以得到


 */
const int xWmin = -100, xWmax = 100, yWmin = -100, yWmax = 100;

bool clipTest(double p, double q, double &u1, double &u2){
    int d = doubleCmp(p, 0);
    if(0==d) {
        return doubleCmp(q,0)>=0;
    }

    double r = q/p;
    if (d<0) {
        if(doubleCmp(r, u2)>0)return false;
        u1 = max(r, u1);
    } else {
        if(doubleCmp(r, u1)<0)return false;
        u2 = min(r, u2);
    }
    return true;
}

void LiangBarsky(Point p1, Point p2) {
    double u1 = 0.0, u2 = 1.0;
    int dx = p2.x-p1.x;
    int dy = p2.y-p1.y;

    if(!clipTest(-dx, p1.x-xWmin, u1, u2))return;
    if(!clipTest(dx, xWmax-p1.x, u1, u2))return;
    if(!clipTest(-dy, p1.y-yWmin, u1, u2))return;
    if(!clipTest(dy, yWmax-p1.y, u1, u2))return;
    if (u2<1) {
        p2.x = p1.x + u2*dx;
        p2.y = p1.y + u2*dy;
    }

    if (u1>0) {
        p1.x += u1*dx;
        p1.y += u1*dy;
    }

    LineBres_1(p1, p2);
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

        // 画边框
        glColor3f(1, 0, 0); //Red
        LineBres_1(Point(xWmin,yWmin),Point(xWmin,yWmax));
        LineBres_1(Point(xWmin,yWmin),Point(xWmax,yWmin));
        LineBres_1(Point(xWmax,yWmax),Point(xWmax,yWmin));
        LineBres_1(Point(xWmin,yWmax),Point(xWmax,yWmax));

        glColor3f(0, 1, 0); //green
        // 剪裁前线段
        /*
               LineBres_1(Point(150,-100),Point(150,0));
               LineBres_1(Point(145,-100),Point(-150,100));
               LineBres_1(Point(0,-150),Point(0,150));
               LineBres_1(Point(-150,0),Point(130,0));
               LineBres_1(Point(145,50),Point(-150,200));
               LineBres_1(Point(145,150),Point(-150,200));
               LineBres_1(Point(145,-50),Point(-150,-150));
               LineBres_1(Point(145,-120),Point(-150,-170));
               LineBres_1(Point(145,-150),Point(-150,-100));
               LineBres_1(Point(145,-150),Point(-150,-30));
               LineBres_1(Point(-150,-30), Point(145,150));
               LineBres_1(Point(-150,-30), Point(-145,150));
               LineBres_1(Point(-0,-130), Point(-145,-150));
*/
        // glColor3f(0, 0, 1); //green
        // 剪裁后线段
        LiangBarsky(Point(150,-100),Point(150,0));
        LiangBarsky(Point(145,-100),Point(-150,100));
        LiangBarsky(Point(0,-150),Point(0,150));
        LiangBarsky(Point(-150,0),Point(130,0));
        LiangBarsky(Point(145,50),Point(-150,200));
        LiangBarsky(Point(145,150),Point(-150,200));
        LiangBarsky(Point(145,-50),Point(-150,-150));
        LiangBarsky(Point(145,-120),Point(-150,-170));
        LiangBarsky(Point(145,-150),Point(-150,-100));
        LiangBarsky(Point(145,-150),Point(-150,-30));
        LiangBarsky(Point(-150,-30), Point(145,150));
        LiangBarsky(Point(-150,-30), Point(-145,150));
        LiangBarsky(Point(-0,-130), Point(-145,-150));

        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);
        //break;
    }
    glfwTerminate();
    return 0;
}
