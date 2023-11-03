#include<stdio.h>
#include<cmath>
#include <algorithm>
#include <vector>


using namespace std;
const double EPS = 1e-9;
const int N = 1e5 + 10;
const int M = 1e5 + 10;

int cmp(double d) {
	if (abs(d) < EPS)return 0;
	if (d > 0)return 1;
	return -1;
}

class Point {
public:
	double x, y;

	Point() {}
	Point(double a, double b) :x(a), y(b) {}
	Point(const Point& p) :x(p.x), y(p.y) {}

	void in() {
		scanf("%lf %lf", &x, &y);
	}
	void out() {
		printf("%f %f\n", x, y);
	}

	double dis() {
		return sqrt(x * x + y * y);
	}

	double dis2() {
		return x * x + y * y;
	}

	Point operator -() const {
		return Point(-x, -y);
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

	bool operator<(const Point& a) const {
		return x < a.x || (fabs(x - a.x) < EPS && y < a.y);
	}

	bool operator==(const Point& a) const {
		return fabs(x - a.x) < EPS && fabs(y - a.y) < EPS;
	}
};

// 向量操作

double cross(const Point& a, const Point& b) {
	return a.x * b.y - a.y * b.x;
}

double dot(const Point& a, const Point& b) {
	return a.x * b.x + a.y * b.y;
}


class Point3D {
public:
	double x, y, z;

	Point3D() {}
	Point3D(double a, double b, double c) :x(a), y(b), z(c) {}
	Point3D(const Point3D& p) :x(p.x), y(p.y), z(p.z) {}

	double dis() {
		return sqrt(x * x + y * y + z * z);
	}

	double dis2() {
		return x * x + y * y + z * z;
	}

	Point3D operator -(const Point3D& p) const {
		return Point3D(x - p.x, y - p.y, z - p.z);
	}

	Point3D operator +(const Point3D& p) const {
		return Point3D(x + p.x, y + p.y, z + p.z);
	}
	Point3D operator *(double d)const {
		return Point3D(x * d, y * d, z * d);
	}

	Point3D operator /(double d)const {
		return Point3D(x / d, y / d, z / d);
	}


	void operator -=(Point3D& p) {
		x -= p.x;
		y -= p.y;
		z -= p.z;
	}

	void operator +=(Point3D& p) {
		x += p.x;
		y += p.y;
		z += p.z;
	}
	void operator *=(double d) {
		x *= d;
		y *= d;
		z *= d;
	}

	void operator /=(double d) {
		this ->operator*= (1 / d);
	}
};

// 向量操作
Point3D cross(const Point3D& a, const Point3D& b) {
	return Point3D(a.y * b.z - a.z * b.y, -a.x * b.z + a.z * b.x,
		a.x * b.y - a.y * b.x);
}

double dot(const Point3D& a, const Point3D& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}


class Line {
public:
	Point front, tail;
	Line() {}
	Line(Point a, Point b) :front(a), tail(b) {}
};

/*
0 不相交
1 相交
0 平行/重合
*/
int cross(const Line& a, const Line& b) {
	Point dir1 = a.front - a.tail;
	Point dir2 = b.front - b.tail;
	if (cmp(cross(dir1, dir2)) == 0) {
		return 0;
	}

	if (cmp(cross(a.front - b.tail, dir2)) * cmp(cross(a.tail - b.tail, dir2)) >= 0)return 0;
	if (cmp(cross(b.front - a.tail, dir1)) * cmp(cross(b.tail - a.tail, dir1)) >= 0)return 0;
	return 1;
}


int inCircle(Point p0, Point p1, Point p2, Point p3) {
	Point d1 = p1 - p0;
	Point d2 = p2 - p0;
	if (cross(d1, d2) < 0)return inCircle(p0, p2, p1, p3); // 保证平面法向向上

	// 构建映射点
	Point3D lift0(p0.x, p0.y, p0.dis2());
	Point3D lift1(p1.x, p1.y, p1.dis2());
	Point3D lift2(p2.x, p2.y, p2.dis2());
	Point3D lift3(p3.x, p3.y, p3.dis2());

	Point3D z1(lift1 - lift0), z2(lift2 - lift0);
	Point3D normal = cross(z1, z2); // 计算平面法向
	double project = dot(normal, lift3 - lift0); // 计算点到平面距离

	return cmp(project);
}


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

class Edge {
public:
	Edge() {}
	Edge(int t, int n, double w) :to(t), next(n), weight(w) {}
	int index;
	int from;
	int to;
	int next;
	double weight;
	bool isValid() const { return to >= 0; }
};

class Graph {
public:
	int size;
	Graph() {}
	void init(int n) {
		size = n;
		head.assign(n, -1);
		edge.resize(M * 2);
		len = 0;
		emptyEdge = Edge(-1, -1, 0);
	}

