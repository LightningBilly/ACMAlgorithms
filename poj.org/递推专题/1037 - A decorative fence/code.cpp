#include<stdio.h>
#include<iostream>
#include<vector>
#include<cstring>
#include<algorithm>
using namespace std;

const int N = 20+2;
typedef long long lld;

lld dp[N][N][2]={0};

void getDps()
{
    dp[1][1][0]=dp[1][1][1]=1;
    for(int j=2;j<N;++j)
    {
        for(int i=1;i<=j;++i)
        {
            for(int s=i;s<j;++s) dp[i][j][0]+=dp[s][j-1][1];
            for(int s=1;s<i;++s) dp[i][j][1]+=dp[s][j-1][0];
            //printf("%lld ", dp[i][j][0]+dp[i][j][1]);
        }

        //puts("");

    }

}

bool vis[N]={0};
int ans[N];

void getAns(int n, lld c)
{
    if(n==1) {
        ans[0]=1;
        return;
    }

    memset(vis, 0, sizeof(vis));
    //查找第一个
    int ind=0;
    for(int i=1;i<=n;++i)
    {
        lld d=dp[i][n][0]+dp[i][n][1];
        if(d>=c)
        {
            vis[i]=1;
            ans[ind++]=i;
            break;
        }
        else c-=d;
    }

    for(int i=n-1;i>0;i--)
    {
        int cnt=0;//计数有效数字中的第几个了
        for(int j=1;j<=n;++j)
        {
            if(vis[j])continue;
            cnt++;
            int d=j>ans[ind-1];
            if(ind>1 && ((ans[ind-1]>ans[ind-2])^d)==0)continue;
            lld countleft = dp[cnt][i][d];
            if(countleft>=c)
            {
                vis[j]=1;
                ans[ind++]=j;
                break;
            }
            else c-=countleft;
        }
    }
}


void solve()
{
    int k,n;
    lld c;
    scanf("%d", &k);
    getDps();
    while(k--)
    {
        scanf("%d%lld", &n, &c);
        getAns(n, c);
        for(int i=0;i<n;++i)
        {
            if(i)putchar(' ');
            printf("%d", ans[i]);
        }
        puts("");
    }
}

int main()
{
    solve();
    return 0;
}

/*
10
4 1
4 2
4 3
4 4
4 5
4 6
4 7
4 8
2 1
3 3

*/
