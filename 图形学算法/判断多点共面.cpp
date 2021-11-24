#include <iostream>
#include <vector>

using namespace std;

class Point {
public:
    int x, y, z;

    Point(int xx, int yy, int zz) : x(xx), y(yy), z(zz) {}

    Point() {}
    Point operator - (Point &a) const {
        return Point(x-a.x, y-a.y, z-a.z);
    }
};

bool isZero(Point p) {
    return p.x+p.y+p.z ==0;
}

Point xmult(Point p1, Point p2) {
    return Point(p1.y*p2.z - p1.z*p2.y, p1.z*p2.x - p1.x*p2.z, p1.x*p2.y - p1.y*p2.x);
}

// 判断多点是否共面，没有重复的点，行列式法
/*
 * 平面方程：Ax+By+Cz+D=0
 * 先选出不共线三点计算出A,B,C，否则如果不存在非共线的点，则都在一平面上。
 * 设有三点P1(x1,y1,z1), P2(x2,y2,z2), P3(x3,y3,z3)
 * A=y1(z2-z3)+y2(z3-z1)+y3(z1-z2)
 * B=z1(x2-x3)+z2(x3-x1)+z3(x1-x2)
 * C=x1(y2-y3)+x2(y3-y1)+x3(y1-y2)
 * D=-x1(y2z3-y3z2)-x2(y3z1-y1z3)-x3(y1z2-y2z1)
 * 将其他点代入平面方程，检查是否为0即可
 */
bool IsInPlane(vector<Point> points) {
    int d3 = -1;
    for (int i = 2; i < points.size(); ++i) {
        Point c = xmult(Point(points[1].x-points[0].x,points[1].y-points[0].y,points[1].z-points[0].z),
                        Point(points[i].x-points[0].x,points[i].y-points[0].y,points[i].z-points[0].z));
        if(!isZero(c)) {
            d3=i;
            break;
        }
    }
    // 所有点都共线
    if(d3==-1) {
        return true;
    }

    vector<Point> planePoints={points[0],points[1],points[d3]};
    // 计算ABCD
    int A = 0;
    for (int i = 0; i < 3; ++i) {
        A += planePoints[i].y * (planePoints[(i + 1) % 3].z - planePoints[(i + 2) % 3].z);
    }
    int B = 0;
    for (int i = 0; i < 3; ++i) {
        B += planePoints[i].z * (planePoints[(i + 1) % 3].x - planePoints[(i + 2) % 3].x);
    }
    int C = 0;
    for (int i = 0; i < 3; ++i) {
        C += planePoints[i].x * (planePoints[(i + 1) % 3].y - planePoints[(i + 2) % 3].y);
    }

    int D = 0;
    for (int i = 0; i < 3; ++i) {
        D -= planePoints[i].x *
             (planePoints[(i + 1) % 3].y * planePoints[(i + 2) % 3].z - planePoints[(i + 2) % 3].y * planePoints[(i + 1) % 3].z);
    }

    // 检查剩余点
    for (int i = 0; i < points.size(); ++i) {
        if (A * points[i].x + B * points[i].y + C * points[i].z + D != 0) {
            return false;
        }
    }

    return true;
}


// 判断多点是否共面，没有重复的点, 法点式
/*
 * 平面方程：Ax+By+Cz+D=0
 * 先选出不共线三点计算出A,B,C，否则如果不存在非共线的点，则都在一平面上。
 * 设有三点P1(x1,y1,z1), P2(x2,y2,z2), P3(x3,y3,z3)
 * P=xmult(P2-P1, P3-P1), 求叉积得到法向
 * A=P.x, B=P.y, C=P.z, D=-P*P1
 * 将其他点代入平面方程，检查是否为0即可
 */
bool IsInPlane2(vector<Point> points) {
    int d3 = -1;
    for (int i = 2; i < points.size(); ++i) {
        Point c = xmult(points[1]-points[0],points[i]-points[0]);
        if(!isZero(c)) {
            d3=i;
            break;
        }
    }
    // 所有点都共线
    if(d3==-1) {
        return true;
    }

    Point P = xmult(points[1]-points[0], points[d3]-points[0]);
    int D = -(P.x*points[0].x +P.y*points[0].y +P.z*points[0].z);

    // 检查剩余点
    for (int i = 0; i < points.size(); ++i) {
        if (P.x * points[i].x + P.y * points[i].y + P.z * points[i].z + D != 0) {
            return false;
        }
    }

    return true;
}

vector<vector<Point>> getTests() {
    vector<vector<Point>> ans;
    ans.push_back({{1,2,0},{3,4,0}});
    ans.push_back({{1,0,0},{2,0,0},{3,0,0},{4,0,0}});
    ans.push_back({{1,2,0},{3,4,0},{4,5,0},{6,7,0},{6,10,0}});
    ans.push_back({{1,2,0},{3,4,0},{4,5,0},{100,7,1}});
    ans.push_back({{1,2,10},{3,4,1},{4,5,2},{100,7,10}});
    ans.push_back({{1,0,0},{0,1,0},{0,0,1},{2,-1,0}});
    ans.push_back({{1,0,0},{0,1,0},{0,0,1},{2,-1,1}});
    return ans;
}

int main(void) {
    auto tests = getTests();

    for (auto v:tests) {
        cout<<"p"<<endl;
        cout<<IsInPlane(v)<<endl;
        cout<<IsInPlane2(v)<<endl;
    }
    return 0;
}