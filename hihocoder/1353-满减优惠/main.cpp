#include<algorithm>
#include<iostream>
#include<string.h>
#include<cctype>
#include<stdio.h>
#include<map>
#include<climits>
#include<stack>
#include<math.h>
using namespace std;

typedef  long long lld;
const int N=10000+10;
const int MOD = 1e9+7;


int ans=INT_MAX;

int arr[N];

void dfs(int cnt, int sum, int x, int n)
{
    if(sum>=x)ans=min(ans, sum);
    if(cnt==n)return ;

    dfs(cnt+1, sum+arr[cnt], x, n);
    dfs(cnt+1, sum, x, n);
}


void solve()
{
    int n, k;
    scanf("%d%d", &n, &k);

    for(int i=0;i<n;++i)scanf("%d", arr+i);

    dfs(0, 0, k, n);

    if(ans==INT_MAX)puts("-1");
    else printf("%d\n", ans);
}

int main()
{
	solve();
	return 0;
}

/*

*/

