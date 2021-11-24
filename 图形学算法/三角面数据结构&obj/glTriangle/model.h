//
// Created by chenbinbin on 2021/11/12.
//
#include <vector>
#include <sstream>
#include <iostream>
#include <unordered_map>
#include <map>
#include "glew/2.2.0_1/include/GL/glew.h"
using namespace std;

#ifndef GLTRIANGLE_MODEL_H
#define GLTRIANGLE_MODEL_H

unsigned char*LoadFileContent(const char*path, int &filesize);

/*
 * 存储点坐标
 * 邻接表法存储邻接点
 *
 */
class Point {
public:
    double Data[3];
    vector<int> links; // 相连的点
    vector<int> edges; // 边索引
    vector<int> faces; // 面索引
};

class TexCoord {
public:
    double Data[2];
};

/* 边定义，通过点索引表示
 * 加入连接面的索引，方面查询
 *
 */
class Edge {
public:
    int p[2]; // 端点
    vector<int> faces; // 相连面的编号
    Edge(){}
};

/*
 * 一个面，由多多个顶点表示的凸多边形, 逆时针表示
 * 如果想表示一个凹多边形，可以把凹多边形拆成多个凸多边形表示
 * 加入点和边，可以快速找到相关的点和边，再通过边可以找到相连的面
 * 后续可扩展纹理坐标等信息
 */
class Face {
public:
    GLdouble points[3]; // 点编号
    int texcoord[3]; // 纹理坐标
    int edges[3]; // 相关的边
    Face(){};
};


class ObjectModel {
public:
    ObjectModel(const char * objFileName);
    void setTextureFromBmp(const char *texFileName);  //从obj文件创建纹理
    void objDraw();
    void addFace(Face face);
    int addEdge(int p1,int p2);
    double distance(int p1, int p2);
    vector<Point> points; // 点集
    vector<Edge> edges;
    vector<Face> faces;
    vector<TexCoord> texcoord; // 纹理信息
    GLuint mTexture;
    unordered_map<string, int> edgeMap;
    int mapInd;


};

#endif //GLTRIANGLE_MODEL_H
