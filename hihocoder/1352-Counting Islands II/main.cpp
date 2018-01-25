#include<algorithm>
#include<iostream>
#include<string.h>
#include<cctype>
#include<stdio.h>
#include<map>
#include<vector>
#include<stack>
#include<math.h>
using namespace std;

typedef  long long lld;

const int N = 1e6+10;
const int M = 1e9+7;

int father[N];

int findFather(int x)
{
    int s=x;

    while(x!=father[x])x=father[x];

    while(x!=father[s])
    {
        int i=s;
        s=father[s];
        father[i]=x;
    }

    return x;
}

void join(int x, int y)
{
    x=findFather(x);
    y=findFather(y);

    if(x==y)return;
    if(x<y)swap(x, y);

    father[x]=y;
}

int dir[4][2]={{1,0}, {-1, 0}, {0, 1}, {0, -1}};

void solve()
{
    int n;
    scanf("%d", &n);

    for(int i=0;i<N;++i) father[i]=-1;

    int sum=0;
    while(n--)
    {
        int a,b;
        scanf("%d%d", &a, &b);

        int ind = a*1000+b;

        if(father[ind]==-1)
        {
            sum++;
            father[ind]=ind;
        }
        else
        {
            //printf("%d\n", sum);
            //continue;
        }

        for(int i=0;i<4;++i)
        {
            int a1=a+dir[i][0];
            int b1=b+dir[i][1];

            if(a1<0 || a1>=1000 || b1<0 || b1>=1000)continue;

            int ni = a1*1000+b1;
            if(father[ni]==-1)continue;

            if(findFather(ind)==findFather(ni))continue;
            join(ind, ni);
            sum--;
        }

        printf("%d\n", sum);
    }
}

int main()
{
	solve();
	return 0;
}

/*
test cases:

3
0 0
1 1
1 0

2
1 1
1 1

3
1 1
1 0
0 0
*/

