#include<stdio.h>
#include<cstring>
#include<string>
#include<iostream>
#include<algorithm>
using namespace std;


const int N=1000+10;
typedef long long lld;

typedef struct BigNum
{
	const static int MAXLEN = 400+10;//数字的最长长度
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
		reverse(numarr, numarr+len);
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
        return make_pair(res, mod);
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
		//cout<<i<<endl;
		while(i--) cout<<numarr[i];
	}

}BigNum;

BigNum arr[N];

void getFibs()
{
	arr[0]=BigNum("1");
	BigNum one = BigNum("1");

	for(int i=1;i<N;++i)
	{
		arr[i] = arr[i-1]+arr[i-1]+one;
	}
}

int list[N];

void solve()
{
	getFibs();
	
	int n;
	while(scanf("%d", &n)!=EOF)
	{
		for(int i=0;i<n;++i)
		{
			scanf("%d", list+i);
		}

		int cnt=0;
		BigNum ans("0");

		for(int i=n-1;i>=0;--i)
		{
			if(list[i]==1)
			{
				cnt++;
				if(cnt%2==0) ans = ans - arr[i];
				else ans = ans + arr[i];
			}
		}
		ans.printDigits();
		puts("");
	}
}

int main()
{
	solve();
	return 0;
}
