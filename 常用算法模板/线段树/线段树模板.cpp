
#include <iostream>
#include<vector>
#include<string>
#include<cstring>
#include<stdio.h>
#include<map>
#include<set>
#include<algorithm>

using namespace std;

#define lson(x) (x<<1)
#define rson(x) (x<<1|1)

const int M = 1e5+10;

class Node {
public:
    int left, right, mid;
    int sumVal;
    int lazyPrice;

    Node() {
    }
};

Node node[M*4];
class segmentTree {
private:
    void buildTree(int ind, int l, int r) {
        node[ind].left=l;
        node[ind].right=r;
        node[ind].sumVal = 0;
        node[ind].lazyPrice = 0;
        node[ind].mid = l + r >> 1;
        if (l == r) {
            return;
        }

        buildTree(lson(ind), l, node[ind].mid);
        buildTree(rson(ind), node[ind].mid + 1, r);
    }


    void pushDown(int ind) {
        if (node[ind].lazyPrice == 0) return;

        this->updateTree(lson(ind), node[lson(ind)].left, node[lson(ind)].right, node[ind].lazyPrice);
        this->updateTree(rson(ind), node[rson(ind)].left, node[rson(ind)].right, node[ind].lazyPrice);
        node[ind].lazyPrice = 0;
    }

public:
    segmentTree(int left, int right) {
        buildTree(1, left, right);
    }

    void updateTree(int ind, int l, int r, int val) {
        if (l > node[ind].right || r < node[ind].left)return;

        if (l <= node[ind].left && node[ind].right <= r) {
            node[ind].sumVal = val * (node[ind].right - node[ind].left + 1);
            node[ind].lazyPrice = val;
            return;
        }

        pushDown(ind);

        updateTree(lson(ind), l, r, val);
        updateTree(rson(ind), l, r, val);

        node[ind].sumVal = node[lson(ind)].sumVal + node[rson(ind)].sumVal;
    }

    int query(int ind, int l, int r) {

        if (l > node[ind].right || r < node[ind].left)return 0;

        if (l <= node[ind].left && node[ind].right <= r) {
            return node[ind].sumVal;
        }

        pushDown(ind);
        return query(lson(ind), l, r) + query(rson(ind), l, r);
    }
};


int main() {
    int T, N, Q;
    cin >> T;
    for (int k = 1; k <= T; k++) {
        cin >> N >> Q;
        segmentTree seg = segmentTree(1, N);
        seg.updateTree(1, 1, N, 1);
        // cout << "lq1:" << seg.query(1, 1, N) << endl;
        while (Q--) {
            int x, y, z;
            scanf("%d%d%d", &x, &y, &z);
            // cout << x << "-" << y << "-" << z << endl;
            seg.updateTree(1, x, y, z);
            // cout << "lq:" << seg.query(1, 1, N) << endl;
        }

        printf("Case %d: The total value of the hook is %d.\n", k, seg.query(1, 1, N));
    }

    return 0;
}

/*
2
10
2
1 5 2
5 9 3
10
2
1 5 2
5 9 3
*/

