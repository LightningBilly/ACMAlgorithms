

#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<cmath>
#include<algorithm>

using namespace std;

const int M = 500000 + 10;
const int N = 500000 + 10;
const int bitL = 22;
int head[N];
int to[M * 2], nextEdge[M * 2], weight[M * 2];
int len;
int h[N];
int father[bitL][N];
int dis[bitL][N];

void initPara(int n)
{
	len = 0;
	for (int i = 0; i < n; i++)
	{
		head[i] = -1;
	}
}

void add(int a, int b, int w)
{
	to[len] = b;
	weight[len] = w;
	nextEdge[len] = head[a];

	head[a] = len++;
}

void dfs(int x, int fa, int w)
{
	if (fa == -1) h[x] = 0;
	else {
		h[x] = h[fa] + 1;
		father[0][x] = fa;
		dis[0][x] = w;
		for (int t = 1; t < bitL && (1 << t) <= h[x]; t++) {
			father[t][x] = father[t - 1][father[t - 1][x]];
			dis[t][x] = dis[t - 1][x] + dis[t - 1][father[t - 1][x]];
		}
	}
	int i;
	for (i = head[x]; i != -1; i = nextEdge[i])
	{
		int j = to[i];
		if (fa == j)continue;
		dfs(j, x, weight[i]);
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


int optDis(int a, int b) {
	if (h[a] < h[b]) {
		return optDis(b, a);
	}
	int d = 0;
	int gap = h[a] - h[b];
	for (int t = bitL - 1; t >= 0; t--) {
		if (gap & (1 << t)) {
			d += dis[t][a];
			a = father[t][a];
		}
	}
	if (a == b)return d;
	gap = h[a];
	for (int t = bitL - 1; t >= 0; t--) {
		if (gap <= (1 << t))continue;
		if (father[t][a] == father[t][b])continue;
		d += dis[t][a];
		d += dis[t][b];
		a = father[t][a];
		b = father[t][b];
		gap -= 1 << t;
	}
	d += dis[0][a] + dis[0][b];
	return d;
}

void solve()
{
	int t;
	int n, m;
	int a, b, w;
	scanf("%d", &t);
	while (t--) {
		scanf("%d%d", &n, &m);
		initPara(n);
		for (int i = 0; i < n - 1; ++i) {
			scanf("%d%d%d", &a, &b, &w);
			a--, b--;
			add(a, b, w);
			add(b, a, w);
		}

		dfs(0, -1, 0);

		/*for (int i = 0; i < n; ++i) {
			printf("%d: %d\n", i, h[i]);
		}*/

		while (m--) {
			scanf("%d%d", &a, &b);
			a--, b--;
			printf("%d\n", optDis(a, b));
		}
	}
}

void test() {
	int t;
	int n = 5000, m = 500000;

	//scanf("%d%d%d", &n, &m, &s);
	initPara(n);
	int a, b;
	for (int i = 0; i < n - 1; ++i) {
		a = i, b = i + 1;
		add(a, b, 1);
		add(b, a, 1);
	}

	dfs(0, -1, 0);
	//printf("%d\n", 1 + lca(10, 5000-1));
	while (m--) {
		a = (m + 102) % n, b = (3823 + m * 2) % n;
		//printf("%d\n", m);
		if (lca(a, b) != min(a, b))
			printf("%d %d %d\n", 1 + lca(a, b), a + 1, b + 1);
	}
}

int main()
{
	solve();
	//test();
	return 0;
}



/*
2
3 2
1 2 10
3 1 15
1 2
2 3

2 2
1 2 100
1 2
2 1

1
12 11
8 1 4
8 9 3
8 12 6
1 5 5
1 7 1
7 6 2
9 4 2
9 11 10
9 2 9
4 3 2
12 10 7

1 2
2 3
3 4
4 5
5 6
6 7
7 8
8 9
9 10
10 11
11 12

1
12 11
8 1 1
8 9 1
8 12 1
1 5 1
1 7 1
7 6 1
9 4 1
9 11 1
9 2 1
4 3 1
12 10 1

1 2
2 3
3 4
4 5
5 6
6 7
7 8
8 9
9 10
10 11
11 12
*/