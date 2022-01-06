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
1  1   1  1
上 下  右  左
用4位数对端点进行编码。
如果走出左右下上的边界则对应的位置为1
下图中，中间区域是有效剪裁区，编码为0000，其他区域根据定义进行编码

            |                 |
    1001    |      1000       |     1010
            |                 |
---------------------------------------------
            |                 |
    0001    |      0000       |     0010
            |                 |
---------------------------------------------
            |                 |
    0101    |      0100       |     0110
            |                 |
 假设线段两点编号为C1, C2
 1. 如果两端同一位都为1，则说明2点都在同一边界外边，要被丢弃。C1&C2 > 0
 2. 如果两端都在有效区域内，则完全保留。 C1|C2 == 0
 3. 对某一端进行左，右，下，上进行剪裁，形成新的线段，对线段重新编码。

 线段求交：
 这里可以使用解议程线段求交。[线段求交]()
 窗口的这边是平行于x轴或y轴的，所以只要用到 y=kx+b,
 x = y/k+b 的形式。

 */
const int codeLeft = 1, codeRight = 1<<1, codeDown = 1<<2, codeUp = 1<<3;
const int xWmin = -100, xWmax = 100, yWmin = -100, yWmax = 100;

int genCode(Point &p) {
    int code = 0;

    if(p.x<xWmin) code |= codeLeft;
    if (p.x>xWmax) code |= codeRight;
    if(p.y<yWmin) code |= codeDown;
    if(p.y>yWmax) code |= codeUp;

    return code;
}

bool inside(int code) {
    return !code;
}

bool reject(int code1, int code2) {
    return code1&code2;
}

bool accept(int code1, int code2) {
    return !(code1|code2);
}

void swap(Point &p1, Point &p2) {
    int a;
    a = p1.x;
    p1.x=p2.x;
    p2.x=a;

    a = p1.y;
    p1.y=p2.y;
    p2.y=a;
}

void CohenSutherLand(Point p1, Point p2) {
    bool done = false;
    int code1, code2;
    double k;
    while(!done) {
        code1 = genCode(p1);
        code2 = genCode(p2);

        if (accept(code1, code2)) {
            done = true;
            break;
        }

        if (reject(code1, code2)) {
            return;
        }

        // 点已经在框内不需要剪裁
        if(inside(code1)) {
            swap(p1, p2);
            continue;
        }

        if(doubleCmp(p1.x, p2.x)) k = (p1.y-p2.y)/(p1.x-p2.x);

        // 查看左边
        if(code1&codeLeft) {
            p1.y+=(xWmin - p1.x)*k;
            p1.x= xWmin;
            continue;
        }

        // 查看左边
        if (code1&codeRight){
            p1.y+=(xWmax - p1.x)*k;
            p1.x= xWmax;
            continue;
        }

        // 查看下方
        if(code1&codeDown){
            if(doubleCmp(p1.x, p2.x)) p1.x += (yWmin - p1.y)/k;
            p1.y = yWmin;
            continue;
        }

        // 查看上方
        if(code1&codeUp){
            if(doubleCmp(p1.x, p2.x)) p1.x += (yWmax - p1.y)/k;
            p1.y = yWmax;
            continue;
        }
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
        /*       LineBres_1(Point(150,-100),Point(150,0));
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
               CohenSutherLand(Point(150,-100),Point(150,0));
               CohenSutherLand(Point(145,-100),Point(-150,100));
               CohenSutherLand(Point(0,-150),Point(0,150));
               CohenSutherLand(Point(-150,0),Point(130,0));
               CohenSutherLand(Point(145,50),Point(-150,200));
               CohenSutherLand(Point(145,150),Point(-150,200));
               CohenSutherLand(Point(145,-50),Point(-150,-150));
               CohenSutherLand(Point(145,-120),Point(-150,-170));
               CohenSutherLand(Point(145,-150),Point(-150,-100));
               CohenSutherLand(Point(145,-150),Point(-150,-30));
               CohenSutherLand(Point(-150,-30), Point(145,150));
               CohenSutherLand(Point(-150,-30), Point(-145,150));
               CohenSutherLand(Point(-0,-130), Point(-145,-150));

        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);
        //break;
    }
    glfwTerminate();
    return 0;
}