	Edge headEdge(int a) {
		if (head[a] < 0)return emptyEdge;
		return edge[head[a]];
	}

	Edge nextEdge(const Edge& ed) {
		if (ed.next < 0)return emptyEdge;
		return edge[ed.next];
	}

	void add(int a, int b, double w) {
		//printf("add : %d %d %.6lf\n", a, b, w);
		edge[len] = Edge(b, head[a], w);
		edge[len].index = len;
		head[a] = len;
		len++;
	}

	void del(int ind) {
		edge[ind].to = -1;
	}

private:
	vector<int> head;
	int len;
	Edge emptyEdge;
	vector<Edge> edge;
};

Graph g;

/*
输入点集
输出图
*/
class Delaunay {
	const vector<Point>& Points;
	vector<int> Index;
	vector<int> IndexInverse;
	Graph gtmp;
public:
	Delaunay(const vector<Point>& ps) :Points(ps) {
		gtmp.init(Points.size());
	}

	vector<Edge> build() {
		for (int i = 0; i < Points.size(); ++i) Index.push_back(i);
		sort(Index.begin(), Index.end(), [=](int a, int b)->bool {
			if (abs(Points[a].x - Points[b].x) > 1e-6)return Points[a].x < Points[b].x;
			return Points[a].y < Points[b].y;
			}); // 按照坐标排序
		/*for (int i = 0; i < Points.size(); ++i) {
			printf("%lf %lf\n", Points[Index[i]].x, Points[Index[i]].y);
		}*/
		IndexInverse.resize(Points.size());
		for (int i = 0; i < Index.size(); ++i) {
			IndexInverse[Index[i]] = i;
		}
		devide(0, Points.size() - 1);
		//g.init(Points.size());
		//puts("init");
		vector<Edge> edges;
		// 删除杂边
		for (int i = 0; i < gtmp.size; ++i) {
			for (Edge ed = gtmp.headEdge(i); ed.isValid(); ed = gtmp.nextEdge(ed)) {
				if (ed.to < 0) continue;
				//g.add(i, ed.to, ed.weight);
				ed.from = i;
				edges.push_back(ed);
			}
		}

		return edges;
	}

