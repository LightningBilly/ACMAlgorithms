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
// ���ݹٷ�����ʵ��Բ�ķ���
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

	// ����Բ�ĵ�Բ���з��ݣ��õ�1��Բ
	Circle invert(const Circle& A) {
		Circle B;
		double oa = (center - A.center).dis();
		B.r = r * r / 2 * (1.0 / (oa - A.r) - 1.0 / (oa + A.r));
		double ob = r * r / (oa + A.r) + B.r;
		B.center = center + (A.center - center) * ob / oa;
		return B;
	}

	// ��Բ�ĵ�Բ���з��ݣ��õ�1��ֱ��
	Point invert2line(const Circle& c) {
		return Point();
	}
	// ���ݵ�
	Point invertPoint(const Point& p) {
		Point dir = p - center;
		double dis = dir.dis();
		dir /= dis;
		dis = r * r / dis;
		return center + dir * dis;
	}

	// ֱ�߷��ݣ��õ�Բ
	Circle invert2circle(const Line& l) {
		Point dir = l.front - l.tail;
		dir /= dir.dis();
		Circle c(Point(0, 0), 0);
		// ����ͶӰ
		Point cdir = center - l.tail;
		Point project = l.tail + dir * (dir.x * cdir.x + dir.y * cdir.y);// ��˵õ�ͶӰ����

		// ����Բ��ֱ�ߵľ���
		Point op = project - center;
		if (op.dis() < 1e-6)return c;// ֱ����Բ���غϷǷ�

		// ���Բ�ϵ���Զ��
		double d = r * r / op.dis();
		Point pf = center + op / op.dis() * d;

		c.center = (center + pf) / 2;
		c.r = d / 2;

		return c;
	}

};
