//
// Created by chenbinbin on 2021/11/12.
//

#include "model.h"


unsigned char*LoadFileContent(const char *path, int &filesize)   //该函数来自于萌谷教程
//读取文件，返回文件内容，把文件大小赋值给filesize
{
    unsigned char*fileContent = nullptr;
    filesize = 0;
    FILE *pFile = fopen(path, "rb");   //二进制方式读取
    if (pFile)
    {
        fseek(pFile, 0, SEEK_END);      //将文件指针移动到文件末尾
        int nLen = ftell(pFile);        //距离文件头部的距离   //这里指文件大小
        if (nLen > 0)
        {
            rewind(pFile);          //移动到文件头部
            fileContent = new unsigned char[nLen + 1];
            //为文件指针开辟空间
            fread(fileContent, sizeof(unsigned char), nLen, pFile);
            //将pFile的内容读入fileContent
            fileContent[nLen] = '\0';         //文件末尾加上\0
            filesize = nLen;                  //为文件大小赋值
        }
        fclose(pFile);
    }
    return fileContent;
}

// --------------- 添加 元素 ---------------- start
/*
 * 添加一条边
 * 如果边已经添加过则返回编号
 * 更新点邻接表
 */
int ObjectModel::addEdge(int p1,int p2)
{
    if(p1>p2){
        return addEdge(p2, p1);
    }

    string key = to_string(p1)+"|"+to_string(p2);
    if(edgeMap.count(key)==0) {
        points[p1].links.push_back(p2);
        points[p2].links.push_back(p1);
        points[p1].edges.push_back(mapInd);
        points[p2].edges.push_back(mapInd);
        Edge e;
        e.p[0]=p1;
        e.p[1]=p2;
        edges.push_back(e);
        edgeMap[key]=mapInd++;
    }
    return edgeMap[key];
}

/*
 * 添加一个面，
 * 影响边，增加3条边，需要判断是否重复添加
 *
 */
void ObjectModel::addFace(Face face) {
    for(int i=0;i<3;i++) {
        int ind = addEdge(face.points[i], face.points[(i+1)%3]);
        // 对边添加面
        edges[ind].faces.push_back(faces.size());

        // 对应点添加面
        points[face.points[i]].faces.push_back(faces.size());
        face.edges[i]=ind;
    }

    faces.push_back(face);
}
// --------------- 添加 元素 ---------------- end

double getDis(Point p1, Point p2)
{
    double d = 0;
    for (int i=0;i<3;i++){
        double t = p1.Data[i]-p2.Data[i];
        d += t*t;
    }
    return sqrt(d);
}

// 计算2点之间沿边行走的最短距离，dijstra算法
double ObjectModel::distance(int p1, int p2)
{
    vector<double> dis(points.size(),INT_MAX);
    map<int, double> que;
    que[p1]=0;
    while(que.size()>0)
    {
        auto front = que.begin(); // 选择当前距离最小的，并确定与p1的距离。
        que.erase(front);
        dis[front->first]=front->second;

        // 从该点出发加入点。
        for(int p: points[front->first].links) {
            double d =getDis( points[front->first], points[p]);
            if (d+front->second< dis[p]) {// 有更优解则替换
                que[p] = d+front->second;
            }
        }
    }
    cout<<"p1:"<< points[p1].Data[0]<<"|"<<points[p1].Data[1]<<"|"<<points[p1].Data[2]<<endl;
    cout<<"p2:"<< points[p2].Data[0]<<"|"<<points[p2].Data[1]<<"|"<<points[p2].Data[2]<<endl;
    cout<<getDis(points[p1], points[p2])<<endl;
    return dis[p2];
}

