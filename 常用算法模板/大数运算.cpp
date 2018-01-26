#include<stdio.h>
#include<iostream>
#include<vector>
#include<cstring>
#include<algorithm>
using namespace std;

const int N = 100000+10;
typedef long long lld;

typedef struct BigNum
{
	const static int MAXLEN = 200+10;//数字的最长长度
	int numarr[MAXLEN];//数组每一位存一个数字
	int len;//数字实际长度

    BigNum()
	{
		BigNum("0");
	}

    //以数字字符串初始化一个大数
	BigNum(const string &s)
	{
		int i=0;
		int ls=s.length();

		while(i<ls-1 && s[i]=='0')++i;

		len=0;
		for(;i<ls;++i, ++len)
		{
			numarr[len]=s[i]-'0';
		}
		//cout<<len<<endl;
		reverse(numarr, numarr+len);
	}

	BigNum(int s)
	{
	    if(s==0)
        {
            BigNum();
            return;
        }

		len=0;

		while(s)
        {
            numarr[len++]=s%10;
            s/=10;
        }
	}

	void clearLeadingZero()
	{
        while(len>1 && numarr[len-1]==0) len--;
	}

    //加法
	BigNum operator + (const BigNum &b)
	{
		BigNum res("0");

		int reallen=0;
		for(int i=0, j=0, k=0; i<this->len || j<b.len || k; ++j, ++i)
		{
			if(i<this->len)k+=numarr[i];
			if(j< b.len)k+=b.numarr[j];

			res.numarr[reallen++] = k%10;
			k/=10;
		}

		res.len = reallen;

		return res;
	}

	//加法
	BigNum operator + (const int b)
	{
		BigNum res("0");

		int reallen=0;
		for(int i=0, k=b; i<this->len || k; ++i)
		{
			if(i<this->len)k+=numarr[i];
			res.numarr[reallen++] = k%10;
			k/=10;
		}
		res.len = reallen;

		return res;
	}

    //减法
	BigNum operator - (const BigNum &b) //被减数要大于减数
	{
		BigNum res=*this;

		for(int i=0; i<b.len || i<len; ++i)
		{
		    if((i<b.len && res.numarr[i]<b.numarr[i]) || res.numarr[i]<0)
            {
                res.numarr[i]+=10;
                res.numarr[i+1]--;
            }
            if(i<b.len) res.numarr[i]-=b.numarr[i];
		}

        res.clearLeadingZero();
		return res;
	}

    //乘法
	BigNum operator * (const BigNum &b)
	{
		BigNum res("0");
		for(int i=0;i<len;++i) res = res+multiply(b, i, numarr[i]);

		return res;
	}

	BigNum multiply(const BigNum &a, int pre, int x)
	{
		BigNum res("0");
		int reallen=0;
		while(pre--) res.numarr[reallen++]=0;

		for(int i=0, k=0; i<a.len || k;++i)
		{
			if(i<a.len)k+=a.numarr[i]*x;
			res.numarr[reallen++] = k%10;
			k/=10;
		}

		res.len = reallen;
		return res;
	}

    pair<BigNum, BigNum> divAndMod(const BigNum &b)
    {
        BigNum res("0");
        BigNum mod("0");

        for(int i=this->len-1;i>=0;--i)
        {
            mod = multiply(mod, 0, 10);
            mod = mod + this->numarr[i];
            int j=0;
            for(;b<=mod;++j)
                mod = mod - b;
            res.numarr[i]=j;
        }

        res.len = this->len;
        res.clearLeadingZero();
        return {res, mod};
    }

	//除法
	BigNum operator / (const BigNum &b)
	{
        return divAndMod(b).first;
	}

	//取模
	BigNum operator % (const BigNum &b)
	{
		return divAndMod(b).second;
	}

	bool operator < (const BigNum &b) const
	{
	    if(len < b.len)return true;
	    else if(len > b.len)return false;
	    for(int i=len-1;i>=0;--i)
            if(numarr[i]<b.numarr[i])return true;
            else if(numarr[i]>b.numarr[i])return false;
	    return false;
	}

	bool operator == (const BigNum &b) const
	{
	    if(len != b.len)return false;
	    for(int i=0;i<len;++i) if(numarr[i]!= b.numarr[i])return false;
	    return true;
	}

	bool operator != (const BigNum &b) const
	{
	    return !(*this==b);
	}

	bool operator > (const BigNum &b) const
	{
	    if(*this == b)return false;
	    return !(*this<b);
	}

	bool operator <= (const BigNum &b) const
	{
	   return *this < b || *this==b;
	}

	bool operator >= (const BigNum &b) const
	{
	   return *this > b || *this==b;
	}

	void printDigits() const
	{
		int i=len;
		while(i--) cout<<numarr[i];
	}

}BigNum;

char ch[1010];
BigNum arr[1010];
int arrlen=0;

void getFibs(){
	arr[0]=BigNum("0");
	arr[1]=BigNum("1");
	arr[2]=BigNum("2");
	int i=0;
	for(i=3;;i++)
	{
		arr[i] = arr[i-1]+arr[i-2];
		if(arr[i].len>100)break;
	}
	arrlen=i;
}

int getUpper(const BigNum &b)
{
	int ans=0;
	int l=0, r=arrlen-1;

	while(l<=r)
	{
		int mid = l+r>>1;
		if(arr[mid]<b)
		{
			ans=max(ans, mid);
			l=mid+1;
		}
		else r=mid-1;
	}

	return ans;
}

void solve()
{
	getFibs();
	//cout<<arrlen;
	BigNum z("0");
	while(scanf("%s", ch)!=EOF)
	{
		BigNum a(ch);
		scanf("%s", ch);
		BigNum b(ch);
		if(a==z && b==z) break;

		int l=getUpper(a);
		int r = getUpper(b+BigNum("1"));
		printf("%d\n", r-l);
	}
}

int main()
{
    solve();
    return 0;
}

/*
10101 1023
894 341
7 5
5 2
2 1
4 10
10 3
3 1
6 8
8 9
9 1

999999999999999999999999999999999999999999999999999999999999
999999999999999999999999999999999999999999999999999999999999

*/


