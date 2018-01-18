#include<stdio.h>
#include<cstring>
#include<string>
#include<vector>
#include<iostream>
#include<algorithm>
using namespace std;


const int N=4000+10;
const int mod = 1000000007;
typedef long long lld;

int ans[N]={-1};

bool canDivid(int counts, int n)
{
    for(int i=0;i<n;++i)
    {
        if(counts%n!=1)return false;
        counts--;
        counts-=counts/n;
    }

    return counts%n==0;
}

int dp(int n)
{
    if(n==0)return 0;
    //if(ans[n]!=-1)return ans[n];
    int k=0;

    for(int i=n;i>0;--i)
    {
        if(canDivid(n, i))
        {
            k=i;
            break;
        }
    }

    return k;
}

void solve()
{
	int n;
	memset(ans, -1, sizeof(ans));

	for(int i=0;i<=3121;++i) dp(i);
	//cout<<ans[10]<<endl;
	while(scanf("%d", &n)!=EOF && n>=0)
    {
        int k=dp(n);
        if(k==0)
        {
            printf("%d coconuts, no solution\n", n);
        }
        else printf("%d coconuts, %d people and 1 monkey\n", n, k);
        //printf("%d\n", dp(n));
    }
}

int main()
{
	solve();
	return 0;
}
