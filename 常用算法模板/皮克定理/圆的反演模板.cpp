#include<stdio.h>
#include<cmath>
#include <algorithm>
#include <vector>


using namespace std;

class Point {
public:
	double x, y;

	Point() {}
	Point(double a, double b) :x(a), y(b) {}
	Point(const Point& p) :x(p.x), y(p.y) {}

	void in() {
		scanf(" %lf %lf", &x, &y);
	}
	void out() {
		printf("%f %f\n", x, y);
	}

	double dis() {
		return sqrt(x * x + y * y);
	}

	Point operator -(const Point& p) const {
		return Point(x - p.x, y - p.y);
	}

	Point operator +(const Point& p) const {
		return Point(x + p.x, y + p.y);
	}
	Point operator *(double d)const {
		return Point(x * d, y * d);
	}

	Point operator /(double d)const {
		return Point(x / d, y / d);
	}


	void operator -=(Point& p) {
		x -= p.x;
		y -= p.y;
	}

	void operator +=(Point& p) {
		x += p.x;
		y += p.y;
	}
	void operator *=(double d) {
		x *= d;
		y *= d;
	}

	void operator /=(double d) {
		this ->operator*= (1 / d);
	}
};

class Line {
public:
	Point front, tail;
	Line() {}
	Line(Point a, Point b) :front(a), tail(b) {}
};

// https://oi-wiki.org//geometry/inverse/#%E5%8F%82%E8%80%83%E8%B5%84%E6%96%99%E4%B8%8E%E6%8B%93%E5%B1%95%E9%98%85%E8%AF%BB
// 根据官方定义实现圆的反演
class Circle
{
public:
	Point center;
	double r;

	Circle(const Point& c, double a) :center(c), r(a) {}
	Circle() {}
	void in() {
		center.in();
		scanf("%lf", &r);
	}
	void out() {
		center.out();
		printf("%f\n", r);
	}

	// 不过圆心的圆进行反演，得到1个圆
	Circle invert(const Circle& A) {
		Circle B;
		double oa = (center - A.center).dis();
		B.r = r * r / 2 * (1.0 / (oa - A.r) - 1.0 / (oa + A.r));
		double ob = r * r / (oa + A.r) + B.r;
		B.center = center + (A.center - center) * ob / oa;
		return B;
	}

	// 过圆心的圆进行反演，得到1条直线
	Point invert2line(const Circle& c) {
		return Point();
	}
	// 求反演点
	Point invertPoint(const Point& p) {
		Point dir = p - center;
		double dis = dir.dis();
		dir /= dis;
		dis = r * r / dis;
		return center + dir * dis;
	}

	// 直线反演，得到圆
	Circle invert2circle(const Line& l) {
		Point dir = l.front - l.tail;
		dir /= dir.dis();
		Circle c(Point(0, 0), 0);
		// 计算投影
		Point cdir = center - l.tail;
		Point project = l.tail + dir * (dir.x * cdir.x + dir.y * cdir.y);// 点乘得到投影长度

		// 计算圆到直线的距离
		Point op = project - center;
		if (op.dis() < 1e-6)return c;// 直线与圆心重合非法

		// 求解圆上的最远点
		double d = r * r / op.dis();
		Point pf = center + op / op.dis() * d;

		c.center = (center + pf) / 2;
		c.r = d / 2;

		return c;
	}

};