// objmodel 方法
ObjectModel::ObjectModel(const char * objFileName)
{
    mapInd=0;
    int nFileSize = 0;
    unsigned char*fileContent = LoadFileContent(objFileName, nFileSize);    //读取文件内容
    if (fileContent == nullptr)     //文件为空
    {
        return;
    }
    stringstream ssFileContent((char*)fileContent);   //流读取文件内容
    string temp;       //接受无关信息
    char szoneLine[256];        //读取一行的数据
    while (!ssFileContent.eof())
    {
        memset(szoneLine, 0, 256);        //  每次循环初始化数组szoneLine
        ssFileContent.getline(szoneLine, 256);      //流读取一行
        if (strlen(szoneLine) > 0)       //该行不为空
        {
            if (szoneLine[0] == 'v')     //v开头的数据
            {
                stringstream ssOneLine(szoneLine);        //数据存储到流中 方便赋值
                if (szoneLine[1] == 't')       //纹理信息
                {
                    ssOneLine >> temp;     //接受标识符 vt
                    TexCoord tempTexcoord;
                    ssOneLine >> tempTexcoord.Data[0] >> tempTexcoord.Data[1];   //数据存入临时变量中
                    texcoord.push_back(tempTexcoord);         //存入容器

                }
                else                          //点的位置信息
                {
                    ssOneLine >> temp;
                    Point tempLocation;
                    ssOneLine >> tempLocation.Data[0] >> tempLocation.Data[1] >> tempLocation.Data[2];
                    points.push_back(tempLocation);
                }
            }
            else if (szoneLine[0] == 'f')          //面信息
            {
                stringstream ssOneLine(szoneLine);     //流读取一行数据
                ssOneLine >> temp; //接收标识符f
                //    f信息    exp： f 1/1/ 2/2 3/3      位置索引/纹理索引   三角面片 三个点构成一个面
                string vertexStr;   //接收流的内容
                Face tempFace;
                for (int i = 0; i < 3; ++i)         //每个面三个点
                {
                    ssOneLine >> vertexStr;           //从流中读取点的索引信息
                    size_t pos = vertexStr.find_first_of('/');       //找到第一个/的位置      //即找到点的位置信息
                    string locIndexStr = vertexStr.substr(0, pos);       //赋值点的位置信息
                    string texIndexSrt = vertexStr.substr(pos + 1, vertexStr.length()-1-pos);       //赋值点的纹理坐标信息

                    tempFace.points[i] = atoi(locIndexStr.c_str())-1;        //将索引信息从 srting转换为 int     //位置索引信息赋值
                    tempFace.texcoord[i] = atoi(texIndexSrt.c_str())-1;         //纹理坐标索引信息赋值
                }
                addFace(tempFace);
            }
        }   //end 非0行
    }  //end while

    cout<<"dis:"<<distance(0,90)<<endl;
    delete fileContent;
}

void ObjectModel::objDraw()
{
    // glBindTexture(GL_TEXTURE_2D, mTexture);
    // glColor3f(1, 0, 0); //Red
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glColor3f(0.2, 0.5, 0.8);
    glBegin(GL_TRIANGLES);
    for (auto faceIndex = faces.begin(); faceIndex != faces.end(); ++faceIndex)         //循环遍历face信息
    {
        //第一个点的法线，纹理，位置信息
        // glNormal3fv(mNormal[faceIndex->vertex[0][2]-1].Data);
        glTexCoord2dv(texcoord[faceIndex->texcoord[0]].Data);
        glVertex3dv(points[faceIndex->points[0]].Data);
        //第二个点的法线，纹理，位置信息
        // glNormal3fv(mNormal[faceIndex->vertex[1][2]-1].Data);
        glTexCoord2dv(texcoord[faceIndex->texcoord[1]].Data);
        glVertex3dv(points[faceIndex->points[1]].Data);
        //第三个点的法线，纹理，位置信息
        // glNormal3fv(mNormal[faceIndex->vertex[2][2]-1].Data);
        glTexCoord2dv(texcoord[faceIndex->texcoord[2]].Data);
        glVertex3dv(points[faceIndex->points[2]].Data);
    }
    glEnd();
}

void ObjectModel::setTextureFromBmp(const char * texFileName) {
    // mTexture = CreateTexture2DFromBMP(texFileName);
}
