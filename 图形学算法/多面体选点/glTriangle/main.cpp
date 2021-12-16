#include "glew/2.2.0_1/include/GL/glew.h"
#include "glfw/3.3.4/include/GLFW/glfw3.h"
#include <iostream>
#include "model.h"
#include <cmath>
using namespace std;

#define  ColoredVertex(c,v) do{ glColor3fv(c); glVertex3fv(v); }while(0)

char *path = "/Users/bytedance/CLionProjects/glTriangle/d.txt";
ObjectModel om(path);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if(key==GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
        std::cout << "ESC" << mode;
    }

    if (action != GLFW_PRESS)return;
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case  GLFW_KEY_D:
        {

        }
            break;
        default:
            break;
    }

}

int moving = 0;
double sx=0, sy=0, angy=0, angx=0;
Vector3 select;

void mouse_click(GLFWwindow* window, int button, int action, int mods) {
    cout<<button<<","<<action<<","<<mods<<endl;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    sx = xpos;
    sy = ypos;
    cout<<xpos/300-1<<","<<1-ypos/300<<endl;
    cout<<xpos<<","<<ypos<<endl;
    moving = action;
    if(action==0) {
        select = om.getPointWithLine(Vector3(xpos / 300 - 1, 1 - ypos / 300, -1), Vector3(0, 0, 1));
        select.Out();
        if(select.z<99) {
            om.addPoint(select);
        }
    }
}


void  RevolveTriangle(){
    static   int  list  =   0 ;
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT  |  GL_DEPTH_BUFFER_BIT);
    //if (list  ==   0 ){  // 如果显示列表不存在，则创建
        GLfloat PointA[]  =  {  0.5f , - sqrt( 6.0f ) / 12 , - sqrt( 3.0f ) / 6 },  // 设置4个角的位置
                PointB[]  =  { - 0.5f , - sqrt( 6.0f ) / 12 , - sqrt( 3.0f ) / 6 },
                PointC[]  =  {  0.0f , - sqrt( 6.0f ) / 12 , sqrt( 3.0f ) / 3 },
                PointD[]  =  {  0.0f , sqrt( 6.0f ) / 4 ,   0 };

        GLfloat ColorR[]  =  { 1 , 0 , 0 },  // 设置4个角的颜色
                ColorG[]  =  { 0 , 1 , 0 },
                ColorB[]  =  { 0 , 0 , 1 },
                ColorY[]  =  { 1 , 1 , 1 };

        list  =  glGenLists( 1 );
        glNewList(list,GL_COMPILE);
/*
        glBegin(GL_TRIANGLES);  // 画出4个三角形
        // 平面BAD(左)
        ColoredVertex(ColorG,PointB);
        ColoredVertex(ColorR,PointA);
        ColoredVertex(ColorY,PointD);

        // 平面ABC(正)
        ColoredVertex(ColorR,PointA);
        ColoredVertex(ColorG,PointB);
        ColoredVertex(ColorB,PointC);

        // 平面ACD(右)
        ColoredVertex(ColorR,PointA);
        ColoredVertex(ColorB,PointC);
        ColoredVertex(ColorY,PointD);

        // 平面CBD(底)
        ColoredVertex(ColorB,PointC);
        ColoredVertex(ColorG,PointB);
        ColoredVertex(ColorY,PointD);
        glEnd();

*/
        glColor3f(1, 0, 0); //Red
        glPointSize(10);
        glBegin(GL_POINTS);
        glVertex3fv(PointD);
        glVertex3d(0,0,0);
        glEnd();

        glColor3f(0, 0, 0.4); //Red
        glLineWidth(5);

        glBegin(GL_LINE_STRIP);
        glVertex3fv(PointA);
        glVertex3fv(PointB);
        glVertex3fv(PointC);
        glVertex3fv(PointD);
        glVertex3fv(PointA);
        glVertex3fv(PointB);
        glVertex3fv(PointD);
        glVertex3fv(PointC);
        glVertex3fv(PointA);
        glEnd();

        glEndList();



    // 已经创建了显示列表，在每次绘制正四面体时将调用它
    glPushMatrix();  // 入矩阵栈
    glRotatef(angx, 0 , 1 , 0 );  // 旋转
    glRotatef(angy, 1 , 0 , 0 );  // 旋转
    //glRotatef(ang, 0 , 0 , 0.5 );  // 旋转
    glCallList(list);  // 调用列表
    glPopMatrix();  // 出矩阵栈

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


    while (!glfwWindowShouldClose(window)) {
        /*******轮询事件*******/
        glfwPollEvents();
        // cout<<456<<endl;
        //选择清空的颜色RGBA
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (moving && ypos>0 && xpos>0&&(fabs(ypos -sy)>2 || (fabs(xpos -sx)>2)))  {
            cout<<"cur p"<<xpos<<","<<ypos<<endl;
            angy += (sy-ypos) / 300 * 360 + 360;
            while (angy >= 360) angy -= 360;
            cout<<"angley:"<<angy<<endl;


            angx += (sx-xpos) / 300 * 360 + 360;
            while (angx >= 360) angx -= 360;
            cout<<"anglex:"<<angx<<endl;

            om.angy = angx;
            om.angx = angy;
            sx = xpos;
            sy = ypos;
        }

        /*
        sx = xpos/300-1;
        sy = -(ypos/300-1);
         */
        glClearColor(0, 0, 0, 1);
        // glColor3f(0,0, 0);
        glMatrixMode(GL_PROJECTION);
        om.pointTrance();
        om.objDraw();
        // RevolveTriangle();
//        glColor3f(1,0,0);
//        glPointSize(10);
//        glBegin(GL_POINTS);
//        glVertex3d(xpos/300-1, -ypos/300+1, -1 );
//        glEnd();
        // glGetFloatv()
        /******交换缓冲区，更新window上的内容******/
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}
