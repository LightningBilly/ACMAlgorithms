
#include <iostream>
#include<vector>
#include<string>
#include<stack>
#include<stdio.h>
#include<unordered_map>
#include<set>
#include<algorithm>

using namespace std;

typedef long long lld;

const lld M = 1e6+7;

lld arr[M];

/*
a*x + b*y = 1
b*x1 + a%b*y1 = 1

(a-(a/b)*b)*y1 + b*x1 = 1
a*y1 + b*(x1-(a/b)*y1) = 1

 */

int e_gcd(int a, int b, int &x, int &y) {
    if(b==0) {
        x=1;
        y=0;
        return a;
    }

    int ans = e_gcd(b, a%b, x, y);
    int temp = x;
    x=y;
    y=temp - a/b*y;
    return ans;
}

// 快速幂
int fastPow(int a, int n, int mod) {
    int ans = 1;
    while(n) {
        if(n&1){
            ans = ans*a%mod;
        }
        // cout<<"ans: "<<ans<<endl;
        a=a*a%mod;
        n>>=1;
    }

    return ans;
}

int main() {

    int p,q,e,m,c;
    cin>>p>>q>>e>>m;
    int n = p*q;
    int fn = (p-1)*(q-1);

    int d, d2;
    e_gcd(e, fn, d, d2);
    d = (d+fn)%fn;
    cout<<d<<endl;
    c = fastPow(m, e, n);
    cout<<"encode:"<<c<<endl;

    c = fastPow(c, d, n);

    cout<<"decode:"<<c<<endl;

    return 0;
}

/*
17 11 7 123
17 31 7 2


*/

