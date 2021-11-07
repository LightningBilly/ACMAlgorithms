文章目录

[TOC]

# 零、简介

RMQ-ST(Range Minimum/Maximum Query - Spare Table) 是指区间最大/最小值查询，稀疏表法。是典型的利用空间换时间的算法。

其原理是先利用动态规划求解出所有 i^2 长度的区间最值，然后利用二分思想+最值比较性质在O(1)时间求解出区间最值。其稀疏性就体现在只计算i^2长度的区间，并不计算所有长度的区间值。

本文从朴素分组算法出发，引出动态规划方法，最后再优化空间复杂度引出RMQ-ST，图文结合，讲解原理。

# 一、空间换时间思想

先来看一个经题目

区间中最大的数

链接：http://www.51nod.com/Challenge/Problem.html#problemId=1174

## 题目大意

给出一个有N个数的序列，编号0 - N - 1。进行Q次查询，查询编号i至j的所有数中，最大的数是多少。

例如: 1 7 6 3 1。i = 1, j = 3，对应的数为7 6 3，最大的数为7。（该问题也被称为RMQ问题）

 收起

### 输入

```
第1行：1个数N，表示序列的长度。(2 <= N <= 10000)
第2 - N + 1行：每行1个数，对应序列中的元素。(0 <= S[i] <= 10^9)
第N + 2行：1个数Q，表示查询的数量。(2 <= Q <= 10000)
第N + 3 - N + Q + 2行：每行2个数，对应查询的起始编号i和结束编号j。(0 <= i <= j <= N - 1)
```

### 输出

```
共Q行，对应每一个查询区间的最大值。
```

### 输入样例

```
5
1
7
6
3
1
3
0 1
1 3
3 4
```

### 输出样例

```
7
7
3
```



## 解法一 朴素做法

每次查询遍历区间值。

复杂度：每次查询O(n) 总体复杂度O(n^2)。

结论：复杂度过高。



## 解法二 分组计算

我们把数组按连续的a个数字进行分组。每组的最大值用Max[i]表示。

展开如下

