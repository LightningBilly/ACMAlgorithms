
#include <iostream>
#include<vector>
#include<string>
#include<cstring>
#include<stdio.h>
#include <queue>
#include<map>
#include <algorithm>

using namespace std;


void solve

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