	void devide(int l, int r) {
		if (r - l < 2) {
			for (int i = l; i <= r; ++i) {
				for (int j = l; j <= r; ++j) {
					if (i == j)continue;
					gtmp.add(Index[i], Index[j], (Points[Index[i]] - Points[Index[j]]).dis());
				}
			}
			return;
		}
		int mid = (l + r) >> 1;
		devide(l, mid);
		devide(mid + 1, r);

		int bl = l, br = r;
		int update = 1;
		for (; update;) {
			update = 0;
			// 检查右边点是否有在底边左下方
			for (Edge ed = gtmp.headEdge(Index[bl]); ed.isValid(); ed = gtmp.nextEdge(ed)) {
				if (ed.to < 0)continue;
				auto p0 = Points[Index[bl]];
				auto p1 = Points[Index[br]];
				auto p2 = Points[ed.to];
				auto c = cmp(cross(p1 - p0, p2 - p0));
				if (c < 0 || c == 0 && (p2.x > p0.x || cmp(p2.x - p0.x) == 0 && p2.y > p0.y)) {
					update = 1;
					bl = IndexInverse[ed.to];
					break;
				}
			}

			if (update)continue;
			// 检查左边点是否有在底边左下方
			for (Edge ed = gtmp.headEdge(Index[br]); ed.isValid(); ed = gtmp.nextEdge(ed)) {
				if (ed.to < 0)continue;
				auto p0 = Points[Index[bl]];
				auto p1 = Points[Index[br]];
				auto p2 = Points[ed.to];
				auto c = cmp(cross(p1 - p0, p2 - p0));
				if (c < 0 || c == 0 && (p2.x < p1.x || cmp(p2.x - p0.x) == 0 && p2.y < p1.y)) {
					update = 1;
					br = IndexInverse[ed.to];
					break;
				}
			}
		}

		// 添加一条边
		gtmp.add(Index[br], Index[bl], (Points[Index[br]] - Points[Index[bl]]).dis());
		gtmp.add(Index[bl], Index[br], (Points[Index[br]] - Points[Index[bl]]).dis());

		// 查找新加的边
		for (; true;) {
			Point baseDir = Points[Index[br]] - Points[Index[bl]];
			int side = 0, adp = -1;
			// 查找左边
			for (Edge ed = gtmp.headEdge(Index[bl]); ed.isValid(); ed = gtmp.nextEdge(ed)) {
				if (ed.to < 0)continue;
				if (cmp(cross(baseDir, Points[ed.to] - Points[Index[bl]])) <= 0)continue; // 是否小于180度
				if (adp == -1 || inCircle(Points[Index[br]], Points[Index[bl]], Points[adp], Points[ed.to]) < 0) { // 判断空圆性
					adp = ed.to;
					side = -1;
				}
			}

			// 查找右边
			for (Edge ed = gtmp.headEdge(Index[br]); ed.isValid(); ed = gtmp.nextEdge(ed)) {
				if (ed.to < 0)continue;
				if (cmp(cross(Points[ed.to] - Points[Index[br]], -baseDir)) <= 0)continue; // 是否小于180度
				if (adp == -1 || inCircle(Points[Index[br]], Points[Index[bl]], Points[adp], Points[ed.to]) < 0) { // 判断空圆性
					adp = ed.to;
					side = 1;
				}
			}

			if (adp < 0)break;
			// 新选择的点在左边
			if (side < 0) {
				// 删除相交的边
				for (Edge ed = gtmp.headEdge(Index[bl]); ed.isValid(); ed = gtmp.nextEdge(ed)) {
					if (ed.to < 0)continue;
					if (ed.to == adp || ed.to == Index[bl] || ed.to == Index[br])continue; // 被选中的点忽略
					if (cmp(cross(baseDir, Points[ed.to] - Points[Index[bl]])) <= 0)continue; // 是否小于180度
					// 判断线段相交
					if (cross(Line(Points[ed.to], Points[Index[bl]]), Line(Points[Index[br]], Points[adp])) > 0) {
						gtmp.del(ed.index);
						gtmp.del(ed.index ^ 1);
					}
				}
				gtmp.add(adp, Index[br], (Points[adp] - Points[Index[br]]).dis());
				gtmp.add(Index[br], adp, (Points[adp] - Points[Index[br]]).dis());
				bl = IndexInverse[adp]; // 更新baseline 左点
			}
			else {// 新选择的点在右边
				// 删除相交的边
				for (Edge ed = gtmp.headEdge(Index[br]); ed.isValid(); ed = gtmp.nextEdge(ed)) {
					if (ed.to < 0)continue;
					if (ed.to == adp || ed.to == Index[bl] || ed.to == Index[br])continue; // 被选中的点忽略
					if (cmp(cross(Points[ed.to] - Points[Index[br]], -baseDir)) <= 0)continue; // 是否小于180度
					// 判断线段相交
					if (cross(Line(Points[ed.to], Points[Index[br]]), Line(Points[Index[bl]], Points[adp])) > 0) {
						gtmp.del(ed.index);
						gtmp.del(ed.index ^ 1);
					}
				}

				gtmp.add(adp, Index[bl], (Points[adp] - Points[Index[bl]]).dis());
				gtmp.add(Index[bl], adp, (Points[adp] - Points[Index[bl]]).dis());
				br = IndexInverse[adp]; // 更新baseline 右点
			}
		}
	}
};



class UnionFindSet {
private:
	vector<int> father; // 父结点定义，father[i]=i时，i为本集合的代表
	vector<int> height; // 代表树高度，初始为1
	int nodeNum; // 集合中的点数

public:
	UnionFindSet(int n); // 初始化
	bool Union(int x, int y); // 合并
	int Find(int x);

