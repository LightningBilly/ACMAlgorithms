#include<algorithm>
#include<iostream>
#include<string.h>
#include<cctype>
#include<stdio.h>
#include<unordered_map>
#include<map>
#include<vector>
#include<stack>
#include<math.h>
using namespace std;

typedef long long lld;

const int N = 110;
lld dp[2*N][2*N][N]={0};


void solve()
{
	bool used[300]={0};
	used['J']=1;
	char ch[300];

	scanf("%s", ch);
	char ans[100];
	int l=0;
	for(int i=0;ch[i];i++)
	{
		if(ch[i]=='J')ch[i]='I';
		if(used[ch[i]])continue;
		used[ch[i]]=1;

		ans[l++]=ch[i];
	}

	for(int i='A';i<='Z';++i)
	{
		if(used[i])continue;
		ans[l++]=i;
	}

	for(int i=0;i<25;++i)
	{
		putchar(ans[i]);
		if((i+1)%5==0)puts("");
	}
}

int main()
{
	solve();
	return 0;
}

/*
4 4
0 0
1 0
1 1
0 1

4 3
0 0
1 0
1 1
0 1

5 3
0 0
1 0
2 1
1 2
0 1

5 5
0 0
1 0
2 1
1 2
0 1
*/
