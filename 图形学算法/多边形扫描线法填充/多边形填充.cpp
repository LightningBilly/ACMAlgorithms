#include "glew/2.2.0_1/include/GL/glew.h"
#include "glfw/3.3.4/include/GLFW/glfw3.h"
#include <iostream>
#include <vector>
using namespace std;

double esp = 1e-6;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    std::cout<<"ESC"<<mode;
}

int cmpDouble(double a, double b) {
    if (fabs(a-b)<esp)return 0;
    if (a<b)return -1;
    return 1;
}

class ActiveEdge {
public:
    double x; // 每次+k
    int maxY;
    int deltaX, deltaY;
    double k ; // ∆x/∆y
};

int curY;

bool cmpActiveEdge(ActiveEdge a, ActiveEdge b) {
    if(curY>=a.maxY)return false;
    if(curY>=b.maxY)return true;
    int eq = cmpDouble(a.x, b.x);
    if(eq) return eq<0; // x小的排前面
    eq = cmpDouble(a.k, b.k); // 斜率小说明x在减小得快
    return eq<0;
}

class Point {
public:
    int x, y;
    Point(int xx, int yy):x(xx), y(yy){}
    Point(){}
};

class Line {
public:
    Point low, high;
    Line(){}
    Line(Point tp1, Point tp2):low(tp1),high(tp2){
        if (tp2.y<tp1.y){
            low=tp2;
            high=tp1;
        }
    }

    void Out(){
        printf("%d,%d-%d,%d\n", low.x, low.y, high.x, high.y);
    }
};

bool cmpLine(Line a, Line b) {
    return a.low.y<b.low.y;
}

void setPixel(Point p) {
    // cout<<p.x<<","<<p.y<<endl;
    glPointSize(1.3);
    glBegin(GL_POINTS);
    glVertex2i(p.x, p.y);
    glEnd();
    glFlush();
}

void drawHorizon(int start, int end, int y) {
    for(;start<=end;start++){
        setPixel(Point(start, y));
    }
}

/*
 * 扫描线从低到高扫描
 * 先更新活动边表x值，每一对点画一条线段。
 * 查看当前y值是否有新线段加入，有则入加，并排序。
 */
void fillPolygen(vector<Line> polygen) {
    //1. 对线段按照底端从低到高排序
    sort(polygen.begin(), polygen.end(), cmpLine);

    // 2. 查找最低点与最高点
    int minY = polygen[0].low.y;
    int maxY = polygen[0].high.y;
    for (Line l : polygen){
        maxY = max(maxY, l.high.y);
    }

    // 定义活动边表
    vector<ActiveEdge> ae;
    for(int h=minY, j=0;h<=maxY;h++) {
        bool needSort=false;
        // 更新提有活动边x值。
        for(int i=0;i<ae.size();i++) {
            if (h>ae[i].maxY)continue;
            if (ae[i].deltaX) ae[i].x += ae[i].k;
            // 每两点画一条线
            if (i&1 && h<=ae[i].maxY) {
                drawHorizon(ae[i-1].x, ae[i].x, h);
            }
            if (h>=ae[i].maxY && !needSort) needSort=true; // 有边需要删除，删除采用假删除，将无用边排到最后边去
        }

        // 查看当前是否有边加入
        for(;j<polygen.size() && polygen[j].low.y==h;j++){
            // 平行边不需要
            if (polygen[j].low.y== polygen[j].high.y)continue;
            if (!needSort) needSort=true; // 有新边加入，需要排序
            ActiveEdge newEdge;
            newEdge.maxY = polygen[j].high.y;
            newEdge.x = polygen[j].low.x;
            newEdge.deltaX = polygen[j].high.x-polygen[j].low.x;
            newEdge.deltaY = polygen[j].high.y-polygen[j].low.y;
            newEdge.k = double(newEdge.deltaX) / double(newEdge.deltaY); // ∆x/∆y
            ae.push_back(newEdge);
        }

        if (needSort) {
            curY=h;
            sort(ae.begin(), ae.end(), cmpActiveEdge);
        }
    }
}


vector<Line> getTriangle() {
    vector<int> pList = {
            -100,-200,200,50,
            -100,-200,0,100,
            0,100,200,50,
    };

    vector<Line> sq;
    for(int i=0;i+3<pList.size();i+=4){
        sq.push_back(Line(Point(pList[i]-350,pList[i+1]-250),Point(pList[i+2]-350,pList[i+3]-250)));
    }

    return sq;
}

vector<Line> get5Triangle() {
    vector<int> pList = {
            -100,-200,150,0,
            -100,-200,0,100,
            -150,0,150,0,
            100,-200,-150,0,
            100,-200,0,100,
    };

    vector<Line> sq;
    //glBegin(GL_LINES);
    for(int i=0;i+3<pList.size();i+=4){
        sq.push_back(Line(Point(pList[i]-350,pList[i+1]+50),Point(pList[i+2]-350,pList[i+3]+50)));
        //glVertex2i(pList[i],pList[i+1]);
        //glVertex2i(pList[i+2],pList[i+3]);
    }
    //glEnd();

    return sq;
}


vector<Line> getUnNor() {
    vector<int> pList = {
            -10,10,100,-50,
            120,51,150,-58,
            170,100,200,-50,
            300,150,250,120,300,170,270,200,
            210,180,200,250,
            150,210,
            110,230,
    };

    vector<Line> sq;
    // glBegin(GL_LINES);
    for(int i=0;i+3<pList.size();i+=2){
        sq.push_back(Line(Point(pList[i],pList[i+1]),Point(pList[i+2],pList[i+3])));
        //glVertex2i(pList[i],pList[i+1]);
        //glVertex2i(pList[i+2],pList[i+3]);
    }
    int n = pList.size();
    sq.push_back(Line(Point(pList[n-2],pList[n-1]),Point(pList[0],pList[1])));
    //glVertex2i(pList[n-2],pList[n-1]);
    //glVertex2i(pList[0],pList[1]);
    //glEnd();

    return sq;
}

vector<Line> getSquare() {
    vector<int> pList = {
            0,0,100,0,
            0,0,0,100,
            100,0,100,100,
            0,100,100,100,
    };

    vector<Line> sq;
    for(int i=0;i+3<pList.size();i+=4){
        sq.push_back(Line(Point(pList[i]-350,pList[i+1]+250),Point(pList[i+2]-350,pList[i+3]+250)));
    }

    return sq;
}

int main(void) {

    //初始化GLFW库
    if (!glfwInit())
        return -1;
    //创建窗口以及上下文
    GLFWwindow *window = glfwCreateWindow(1024, 900, "hello world", NULL, NULL);
    if (!window) {
        //创建失败会返回NULL
        glfwTerminate();
    }

    //建立当前窗口的上下文
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback); //注册回调函数
    //glViewport(0, 0, 400, 400);
    gluOrtho2D(-512, 512, -450, 450);
    //循环，直到用户关闭窗口
    cout<<123<<endl;
    while (!glfwWindowShouldClose(window)) {
        /*******轮询事件*******/
        glfwPollEvents();
        //选择清空的颜色RGBA
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);


        vector<Line> polygen = getSquare();
        glColor3f(1,0, 0);
       fillPolygen(polygen);

        polygen = getTriangle();
        glColor3f(1,1, 0);
       fillPolygen(polygen);


        glColor3f(1,0, 1);
        polygen = get5Triangle();
       fillPolygen(polygen);

        polygen = getUnNor();
        fillPolygen(polygen);


        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);
        //break;
    }
    glfwTerminate();
    return 0;
}
