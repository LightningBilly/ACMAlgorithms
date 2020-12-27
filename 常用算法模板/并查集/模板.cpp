
#include <iostream>
#include<vector>
#include<string>
#include<cstring>
#include<stdio.h>
#include <queue>
#include<map>
#include <algorithm>

using namespace std;


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
    while(father[x] != x) {
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
    } else if (height[x] > height[y]) {
        father[y] = x;
    } else {
        father[x] = y;
        height[y]++;
    }
    return true;
}

class IndexMaker {
private:
    int curUsedIndex;
    map<string, int> indMap;
public:
    IndexMaker();

    int GetIndByName(string name);
};

IndexMaker::IndexMaker() : curUsedIndex(0) {
    indMap.clear();
}

int IndexMaker::GetIndByName(string name) {
    if (indMap.count(name) == 0) indMap[name] = curUsedIndex++;
    return indMap[name];
}

int main() {

    int n;
    cin >> n;
    UnionFindSet us(2 * n); // 最多有可能2n个人

    string a, b;
    int o;
    IndexMaker indMaker;

    for (int i = 0; i < n; ++i) {
        cin >> o >> a >> b;
        int indexa = indMaker.GetIndByName(a), indexb = indMaker.GetIndByName(b);
        if (o == 0) us.UnionV2(indexa, indexb);
        else {
            int roota = us.FindV2(indexa), rootb = us.FindV2(indexb);
            string ans = roota == rootb ? "yes" : "no";
            cout << ans << endl;
        }
    }
}


/**

10
0 Steven David
0 Lcch Dzx
1 Lcch Dzx
1 David Dzx
0 Lcch David
0 Frank Dzx
1 Steven Dzx
1 Frank David
0 Steven Dzx
0 Dzx Frank
样例输出
yes
no
yes
yes

*/