![分组](file:///Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/RMQ-ST/分组.png)

求解区间最值
$$
求解区间和 maxRange(i,j) = 
max(\ max^{\lfloor i/a \rfloor * a+a-1}_{x=i}arr[x], \ \ max^{ \lfloor j/a \rfloor -1 }_{x=\lfloor i/a \rfloor +1}Max[x], \ \ max^{j}_{x=\lfloor j/a \rfloor * a}arr[x]\ )
$$

![求和过程](file:///Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/RMQ-ST/求值过程.png)从上式可以看出，求和复杂度是分组组数+每组个数 = O(len(arr)/a+a)

复杂度的大小取决于a的值。那么a取多少合适呢。

转化一下 y=n/a+a, n是固定值，a的取值范围是1到n，问a取多少时整体y最小。		

对y进行求导
$$
y'=- \frac{n}{a^2}+1 \\当a\in[1,\sqrt n) 时，y'<0, 说明y一直在减小 \\a=\sqrt n时到达最小值。\\之后 y'>0, y又开始增大。\\故a越接近 \sqrt n， y越小 \\令a= \lfloor \sqrt n \rfloor 即可 \\此时总体复杂度为O(n \sqrt n)。题目中最大规模为10^4，算下来规模是10^6可以接受。
$$

### AC代码

51nod平台有go选项，但是配置好像坏了，一直编译报错，先用C++写一下下。

```c++
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

const int N = 1e4 + 10;

class NumArray {
    /*
      a 代表每组多少个
      Max[i] = max(nums[i*a], nums[i*a+1],..., nums[i*a+a-1])
      nums[i] 属于Max[i/a]
    */
private:
    int Max[N]; // 分组最大值
    int num[N]; // 原始值
    int len; // 总长度
    int a; // 每个长度
public:
    NumArray(vector<int> arr);

    int Query(int i, int j);
};

NumArray::NumArray(vector<int> arr) : len(arr.size()) {
    a = sqrt(arr.size());
    for (int i = 0; i < arr.size(); i++) {
        num[i] = arr[i];
        Max[i / a] = max(Max[i / a], arr[i]);
    }
}

int NumArray::Query(int i, int j) {
    if (j < i) return Query(j, i);
    int ans = 0;

    for (int x = i; x < i / a * a + a && x<=j; x++) // 需要考虑头尾在一组的情况
        ans = max(ans, num[x]); // 汇总前段

    for (int x = i / a + 1; x < j / a; x++)
        ans = max(ans, Max[x]); // 汇总中间完整分组

    for (int x = max(i, j / a * a); x <= j; x++) // 需要考虑头尾在一组的情况
        ans = max(ans, num[x]); // 汇总后段

    return ans;
}

int main() {
    int n;
    cin >> n;

    vector<int> arr(n, 0);
    for (int i = 0; i < n; i++) cin >> arr[i];

    NumArray na(arr);
    int q;
    cin >> q;

    for (int i = 0; i < q; i++) {
        int a, b;
        cin >> a >> b;
        cout << na.Query(a, b) << endl;
    }
}

```

## 解法三 动态规划（两端法）

dp 是一个二维数组，dp[i\]\[j](i<=j) 表示区间[i,j] 内的最大值。

区间[a,b]的最大值就是dp[a]\[b]。

初始条件与转移方程如下
$$
dp[i][j] = \left\{
\begin{aligned}
arr[i] \ \ \ \  (i=j)  \\
max(dp[i][j-1], arr[j])\ \ \ \ (i<j) \\
\end{aligned}
\right.
$$
从上式中可看出，空间复杂度和时间复杂度都是O(n^2)=10^8。不符合题目的复杂度要求。



## 解法四 动态规划（长度法）

dp[i][j] 表示 长度为i起始位置为j的区间的最大值。
区间[a, b] 的最大值就是dp[b-a+1\][a]。

初始条件与转移方程如下
$$
dp[i][j] = \left\{
\begin{aligned}
arr[j] \ \ \ \  (i=1)  \\
max(dp[i-1][j], arr[j+i-1])\ \ \ \ (i>1) \\ 在原来的i-1长度加上arr[j+i-1], 从j开始往后数第i个\\
\end{aligned}
\right.
$$
从上式中可看出，空间复杂度和时间复杂度都是O(n^2)=10^8。不符合题目的复杂度要求。



## 解法五 动态规划（稀疏表法 RMQ-ST）

在解法4中，我们需要计算所有长度的可能性，空间上不可行。

在解法2中只计算了1种长度，需要比较多次，时间上还有优化的空间。

RMQ-ST的思想是只计算长度2^i (i=0,1,2,3...n) 2^n <= len(arr)。其稀疏性体现在并不是计算所有长度。

那么任意一个区间都可以找到2段区间长度为2^i使得子区间的并集等于该区间。

![基本求值原理](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/RMQ-ST/基本求值原理.png)

上述max(arr[10,17]), max(arr[13,])

这样空间复杂度只要nlog(n)，时间复杂度是O(1)。

# 二、RMQ-ST详解

## 状态定义及空间优化

定义 dp[i\][j]为以第j个元素开始，长度为i的区间的最大值。

初始条件及转移方程如下：
$$
dp[i][j] = \left\{\begin{aligned}arr[j] \ \ \ \  (i=1)  \\max(dp[i/2][j], dp[i/2][j+i/2])\ \ \ \ (i=2^n) \\ 将原来两个长度为i/2的区间组合成一个长度i的区间
\end{aligned}
\right.
$$


从上式可以看出i最大可以达到len(arr) 那么数组最大会达到dp[len(arr)\][len(arr)]空间复杂度依然是O(n^2), 但是计算dp的时间复杂度已经下来了。

计算过程如下

```go

func makeST(arr []int) [][]int {
	dp := make([][]int, len(arr))
	for i := 1;i<=len(arr); i*=2 { // log(n) 次
		dp[i] = make([]int, len(arr))
		for j := 0; j+i-1 < len(arr); j++ { // n次
			if i == 1 {
				dp[i][j] = arr[j]
				continue
			}
			dp[i][j] = max(dp[i/2][j], dp[i/2][j+i/2])
		}
	}
	return dp
}

```

### 优化

上述算法中第一维只用了2^n行，很多空间是浪费的。

可以看出只用到了dp[2^0], dp[2^1], dp[2^2],..., dp[2^n]，

对下标取对数可以得到dp[log(2^0)], dp[log(2^1)], dp[log(2^2)],..., dp[log(2^n)] = dp[l0], dp[1], dp[2],..., dp[n]  

所以我们可以重新定义dp

dp[i\][j] 表示 以第j个元素开始，长度为2^i的区间的最大值。

转移方程如下：
$$
dp[i][j] = \left\{\begin{aligned} arr[j] \ \ \ \  (i=0)  \\max(dp[i-1][j], dp[i-1][j+2^{i-1}])\ \ \ \ (i>0, 2^i<=len(arr)) \\ 将原来两个长度为2^{i-1}的区间组合成一个长度2^i的区间
\end{aligned}
\right.
$$
![合并过程](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/RMQ-ST/合并过程.png)





## 求值过程

对于任意一个区间x，利用二进制原理找到2段长度为2^i的区间a, b，使得这2段区间a, b的并集刚好等于x。

![求值原理2](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/RMQ-ST/求值原理2.png)

待求区间是[left, right], 区间a 是从left开始长度是2^i, 区间b是长度为2^i刚好到right, 从图中可以看出a,b长度之和是大于等[left, right]长度，并集刚好是[left, right]。由于是取大值比较，所以可以重复比较，我们只要拿到a, b区间的最大值就可以了。

区间a最大值根据定义可以知是dp[i\][left], 

区间b要先知道b的起始位置是right-2^i+1, 故最大值是dp[i\][right-2^i+1]

所以区间[left, right]最大值是max(dp[i\][left], dp[i\][right-2^i+1])。

## i如何求得

从图中可知只要使得2^i<=right-left+1, i取最大值就可以。

i=log(right-left+1), 由于i是整数，向下取整就可以。
$$
i=\lfloor log(right-left+1) \rfloor
$$


## 小结

dp[i\][j]表示 以第j个元素开始，长度为2^i的区间的最大值。

转移方程
$$
dp[i][j] = \left\{\begin{aligned} arr[j] \ \ \ \  (i=0)  \\max(dp[i-1][j], dp[i-1][j+2^{i-1}])\ \ \ \ (i>0, 2^i<=len(arr)) \\ 将原来两个长度为2^{i-1}的区间组合成一个长度2^i的区间
\end{aligned}
\right.
$$
求值过程

区间[left, right] 最大值为 
$$
max(dp[i][left], dp[i][right-2^i+1])。\\
i=\lfloor log(right-left+1) \rfloor
$$


# 三、算法模板

Go 版本

```go

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

type RMQ struct {
	t [][]int
}

func (r *RMQ) makeST(arr []int) {
	r.t = make([][]int, 1+int(math.Log2(float64(len(arr)))))
	for i := 0; (1 << i) <= len(arr); i++ {
		r.t[i] = make([]int, len(arr))
		for j := 0; j+(1<<i)-1 < len(arr); j++ {
			if i == 0 {
				r.t[i][j] = arr[j]
				continue
			}
			r.t[i][j] = max(r.t[i-1][j], r.t[i-1][j+(1<<(i-1))])
		}
	}
}

func (r *RMQ) query(left, right int) int {
	if right < left {
		return -1
	}

	l := int(math.Log2(float64(right - left + 1)))
	return max(r.t[l][left], r.t[l][right-(1<<l)+1])
}

```

C++版本

```c++
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

const int N = 10000+10;

class RMQ {
private:
     int st[20][N];

public:
    void makeST(vector<int> &arr);
    int query(int left, int right);
};

void RMQ::makeST(vector<int> &arr) {
    for (int i = 0; (1 << i) <= arr.size(); i++) {
        for (int j = 0; j+(1<<i)-1 < arr.size(); j++) {
            if (i == 0) {
                st[i][j] = arr[j];
                continue;
            }
            st[i][j] = max(st[i-1][j], st[i-1][j+(1<<(i-1))]);
        }
    }
}

int RMQ::query(int left, int right) {
    if (right < left) {
        return -1;
    }

    int l = log2(right - left + 1);
    return max(st[l][left], st[l][right-(1<<l)+1]);
}

int main() {
    int n, q;

    cin >> n;

    vector<int> arr(n, 0);
    for(int i=0;i<n;i++) {
        cin>>arr[i];
    }
    RMQ rmq;
    rmq.makeST(arr);
    cin>>q;

    while(q--) {
        int a,b;
        cin>>a>>b;
        cout<<rmq.query(a,b)<<endl;
    }

    return 0;
}

```





# 四、牛刀小试

## 练习1 重做例题
http://www.51nod.com/Challenge/Problem.html#problemId=1174

### 题目大意

略

### 题目解析
直接利用模块

### AC代码

~~~c++
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

const int N = 10000+10;

class RMQ {
private:
     int st[20][N];

public:
    void makeST(vector<int> &arr);
    int query(int left, int right);
};

void RMQ::makeST(vector<int> &arr) {
    for (int i = 0; (1 << i) <= arr.size(); i++) {
        for (int j = 0; j+(1<<i)-1 < arr.size(); j++) {
            if (i == 0) {
                st[i][j] = arr[j];
                continue;
            }
            st[i][j] = max(st[i-1][j], st[i-1][j+(1<<(i-1))]);
        }
    }
}

int RMQ::query(int left, int right) {
    if (right < left) {
        return -1;
    }

    int l = log2(right - left + 1);
    return max(st[l][left], st[l][right-(1<<l)+1]);
}

int main() {
    int n, q;

    cin >> n;

    vector<int> arr(n, 0);
    for(int i=0;i<n;i++) {
        cin>>arr[i];
    }
    RMQ rmq;
    rmq.makeST(arr);
    cin>>q;

    while(q--) {
        int a,b;
        cin>>a>>b;
        cout<<rmq.query(a,b)<<endl;
    }

    return 0;
}

// go 版本
package main

import (
	"fmt"
	"math"
)

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

type RMQ struct {
	t [][]int
}

func (r *RMQ) makeST(arr []int) {
	r.t = make([][]int, 1+int(math.Log2(float64(len(arr)))))
	for i := 0; (1 << i) <= len(arr); i++ {
		r.t[i] = make([]int, len(arr))
		for j := 0; j+(1<<i)-1 < len(arr); j++ {
			if i == 0 {
				r.t[i][j] = arr[j]
				continue
			}
			r.t[i][j] = max(r.t[i-1][j], r.t[i-1][j+(1<<(i-1))])
		}
	}
}

func (r *RMQ) query(left, right int) int {
	if right < left {
		return -1
	}

	l := int(math.Log2(float64(right - left + 1)))
	return max(r.t[l][left], r.t[l][right-(1<<l)+1])
}

func maxSlidingWindow(nums []int, k int) []int {
	rmq := &RMQ{}
	rmq.makeST(nums)
	ans := []int{}
	for left, right := 0, k-1; right < len(nums); left, right = left+1, right+1 { // 初始区间，每次右移，直到右边出界
		ans = append(ans, rmq.query(left, right))
	}
	return ans
}

func main() {
	n := 0
	fmt.Scanf("%d", &n)
	arr := make([]int, n)
	for i := 0; i < n; i++ {
		fmt.Scanf("%d", &arr[i])
	}

	rmq := &RMQ{}
	rmq.makeST(arr)

	q := 0
	fmt.Scanf("%d", &q)
	for ; q > 0; q-- {
		l, r := 0, 0
		fmt.Scanf("%d %d", &l, &r)
		fmt.Println(rmq.query(l, r))
	}
}

~~~


## 练习2 滑动窗口最大值

题目链接：https://leetcode-cn.com/problems/sliding-window-maximum/

### 题目大意
给你一个整数数组 nums，有一个大小为 k 的滑动窗口从数组的最左侧移动到数组的最右侧。你只可以看到在滑动窗口内的 k 个数字。滑动窗口每次只向右移动一位。

返回滑动窗口中的最大值。

 

示例 1：

输入：nums = [1,3,-1,-3,5,3,6,7], k = 3
输出：[3,3,5,5,6,7]
解释：
滑动窗口的位置                最大值
---------------               -----
[1  3  -1] -3  5  3  6  7       3
 1 [3  -1  -3] 5  3  6  7       3
 1  3 [-1  -3  5] 3  6  7       5
 1  3  -1 [-3  5  3] 6  7       5
 1  3  -1  -3 [5  3  6] 7       6
 1  3  -1  -3  5 [3  6  7]      7
示例 2：

输入：nums = [1], k = 1
输出：[1]
示例 3：

输入：nums = [1,-1], k = 1
输出：[1,-1]
示例 4：

输入：nums = [9,11], k = 2
输出：[11]
示例 5：

输入：nums = [4,-2], k = 2
输出：[4]


提示：

1 <= nums.length <= 10^5
-10^4 <= nums[i] <= 10^4
1 <= k <= nums.length


### 题目解析
每滑动一个都会产生一个新的区间，可以利用求区间最大值解决。

### AC代码

~~~go

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

type RMQ struct {
	t [][]int
}

func (r *RMQ) makeST(arr []int) {
	r.t = make([][]int, 1+int(math.Log2(float64(len(arr)))))
	for i := 0; (1 << i) <= len(arr); i++ {
		r.t[i] = make([]int, len(arr))
		for j := 0; j+(1<<i)-1 < len(arr); j++ {
			if i == 0 {
				r.t[i][j] = arr[j]
				continue
			}
			r.t[i][j] = max(r.t[i-1][j], r.t[i-1][j+(1<<(i-1))])
		}
	}
}

func (r *RMQ) query(left, right int) int {
	if right < left {
		return -1
	}

	l := int(math.Log2(float64(right - left + 1)))
	return max(r.t[l][left], r.t[l][right-(1<<l)+1])
}

func maxSlidingWindow(nums []int, k int) []int {
	rmq := &RMQ{}
	rmq.makeST(nums)
	ans := []int{}
	for left, right := 0, k-1; right < len(nums); left, right = left+1, right+1 { // 初始区间，每次右移，直到右边出界
		ans = append(ans, rmq.query(left, right))
	}
	return ans
}
~~~



## 练习3 查找最有竞争力的子序列 

题目链接：https://leetcode-cn.com/problems/find-the-most-competitive-subsequence

### 题目大意

给你一个整数数组 nums 和一个正整数 k ，返回长度为 k 且最具 竞争力 的 nums 子序列。

数组的子序列是从数组中删除一些元素（可能不删除元素）得到的序列。

在子序列 a 和子序列 b 第一个不相同的位置上，如果 a 中的数字小于 b 中对应的数字，那么我们称子序列 a 比子序列 b（相同长度下）更具 竞争力 。 例如，[1,3,4] 比 [1,3,5] 更具竞争力，在第一个不相同的位置，也就是最后一个位置上， 4 小于 5 。

示例 1：

输入：nums = [3,5,2,6], k = 2
输出：[2,6]
解释：在所有可能的子序列集合 {[3,5], [3,2], [3,6], [5,2], [5,6], [2,6]} 中，[2,6] 最具竞争力。

示例 2：

输入：nums = [2,4,3,3,5,4,9,6], k = 4
输出：[2,3,3,4]



- `1 <= nums.length <= 10^5`
- `0 <= nums[i] <= 10^9`
- `1 <= k <= nums.length`


### 题目解析

之前讲解链接 [[*1673*] 查找最有竞争力的子序列](https://mp.weixin.qq.com/s/D0IN_pJQgMgstd0GX6yDmw)

本题有2种做法，有兴趣可以前往了解，这里主要讲解如何使用RMQ解决。

根据题意可以使用贪心思想，从左往右依次选择最小的数字。这里要保证选择完一个数字以后，剩下的数字个数要满足题要求的k个。

为了保证这一点，我们每次先预留出最右边的k-1个数字。然后从剩下的数字里选择出最小的并且靠最左边的数字x。然后把x左边的数字从数组中删除。做完一轮k--，直到k=0。

以下是例2的算法过程。

![算法过程](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/RMQ-ST/算法过程.png)

每次从待选值中选出值最小的数字，如果数字大小相同则取下标小的。实现方法，存储最小值的下标，判断条件加入下标判断。



### AC代码

~~~go
/*
区间最值问题，指的是查询数组某一段中数字最大或最小值，
时间复杂度: 初始化 nlog(n), 查询O(1)
空间复杂度：nlog(n)
*/

func MinFunc(a, b int) int { // 
    if a == b {
        return 0
    }
    if a < b {
        return -1
    }
    return 1
}

func MaxFunc(a, b int) int {
    return MinFunc(b, a)
}

type RangeM struct {
    array       []int              // 存储原始数据
    store       [][]int            // 存储计算最值结果, store [i][j] 存储的是起点为j长度为2^i 的区间 [j, j+2^i）的最值（下标）。
    compareFunc func(a, b int) int // 自定义比较函数
}

func (r *RangeM) Init(arr []int, comFunc func(a, b int) int) error {
    if comFunc == nil {
        return errors.New("comFunc is empty")
    }
    r.compareFunc = comFunc
    r.array = arr
    if err := r.assignStore(); nil != err {
        return err
    }

    if err := r.calStore(); nil != err {
        return err
    }

    return nil
}

// 预先分配内在
func (r *RangeM) assignStore() error {
    l := len(r.array)
    if l <= 0 {
        return nil
    }

    r.store = make([][]int, int(math.Log2(float64(l)))+2) // 需要 log(l) 行，保险起见，+2
    for i, _ := range r.store {
        r.store[i] = make([]int, l)
    }

    return nil
}

func (r *RangeM) calStore() error {
    l := len(r.array)
    // 第一行长度为1，赋值为自身
    for i := 0; i < l; i++ {
        r.store[0][i] = i
    }

    // store[i][j] = max | min (store[i-1][j], store[i-1][j+1<<(i-1)])
    for i := uint(1); 1<<i <= l; i++ {
        for j := 0; j < l; j++ {
            r.store[i][j] = r.store[i-1][j] // 默认赋值左半边最值
            right := j + int(1<<(i-1))
            // 如果有右半边，且值比左半边更优，则取右半边值
            if right < l && r.compareFunc(r.array[r.store[i-1][j]], r.array[r.store[i-1][right]]) > 0 {
                r.store[i][j] = r.store[i-1][right]
            }
        }
    }

    return nil
}

func (r *RangeM) GetValue(start, end int) (value int, index int, err error) {
    if r.store == nil {
        return 0, -1, errors.New("not init yet")
    }
    bit := uint(math.Log2(float64(end - start+1))) // 找到一个长度，使得从前后2边伸展，并最终可以交叉
    // 比较前后2边最值，取最优解。
    index = r.store[bit][start]
    right := end - (1 << bit) +1
    if r.compareFunc(r.array[index], r.array[r.store[bit][right]]) > 0 {
        index = r.store[bit][right]
    }

    return r.array[index], index, nil
}

func getRangeMin(nums []int, start, end int) (num, ind int) {
    num, ind = nums[start], start
    for ; start < end; start++ {
        if nums[start] < num {
            num, ind = nums[start], start
        }
    }
    return
}

func mostCompetitive(nums []int, k int) []int {
    res := []int{}
    l := len(nums)
    rm := &RangeM{}
    rm.Init(nums, MinFunc)

    for ind := -1; k > 0; k-- {
        var minNum int
        minNum, ind, _ = rm.GetValue(ind+1, l-(k-1)) // 选完一个将起始值更新
        res = append(res, minNum)
    }

    return res
}
/*
[0]
1
*/
~~~

# 五、总结

## 主要内容：

1. 本文详细介绍了RMQ-ST原理，先求出所有2^i长度区间的最值，利用二分法找到2段区间，求最值。

2. 作用：查询区间最值。

笔者水平有限，有写得不对或者解释不清楚的地方还望大家指出，我会尽自己最大努力去完善。

下面我精心准备了几个流行网站上的题目（首先AK F.\*ing leetcode），给大家准备了一些题目，供大家练习参考。干他F.\*ing (Fighting?)。

# 六、实战训练

## 代码基础训练题

光说不练假把式，学完了怎么也要实操一下吧，快快动手把刚才的题A了。

1. 区间中最大的数：http://www.51nod.com/Challenge/Problem.html#problemId=1174
2. 滑动窗口最大值：https://leetcode-cn.com/problems/sliding-window-maximum/
3. 查找最有竞争力的子序列：https://leetcode-cn.com/problems/find-the-most-competitive-subsequence

## AK leetcode

leetcode相关题目都在下面了，拿起武器挨个点名呗。

1. https://leetcode-cn.com/problems/find-a-value-of-a-mysterious-function-closest-to-target/
2. https://leetcode-cn.com/problems/longest-continuous-subarray-with-absolute-diff-less-than-or-equal-to-limit/

----

做完以上还觉得不过瘾，我给大家还准备了一些。

## 大神进阶

也可以去vjudge https://vjudge.net/problem 搜索相关题号

hdu

以下将序号替换就是题目链接。

1. https://acm.hdu.edu.cn/showproblem.php?pid=3183
2. https://acm.hdu.edu.cn/showproblem.php?pid=3193
3. https://acm.hdu.edu.cn/showproblem.php?pid=3486
4. https://acm.hdu.edu.cn/showproblem.php?pid=6305

Poj

以下将序号替换就是题目链接。

1. http://poj.org/problem?id=2019
2. http://poj.org/problem?id=3264
3. http://poj.org/problem?id=3368
