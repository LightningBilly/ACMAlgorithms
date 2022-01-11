#include "glew/2.2.0_1/include/GL/glew.h"
#include "glfw/3.3.4/include/GLFW/glfw3.h"
#include <iostream>
#include "BSpline.h"
#include <cmath>

using namespace std;

BSpline bs;
vector<Vector3> select;
int focus = -1;
bool isDrag = 0;

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode) {
    //如果按下ESC，把windowShouldClose设置为True，外面的循环会关闭应用
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
        std::cout << "ESC" << mode;
    }

    if (action != GLFW_PRESS)return;
    switch (key) {
        case GLFW_KEY_ESCAPE:
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_DELETE:
            select.erase(select.end()-1);
            bs = bs.CubicInterpolate(select);
            break;
        default:
            break;
    }

}

void mouse_click(GLFWwindow *window, int button, int action, int mods) {
    cout << button << "," << action << "," << mods << endl;
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    cout << xpos / 300 - 1 << "," << 1 - ypos / 300 << endl;
    cout << xpos-300 << "," << 300-ypos << endl;
    isDrag = action;
    if(action==0) {
        if(focus>-1) {
            select[focus] = {xpos - 300, 300 - ypos, 0};
        }else {
            select.push_back({xpos - 300, 300 - ypos, 0});
        }
        // select = {{-150,0,0},{-60,60,0},{0,0,0},{90,100,0},{150,10,0},};
        bs = bs.CubicInterpolate(select);
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
    glfwSetMouseButtonCallback(window, mouse_click);
    //glViewport(0, 0, 400, 400);
    gluOrtho2D(-300, 300.0, -300, 300.0);
    //循环，直到用户关闭窗口
    cout << 123 << endl;

    // vector<Vector3> vc({{-150,0,0},{-60,60,0},{0,0,0},{90,100,0},{150,10,0},});
    //vector<Vector3> vc({{-150,0,0},{-60,0,0},{0,0,0}});
    // bs = bs.CubicInterpolate(vc);

    while (!glfwWindowShouldClose(window)) {
        /*******轮询事件*******/
        glfwPollEvents();
        // cout<<456<<endl;
        //选择清空的颜色RGBA

        glEnable(GL_DEPTH_TEST);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if(!isDrag) {
            focus = -1;
            for (int i = 0; i < select.size(); i++) {
                if (PointDistance({xpos - 300, 300 - ypos, 0}, select[i]) < 7) {
                    focus = i;
                    break;
                }
            }
        }


        if(focus>-1) {
            glColor3f(1, 0, 0);
            glPointSize(10);
            glBegin(GL_POINTS);
            auto v = select[focus];
            glVertex3d(v.x, v.y, v.z);
            glEnd();
        }


        glColor3f(1, 0.5, 0.8);


        glPointSize(7);
        glBegin(GL_POINTS);
        for(auto v: select) {
            glVertex3d(v.x, v.y, v.z);
        }
        glEnd();


        bs.DrawBS();
        // bs.DrawControls();
        // glFlush();
        glfwSwapBuffers(window);
    }
    glfwTerminate();
    return 0;
}

/*
 *
    bs.m_vecCVs = {{-150,0,0},{-60,60,0},{0,0,0},{90,100,0},{150,10,0},};
    bs.m_vecKnots = {0,0,0,1,2,3,3,3};
    bs.m_nDegree = 3;
 */