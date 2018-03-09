#include<stdio.h>
#include<iostream>
#include<vector>
#include <stdlib.h>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;

const int N = 1e5+10;
const int M = 10000000;
typedef long long lld;

lld arr[2*N];

lld getAns(lld n)
{
    sort(arr, arr+n);
    for(int i=0;i<n;++i)
    {
        arr[i+n]=arr[i]+M;
    }

    if(n==1)return 0;
    else if(n==2)
    {
        return min(arr[1]-arr[0], arr[2]-arr[1]);
    }
    int s = n/2;

    lld ans=0, t=0;
    for(int i=0;i<n;++i) {
        if(i<s)t+= arr[s]-arr[i];
        else t+= arr[i]-arr[s];
    }
    ans = t;
    //cout<<t<<endl;
    for(int i=n;i<2*n;++i, s++)
    {
        t += arr[i] - arr[s] - (arr[s]-arr[i-n]);
        t += (n/2)*(arr[s+1]-arr[s]) - ((n-1)/2+1)*(arr[s+1]-arr[s]);
        ans=min(t, ans);
        //cout<<t<<endl;
    }

    return ans;
}


void solve()
{
    int n;
    scanf("%d", &n);
    for(int i=0;i<n;++i)
    {
        scanf("%lld", arr+i);
    }

    lld ans = getAns(n);
    printf("%lld\n", ans);
}

int main()
{
    solve();
    return 0;
}

/*
4
9999999
7
16
9999995

1
1
2
9999999
10

2
5
10

3
1 100000 1415235



*/
