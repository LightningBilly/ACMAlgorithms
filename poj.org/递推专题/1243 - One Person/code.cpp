#include<stdio.h>
#include<iostream>
#include<vector>
#include<cstring>
#include<algorithm>
using namespace std;

const int N = 30+10;
typedef long long lld;

int dp[N][N]={0};

void getAns()
{
    for(int i=0;i<N;++i)
    {
        for(int j=1;j<N;++j)
        {
            if(i==0) dp[i][j]=j;
            else dp[i][j] = dp[i][j-1]+ 1 +dp[i-1][j-1];
        }
    }
}

void solve()
{
    int l, g;
    int ca=1;
    getAns();
    while(scanf("%d%d", &g, &l)!=EOF && g+l)
    {
        printf("Case %d: ", ca++);
        printf("%d\n", dp[l][g]);
    }
}

int main()
{
    solve();
    return 0;
}

/*
10 10
30 30
30 29
29 29
0 5
10 29
29 20


*/