	bool UnionV2(int x, int y); // 合并
	int FindV2(int x);
};

UnionFindSet::UnionFindSet(int n) : nodeNum(n + 1) {
	father = vector<int>(nodeNum);
	height = vector<int>(nodeNum);
	for (int i = 0; i < nodeNum; ++i) father[i] = i, height[i] = 1; // 初始为自己
}

int UnionFindSet::Find(int x) {
	while (father[x] != x) x = father[x];
	return x;
}

bool UnionFindSet::Union(int x, int y) {
	x = Find(x);
	y = Find(y);

	if (x == y)return false;
	father[x] = y;
	return true;
}


int UnionFindSet::FindV2(int x) {
	int root = x; // 保存好路径上的头结点
	while (father[root] != root) {
		root = father[root];
	}
	/*
	从头结点开始一直往根上遍历
	把所有结点的father直接指向root。
	*/
	while (father[x] != x) {
		// 一定要先保存好下一个结点，下一步是要对father[x]进行赋值
		int temp = father[x];
		father[x] = root;
		x = temp;
	}

	return root;
}

/*
需要加入height[]属性，初始化为1.
*/
//合并结点
bool UnionFindSet::UnionV2(int x, int y) {
	x = Find(x);
	y = Find(y);
	if (x == y) {
		return false;
	}
	if (height[x] < height[y]) {
		father[x] = y;
	}
	else if (height[x] > height[y]) {
		father[y] = x;
	}
	else {
		father[x] = y;
		height[y]++;
	}
	return true;
}

// 最小生成树
class MST {
	vector<Edge> edges;
	UnionFindSet ufs;

public:
	MST(int n, vector<Edge>& ed) :ufs(n), edges(ed) { g.init(n); }
	void build() {
		sort(edges.begin(), edges.end(), [=](Edge& a, Edge& b)->bool {return a.weight < b.weight; });
		for (auto d : edges) {
			if (ufs.UnionV2(d.from, d.to)) {
				g.add(d.from, d.to, d.weight);
				g.add(d.to, d.from, d.weight);
			}
		}
	}
};

const int bitL = 22;
int h[N];
int father[bitL][N];
double dis[bitL][N];

void dfs(int x, int fa, double w)
{
	if (fa == -1) h[x] = 0;
	else {
		h[x] = h[fa] + 1;
		father[0][x] = fa;
		dis[0][x] = w;
		for (int t = 1; t < bitL && (1 << t) <= h[x]; t++) {
			father[t][x] = father[t - 1][father[t - 1][x]];
			dis[t][x] = max(dis[t - 1][x], dis[t - 1][father[t - 1][x]]);
		}
	}

	for (auto ed = g.headEdge(x); ed.isValid(); ed = g.nextEdge(ed))
	{
		int j = ed.to;
		if (fa == j)continue;
		dfs(j, x, ed.weight);
	}
}

int lca(int a, int b) {
	if (h[a] < h[b]) {
		return lca(b, a);
	}

	int gap = h[a] - h[b];
	for (int t = bitL - 1; t >= 0; t--) {
		if (gap & (1 << t))a = father[t][a];
	}
	if (a == b)return a;
	gap = h[a];
	for (int t = bitL - 1; t >= 0; t--) {
		if (gap <= (1 << t))continue;
		if (father[t][a] == father[t][b])continue;
		a = father[t][a];
		b = father[t][b];
		gap -= 1 << t;
	}

	return father[0][a];
}


double optDis(int a, int b) {
	if (h[a] < h[b]) {
		return optDis(b, a);
	}
	double d = 0;
	int gap = h[a] - h[b];
	for (int t = bitL - 1; t >= 0; t--) {
		if (gap & (1 << t)) {
			d = max(d, dis[t][a]);
			a = father[t][a];
		}
	}
	if (a == b)return d;
	gap = h[a];
	for (int t = bitL - 1; t >= 0; t--) {
		if (gap <= (1 << t))continue;
		if (father[t][a] == father[t][b])continue;
		d = max(d, dis[t][a]);
		d = max(d, dis[t][b]);
		a = father[t][a];
		b = father[t][b];
		gap -= 1 << t;
	}
	d = max(d, max(dis[0][a], dis[0][b]));
	return d;
}

void  solve() {
	vector<Point> ps;

	int n, m;

	scanf("%d", &n);
	ps.resize(n);
	int a, b;
	for (int i = 0; i < n; ++i) {
		scanf("%d%d", &a, &b);
		ps[i].x = a;
		ps[i].y = b;
	}

	Delaunay de(ps);
	auto edges = de.build();

	//for (int i = 0; i < n; ++i) {
	//	printf("%d : ", i);
	//	for (Edge ed = g.headEdge(i); ed.isValid(); ed = g.nextEdge(ed)) {
	//		printf("%d ", ed.to);
	//	}
	//	puts("");
	//}
	MST mst(n, edges);
	mst.build();

	dfs(0, -1, 0);
	scanf("%d", &m);

	while (m--) {
		scanf("%d%d", &a, &b);
		a--, b--;
		printf("%.11f\n", optDis(a, b));
	}
}

int main() {
	solve();
	return 0;

}

/*
3
0 0
50 10
150 0
3
1 2
1 3
2 3

4
0 1
1 0
1 1
0 0

4
1 2
2 3
4 1
1 3

4
0 0
0 1
1 0
1 1

4
1 2
2 3
4 1
1 3

6
0 0
0 1
0 2
0 3
0 4
0 5

4
1 2
2 3
4 1
1 3

6
0 0
1 0
2 0
3 0
4 0
5 0

4
1 2
2 3
4 1
1 3

10
0 0
1 1
1 2
2 1
2 3
2 9
3 2
4 6
7 9
9 8

10
1 2
2 3
3 4
4 5
5 6
6 7
7 8
8 9
9 10
10 1


7
0 0
1 1
1 2
2 1
2 3
2 9
3 2

1
1 2

*/