
# 文章目录
[TOC]

# 零、简介

KMP算法是一个让人听到就害怕的算法，原因在于其原理比较难理解。看好几遍也看不懂。往往遇到字符匹配问题就绕不开。其主要作用有：字符串匹配，计算字符串循环周期，根据next定义也可以求解字符串最长公共后缀前缀。

本文从朴素匹配算法出发，优化移动次数，图文结合，证明其实定理的正确性，全面理解KMP。

KMP算法主要包括next数组计算，以及串匹配2个过程，其中next数组本身可以计算字符串循环周期的作用。

**为了方便说明，以下作了一些符号定义**

s为非空字符串

- s[i,j] 为s从第i个字符到第j个字符组成的子串
- s[0,i] 为以第i个字符结尾的s前缀
- s[i,len(s)-1]为以第i个字符开始的的s后缀

# 一、字符串匹配从朴素思想到KMP

## 基本问题

https://leetcode-cn.com/problems/implement-strstr/

实现 strStr() 函数。

给你两个字符串 haystack 和 needle ，请你在 haystack 字符串中找出 needle 字符串出现的第一个位置（下标从 0 开始）。如果不存在，则返回  -1 。

示例 1：

输入：haystack = "hello", needle = "ll"
输出：2
示例 2：

输入：haystack = "aaaaa", needle = "bba"
输出：-1
示例 3：

输入：haystack = "", needle = ""
输出：0

- `0 <= haystack.length, needle.length <= 5 * 104`
- `haystack` 和 `needle` 仅由小写英文字符组成

## 朴素思想（逐个后移法）

从前往后依次选择haystack[i] 为开头与needle进行比较，如果成功就返回，不成功就把i后移。

![普通比较法](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/KMP/普通比较法.png)

上图中每次比较到不相等时就后移，当比较到第3趟时发现了完整匹配。



```go
/*
从前往后依次选择haystack[i] 为开头与needle进行比较，如果成功就返回，
不成功就把i后移。
*/
func strStr(haystack string, needle string) int {
	if needle == "" {
		return 0
	}
	for i := 0; i < len(haystack); i++ {
		j := 0
		// 以haystack[i]开始比较
		for ; j < len(needle) && i+j < len(haystack); j++ {
			if haystack[i+j] != needle[j] {
				break
			}
		}
		if j == len(needle) {
			return i
		} // 成功直接返回，不成功i 后移
	}

	return -1
}
```

上述算法每一趟的极端情况要比较len(needle)次。所以，总体复杂度是len(needle)*len(haystack)。



## 引出优化算法（KMP）

上述算法是每次到匹配不上时往后移一个（这里转变一下，将needle后移，效果其实一样），其实很多情况下第1个就匹配不上，所以是不是可以多移几个。这就意味着，移动之后至少有1个是匹配上的。那么怎么决定移几个呢。移多了怕错过前面的可行解，移少了效率不够高。这里KMP思想多加了一个条件，就是needle往后移时，要匹配上之前比较过的haystack的某个**后缀**。

根据贪心原理这个后缀当越长越好，越长意味着后移得越少，不会错过可行解。

![移动后的效果](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/KMP/移动后的效果.png)

假如第1趟比较第needle[3]就不相等了，就有可能出现3种情况，needle后移1个，2个，3个，但不管怎么样，haystack已经比较过的是不会再比较了。

如何确定后移几个呢，KMP引入了一个next数组的概念。通过next数组可以快速O(1)确定后移几个。在下一部分会详细介绍。

后面继续比较，当发现不相等时又会将needle按照上述方法后移，直到比较完所有haystack字符。

从上述描述中可以看出当前比较的字符haystack[i] 是一直后移的，这个过程的复杂度是O(len(haystack))。后续会介绍next数组计算，其复杂度为O(len(needle)), 总复杂度为 O(len(haystack))+O(len(needle))。

# 二、NEXT数组详解

## next数组定义

每个字符串都有自己的next数组，对于字符串s, 其next数组长度为len(s)的int数组。

其中next[i]的值定义如下：

设next[i] = k，则k的值是子串s0s1s2...si 最长公共前缀后缀长度（k<=i, 前缀和后缀不可以是子串本身）

看图，

![next定义](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/KMP/next定义.png)



## next数组在子串查找中的作用以及其正确性



回到上面的问题当失配以后要如何移动。



![移动的条件](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/KMP/移动的条件.png)

在上图中，当匹配到i时，字符失配了，这里首先有一个重要信息，**haystack[0,i-1] = needle[0,j-1]**， **这个结论很重要**, 是后面结论的基石。

![条件推导](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/KMP/条件推导.png)

那么如果我们想要第2趟从needle的第k（k<j）个字符开始匹配, 则要满足haystack[i-k,i-1] = needle[0,k-1].

上图中**蓝色**框中的子串都是相同的。

根据上述结论，**haystack[0,i-1] = needle[0,j-1]**，可以导出haystack[i-k,i-1] = needle[j-k, j-1]。

所以，haystack[i-k,i-1] = needle[0,k-1]该条件可以转变成 满足  needle[j-k, j-1] = needle[0,k-1]。根据贪心原理k越大越好。从图中可以看出这个K就是上面提到的next[j-1], 就是子串needle[0,j-1] 最长公共前缀后缀的长度。

由上述分析得出结论，当haystack[i]=needle[j]时 i++, j++, 否则 i不变，j=next[j-1]\(j>0)。

以下给出KMP算法(假设next已经计算出来了，可以先根据定义手动算一下，后面讲解next数组求解方法)

~~~go
ype KMP struct {
	next    []int
	needle string
}

// a b a c a b a
// 0 0 1 0 1 2 3

func (k *KMP) makeNext(needle string) []int {
	// todo
}

func (k *KMP) find(haystack string) []int {
	j := 0
	res := []int{}
	for i, c := range haystack {
		for j > 0 && c != int32(k.needle[j]) { //不相等就把模式串往后移，直到相等或从头开始
			j = k.next[j-1]
		}
		if c == int32(k.needle[j]) {// 比较下一个
			j++
		}
		if j == len(k.needle) { // 完全找到模式串
			res = append(res, i-len(k.needle)+1)// 添加结果
			j = k.next[j-1] // 查询下一处
		}
	}

	return res
}
~~~

图解如下：

![算法过程](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/KMP/算法过程.png)

至此我们已经大概了解了KMP的基本思想及其主要工作原理。

下面有几个疑问要解释一下。

1. 为什么每次失配后是移动到next[j-1]继续匹配

答：根据next[j-1]的定义，是needle[0,j-1]最长公共前缀后缀的长度。可以得到 haystack[i-next[j-1],i-1] = needle[0, next[j-1]] ，刚好继续比较，上图已经解释。

2. 会不会移动太多导致错过可行解。

答：不会，next[j-1]已经是最大值了，i-next[j-1]是最小的，所以移动的是最小个数使得needle与haystack有部分可以匹配。



## next数组求解方法

还是从next定义出发。

对于字符串s, 其next数组长度为len(s)的int数组。

其中next[i]的值定义如下：

设next[i] = k，则k的值是子串s[0,i]最长公共前缀后缀长度（k<=i, 前缀和后缀不可以是子串本身）

显然，next[0]=0。

当 next[i]已经求出，求解next[i+1]时，根据定义可知s[0, next[i]-1] = s[i-next[i]+1,i]

如果 s[i+1] = s[next[i]] , 则 next[i+1] = next[i]+1，否则就要缩短目前已经匹配上的前缀和后缀，过程与上方needle往后移动类似。即下图中j=next[j-1] (j>0时，否则 j=0) 直到needle_prefix[j]==needle_suffix[i]。

**这个过程要多看几遍，结合代码和看图**。

![next计算过程](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/KMP/next计算过程.png)

实际实现过程中并不需要复制needle, 只要用下标i,j 表示即可。

~~~
// 代码实现

type KMP struct {
	next    []int
	pattern string
}

// a b a c a b a
// 0 0 1 0 1 2 3

func (k *KMP) makeNext(needle string) []int {
	k.pattern = needle
	k.next = make([]int, len(needle))

	for i, j := 1, 0; i < len(needle); i++ {
		for j > 0 && needle[j] != needle[i] { // 这里一定要循环，举例 aaab
			j = k.next[j-1]
		}

		if needle[j] == needle[i] {
			j++
		}
		k.next[i] = j
	}

	return k.next
}
~~~

## 重新求解

回过头来把一开始的题目使用KMP求解

~~~go

type KMP struct {
	next    []int
	pattern string
}

// a b a c a b a
// 0 0 1 0 1 2 3

func (k *KMP) makeNext(pattern string) []int {
	k.pattern = pattern
	k.next = make([]int, len(pattern))

	for i, j := 1, 0; i < len(pattern); i++ {
		for j > 0 && pattern[j] != pattern[i] { // 这里一定要循环，举例 aaab
			j = k.next[j-1]
		}

		if pattern[j] == pattern[i] {
			j++
		}
		k.next[i] = j
	}

	return k.next
}

func (k *KMP) find(s string) []int {
	j := 0
	res := []int{}
	for i, c := range s {
		for j > 0 && c != int32(k.pattern[j]) { // 不相等就把模式串往后移，直到相等或从头开始
			j = k.next[j-1]
		}
		if c == int32(k.pattern[j]) { // 比较下一个
			j++
		}
		if j == len(k.pattern) {  // 完全找到模式串
			res = append(res, i-len(k.pattern)+1) // 添加结果
			j = k.next[j-1]  // 查询下一处
		}
	}

	return res
}

func strStr(haystack string, needle string) int {
	if needle == "" {
		return 0
	}
	kmp := &KMP{}
	kmp.makeNext(needle)
	ind := kmp.find(haystack)
	if len(ind) ==0 {return -1}
	return ind[0]
}
/*
"hello"
"ll"
"aaaaa"
"bba"
""
""
"aaaa"
"aaaa"
*/

~~~



# 三、KMP推论及证明

- s的前缀后缀数组为next, 



# 三、作用

1. 主要作用是求区间和。
2. 与前缀和相比，其最大的特点是可以处理多次可变的数组区间和。
3. 可以处理二维数组的情况。
4. 结合差分可以解决区间修改，区间和查询。
5. 配合二分思想可以做很多动态数组查询问题，比如第K大数字，最大值，最小值。
6. 结合数字离散化，可以解决很多计数问题。

# 四、数据定义及算法

树状数组，顾名思义是用数组来表示的。

主要有更新和求区间和2种操作。

## 数据定义

~~~go
TreeArray {
    sum array // 数组 编号从1开始
  	n int // 数组大小
    // 主要操作：
	  Init(int): // 初始化，申请数组大空间。
	  Add(int,int): //更新操作
	  QueryPre(int):// 求前缀和
	  QuerySum(int,int): // 求区间和
}
~~~

## 算法描述

### 初始化

~~~
Init(l int) {
	sum <- [l]int
	n <- l
}
~~~



### 更新操作

![更新操作](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/树状数组/更新操作.png)

这里的更新操作是对某个元素进行加或减操作，直接赋值操作需要转化成加减操作。

当我们对某个元素进行操作后，那么sum数组中被影响到的结点都要更新。

以Arr[3]+=3为例，那么 如图中沿黑线往 上找到的结点都要更新sum[3], sum[4], sum[8], sum[16]都是被影响到的结点，都要+3。

那么怎么找呢，就要用到上文中计算K的原理了。需要找到二进制中最低位的1.

比较直接的就是移位法，查询x中k的值如下

~~~
getLowBit(x):
	i=1
	while x&i==0 {
		i<<=1
	}
	return i
~~~

还可以利用(x-1)&x的作用是把最后一个1变成0的原理。

~~~
getLowBit(x):
	return x - ((x-1)&x)
~~~

官方给出的方法是

~~~
getLowBit(x):
	return x & (-x)
	
~~~

原理是负数的二进制是原数的反码加1。刚好除了最低位1和后面的0是一样的，其他位都是相反的。如图：



![负数表示](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/树状数组/负数表示.png)

负数表示的原理如下，以32位为例，以0为基准。
$$
0 = \underbrace {1111\ \ 1111\ \ 1111\ \ 1111\ \ 1111\ \ 1111\ \ 1111\ \ 1111}_{32个1} \ \ 最高位是1\\
-1 = 0-1 = \underbrace {1111\ \ 1111\ \ 1111\ \ 1111\ \ 1111\ \ 1111\ \ 1111\ \ 1110}_{31个1+1个0} \\
就是挖空一些位置，而这些挖空的值刚好是与0的差值。
$$


~~~
// 更新算法
add(i, a): // 更新 arr[i] +=a
	for ;i<=n;i+=getLowBit(i) { // 沿着线往上找影响到的结点，并更新
		sum[i]+=a
	}


~~~
### 计算前缀和

![树状求和](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/树状数组/树状求和.png)

与更新相反，求和是从上到下的一个过程。

以求 preSum(11)为例。图中橙色块就是要加的对象。

先加sum[11], 计算出sum[11]中元素的长度getLowBit（11）=1， 11-1=10

再加sum[10], 同理计算出lowbit = 2， 10-2=8

再加sum[8],  同理计算出lowbit = 8, 8-8=0 算法结束。



~~~
// 求前缀和
QueryPre(i):
	res = 0
	for ;i>0;i-=getLowBit(i){
		res += sum[i]
	}
	return res
	
// 求区间和
QuerySum(i,j):
	return QueryPre(j)-QueryPre(i-1)
~~~

二维数组情况可以参考

https://baike.baidu.com/item/%E4%BA%8C%E7%BB%B4%E6%A0%91%E7%8A%B6%E6%95%B0%E7%BB%84/20415329?fr=aladdin

https://blog.csdn.net/zzti_xiaowei/article/details/81053094



# 五、具体实现

~~~go

func getLowBit(x int) int {
	return x & (-x)
}

type TreeArray struct {
	sum []int // 数组 编号从1开始
	n   int   // 数组大小
}

// 初始化，申请数组大空间。
func (t *TreeArray) Init(n int) {
	t.n = n
	t.sum = make([]int, n+1) // 0号元素不用，n号元素需要使用
}

//更新操作
func (t *TreeArray) Add(x, v int) {
	for ; x <= t.n; x += getLowBit(x) { // 沿着线往上找影响到的结点，并更新
		t.sum[x] += v
	}
}

// 求前缀和
func (t *TreeArray) QueryPre(i int) int {
	res := 0
	for ; i > 0; i -= getLowBit(i) {
		res += t.sum[i]
	}
	return res
}

// 求区间和
func (t *TreeArray) QuerySum(i, j int) int {
	return t.QueryPre(j) - t.QueryPre(i-1)
}

~~~

复杂度分析，每次add或sum 都是对数字的二进制进行移位，所以单次操作复杂度是log(n)



# 六、牛刀小试

## 练习1 可区间求和应用
链接：https://leetcode-cn.com/problems/range-sum-query-mutable/

回到刚开始的题目

### 题目大意

略

### 题目解析
只要利用add做单点更新，用querysum求区间和就可以了

### AC代码

~~~c++

func getLowBit(x int) int {
	return x & (-x)
}

type TreeArray struct {
	sum []int // 数组 编号从1开始
	n   int   // 数组大小
}

// 初始化，申请数组大空间。
func (t *TreeArray) Init(n int) {
	t.n = n
	t.sum = make([]int, n+1) // 0号元素不用，n号元素需要使用
}

//更新操作
func (t *TreeArray) Add(x, v int) {
	for ; x <= t.n; x += getLowBit(x) { // 沿着线往上找影响到的结点，并更新
		t.sum[x] += v
	}
}

// 求前缀和
func (t *TreeArray) QueryPre(i int) int {
	res := 0
	for ; i > 0; i -= getLowBit(i) {
		res += t.sum[i]
	}
	return res
}

// 求区间和
func (t *TreeArray) QuerySum(i, j int) int {
	return t.QueryPre(j) - t.QueryPre(i-1)
}

type NumArray struct {
	tree *TreeArray
	nums []int
}

/*
由于树状数组弃用0号元素，所以以下操作下标都要加1
 */
func getIndex(x int) int {
	return x+1
}

func Constructor(nums []int) NumArray {
	na := NumArray{
		tree: &TreeArray{},
		nums: nums,
	}
	na.tree.Init(len(nums))

	// 计算sum值
	for i, v := range nums {
		na.tree.Add(getIndex(i), v) // 注意下标转化
	}

	return na
}

func (this *NumArray) Update(index int, val int) {
	// 更新 差值
	this.tree.Add(getIndex(index),  val - this.nums[index])
	this.nums[index] = val
}

func (this *NumArray) SumRange(left int, right int) int {
	return this.tree.QuerySum(getIndex(left), getIndex(right)) // 直接调用求区间和
}
/*
cases:
["NumArray","sumRange","update","sumRange"]
[[[1,3,5]],[0,2],[1,2],[0,2]]
["NumArray","sumRange","update","sumRange"]
[[[1]],[0,0],[0,2],[0,0]]
["NumArray","sumRange","sumRange","sumRange","update","sumRange","sumRange","update","sumRange"]
[[[1,2,3,4,5,6,7,8,9,10]],[0,9],[1,8],[2,7],[0,9],[3,6],[4,5],[5,5],[5,5]]
*/

~~~


## 练习2 离散化计数 [统计作战单位数](https://leetcode-cn.com/problems/count-number-of-teams/)

题目链接：https://leetcode-cn.com/problems/count-number-of-teams/

### 题目大意
 n 名士兵站成一排。每个士兵都有一个 独一无二 的评分 rating 。

每 3 个士兵可以组成一个作战单位，分组规则如下：

从队伍中选出下标分别为 i、j、k 的 3 名士兵，他们的评分分别为 rating[i]、rating[j]、rating[k]
作战单位需满足： rating[i] < rating[j] < rating[k] 或者 rating[i] > rating[j] > rating[k] ，其中  0 <= i < j < k < n
请你返回按上述条件可以组建的作战单位数量。每个士兵都可以是多个作战单位的一部分。

 

示例 1：

输入：rating = [2,5,3,4,1]
输出：3
解释：我们可以组建三个作战单位 (2,3,4)、(5,4,1)、(5,3,1) 。
示例 2：

输入：rating = [2,1,3]
输出：0
解释：根据题目条件，我们无法组建作战单位。
示例 3：

输入：rating = [1,2,3,4]
输出：4


提示：

n == rating.length
3 <= n <= 1000
1 <= rating[i] <= 10^5
rating 中的元素都是唯一的


### 题目解析
先把题目简化一下给定士兵i 求排在i前面并且rating比 rating[i]小的士兵个数。

这里有2个维度，坐标的大小，rating的大小。

一般这种题目可以从前往后遍历，当前遍历到的士兵的坐标都是比之前大的，就解决了坐标大小的问题。

再看tating统计。其实就是查询之前遍历过的rating里比当前raging小的个数。

如果把rating统计到一个数组 cnt[] 每遍历一个rating[i]，cnt[rating[i]]++, 想查询<rating[i]的个数时。

其实就是 
$$
当cnt数组里<rating[i]的个数 = \sum_{x=0}^{rating[i]-1}cnt[x]
$$
这一看不就是前缀和么。而且是一个可变数组的前缀和。可以使用树状数组解决。

解决了所有士兵排在他前面且rating比他小的计数问题后。假设结果数组是Res1[]。

再来看rating[i] < rating[j] < rating[k] （0 <= i < j < k < n）。

可以再利用上述思想，从前往后遍历，然后统计cnt[rating[i]]+=res1[i]。
$$
 rating[i] < rating[j] < rating[k] 的总数 = \sum_{x=0}^{rating[k]-1}cnt[x]
$$
依然是求前缀和，可以使用树状数组解决。

对于 rating[i] > rating[j] > rating[k] 也可以如法炮制，只不过求和区间要改一下。
$$
\sum_{x=rating[k]+1}^{10^5}cnt[x]
$$
查询比自己大的计数范围，依然是区间和。



### AC代码

~~~go

func getLowBit(x int) int {
	return x & (-x)
}

type TreeArray struct {
	sum []int // 数组 编号从1开始
	n   int   // 数组大小
}

// 初始化，申请数组大空间。
func (t *TreeArray) Init(n int) {
	t.n = n
	t.sum = make([]int, n+1) // 0号元素不用，n号元素需要使用
}

//更新操作
func (t *TreeArray) Add(x, v int) {
	for ; x <= t.n; x += getLowBit(x) { // 沿着线往上找影响到的结点，并更新
		t.sum[x] += v
	}
}

// 求前缀和
func (t *TreeArray) QueryPre(i int) int {
	res := 0
	for ; i > 0; i -= getLowBit(i) {
		res += t.sum[i]
	}
	return res
}

// 求区间和
func (t *TreeArray) QuerySum(i, j int) int {
	return t.QueryPre(j) - t.QueryPre(i-1)
}

type NumArray struct {
	tree *TreeArray
	nums []int
}


func numTeams(rating []int) int {
	res1 , res2 := make([]int, len(rating)), make([]int, len(rating)) // 同时求出比自己大和小的计数。
	tree := &TreeArray{}
	tree.Init(1e5+10)
	for i, r := range rating {
		res1[i] = tree.QueryPre(r-1)
		res2[i] = tree.QuerySum(r+1, tree.n)
		tree.Add(r, 1)
	}

	sum :=0
	// 先求 rating[i] < rating[j] < rating[k]
	tree.Init(1e5+10) // 要重新初始化
	for i,r:= range rating {
		sum += tree.QueryPre(r-1)
		tree.Add(r, res1[i])
	}

	// 再求 rating[i] > rating[j] > rating[k]
	tree.Init(1e5+10) // 要重新初始化
	for i,r:= range rating {
		sum += tree.QuerySum(r+1, tree.n)
		tree.Add(r, res2[i])
	}

	return sum
}
~~~



## 练习3 [查询带键的排列](https://leetcode-cn.com/problems/queries-on-a-permutation-with-key/)

题目链接：https://leetcode-cn.com/problems/queries-on-a-permutation-with-key/

### 题目大意

给你一个待查数组 queries ，数组中的元素为 1 到 m 之间的正整数。 请你根据以下规则处理所有待查项 queries[i]（从 i=0 到 i=queries.length-1）：

一开始，排列 P=[1,2,3,...,m]。
对于当前的 i ，请你找出待查项 queries[i] 在排列 P 中的位置（下标从 0 开始），然后将其从原位置移动到排列 P 的起始位置（即下标为 0 处）。注意， queries[i] 在 P 中的位置就是 queries[i] 的查询结果。
请你以数组形式返回待查数组  queries 的查询结果。

 

示例 1：

输入：queries = [3,1,2,1], m = 5
输出：[2,1,2,1] 
解释：待查数组 queries 处理如下：
对于 i=0: queries[i]=3, P=[1,2,3,4,5], 3 在 P 中的位置是 2，接着我们把 3 移动到 P 的起始位置，得到 P=[3,1,2,4,5] 。
对于 i=1: queries[i]=1, P=[3,1,2,4,5], 1 在 P 中的位置是 1，接着我们把 1 移动到 P 的起始位置，得到 P=[1,3,2,4,5] 。 
对于 i=2: queries[i]=2, P=[1,3,2,4,5], 2 在 P 中的位置是 2，接着我们把 2 移动到 P 的起始位置，得到 P=[2,1,3,4,5] 。
对于 i=3: queries[i]=1, P=[2,1,3,4,5], 1 在 P 中的位置是 1，接着我们把 1 移动到 P 的起始位置，得到 P=[1,2,3,4,5] 。 
因此，返回的结果数组为 [2,1,2,1] 。  
示例 2：

输入：queries = [4,1,2,2], m = 4
输出：[3,1,2,0]
示例 3：

输入：queries = [7,5,5,8,3], m = 8
输出：[6,5,0,7,5]


提示：

1 <= m <= 10^3
1 <= queries.length <= m
1 <= queries[i] <= m


### 题目解析

先用模拟法做一下，把数组设置成2*m长度，把数字都入到后面一半中。

一开始数组如下

arr = [0,0,0...,0, 1, 2, 3, 4, 5, 6,...,m]

维护一个索引map ind, 这样给出一个x可以很快知道在数组中的位置ind[x]。数一下前面有多少个数字。

然后把x放到前面 arr[m-i]=x, 原来的位置变成0arr[ind[x]]=0, ind[x] = m-i。

这样每次操作是O(n)。

主要的花费是在数数字上。

数数字的过程其实就是求某个位置前面有多少个数字。如果把有数字的地方设置成1，其实就是求前缀和。

可以设置arr = [0,0,0,0,0...,0, 1, 1, 1, 1,...,1] 一开始有数字的地方为1，没数字的地方为0，后续移动数字就是+1，-1操作。统计就用前缀和。

### AC代码

~~~go

func getLowBit(x int) int {
	return x & (-x)
}

type TreeArray struct {
	sum []int // 数组 编号从1开始
	n   int   // 数组大小
}

// 初始化，申请数组大空间。
func (t *TreeArray) Init(n int) {
	t.n = n
	t.sum = make([]int, n+1) // 0号元素不用，n号元素需要使用
}

//更新操作
func (t *TreeArray) Add(x, v int) {
	for ; x <= t.n; x += getLowBit(x) { // 沿着线往上找影响到的结点，并更新
		t.sum[x] += v
	}
}

// 求前缀和
func (t *TreeArray) QueryPre(i int) int {
	res := 0
	for ; i > 0; i -= getLowBit(i) {
		res += t.sum[i]
	}
	return res
}

// 求区间和
func (t *TreeArray) QuerySum(i, j int) int {
	return t.QueryPre(j) - t.QueryPre(i-1)
}

type NumArray struct {
	tree *TreeArray
	nums []int
}


func processQueries(queries []int, m int) []int {
	tree := &TreeArray{}
	tree.Init(2*m)
	ind := map[int]int{}
	// 设置初始值，下标从1开始
	for i:=m+1;i<=2*m;i++ {
		ind [i-m]=i
		tree.Add(i, 1)
	}

	ans := make([]int, len(queries))
	for i, v := range queries {
		ans [i] = tree.QueryPre(ind[v]-1) // 统计前面有多少数字
		tree.Add(ind[v], -1) // 删除数字
		ind[v] = m-i // 更新下标
		tree.Add(ind[v], 1)
	}

	return ans
}
~~~

## 练习4 配合二分法求解第K大 [求出 MK 平均值](https://leetcode-cn.com/problems/finding-mk-average/)

题目链接：https://leetcode-cn.com/problems/finding-mk-average/

### 题目大意

给你两个整数 m 和 k ，以及数据流形式的若干整数。你需要实现一个数据结构，计算这个数据流的 MK 平均值 。

MK 平均值 按照如下步骤计算：

如果数据流中的整数少于 m 个，MK 平均值 为 -1 ，否则将数据流中最后 m 个元素拷贝到一个独立的容器中。
从这个容器中删除最小的 k 个数和最大的 k 个数。
计算剩余元素的平均值，并 向下取整到最近的整数 。
请你实现 MKAverage 类：

MKAverage(int m, int k) 用一个空的数据流和两个整数 m 和 k 初始化 MKAverage 对象。
void addElement(int num) 往数据流中插入一个新的元素 num 。
int calculateMKAverage() 对当前的数据流计算并返回 MK 平均数 ，结果需 向下取整到最近的整数 。


示例 1：

输入：
["MKAverage", "addElement", "addElement", "calculateMKAverage", "addElement", "calculateMKAverage", "addElement", "addElement", "addElement", "calculateMKAverage"]
[[3, 1], [3], [1], [], [10], [], [5], [5], [5], []]
输出：
[null, null, null, -1, null, 3, null, null, null, 5]

解释：
MKAverage obj = new MKAverage(3, 1); 
obj.addElement(3);        // 当前元素为 [3]
obj.addElement(1);        // 当前元素为 [3,1]
obj.calculateMKAverage(); // 返回 -1 ，因为 m = 3 ，但数据流中只有 2 个元素
obj.addElement(10);       // 当前元素为 [3,1,10]
obj.calculateMKAverage(); // 最后 3 个元素为 [3,1,10]
                          // 删除最小以及最大的 1 个元素后，容器为 [3]
                          // [3] 的平均值等于 3/1 = 3 ，故返回 3
obj.addElement(5);        // 当前元素为 [3,1,10,5]
obj.addElement(5);        // 当前元素为 [3,1,10,5,5]
obj.addElement(5);        // 当前元素为 [3,1,10,5,5,5]
obj.calculateMKAverage(); // 最后 3 个元素为 [5,5,5]
                          // 删除最小以及最大的 1 个元素后，容器为 [5]
                          // [5] 的平均值等于 5/1 = 5 ，故返回 5


提示：

3 <= m <= 10^5
1 <= k*2 < m
1 <= num <= 10^5
addElement 与 calculateMKAverage 总操作次数不超过 10^5 次。

### 题目解析

题目每次要求统计的其实是当前m个数字。

那我们只要对当前每种数字进行计数就可以了。

设计数数组为cnt[] 长度是10^5。

每次来一个新数字x , cnt[x]++。

然后把前第m个数字 q, cnt[q]--。

求解平均值，先找到前面K个，后面K个。然后减掉。

这里要解决2个问题：

1. 每次有新数据进来后怎么知道前m个是什么。
2. 当前m个数字中最小K个。

问题1用队列可以解决，当队列到达m个时，就开始出队。

问题2 看图

![普通法查找第K小](/Users/bytedance/Documents/个人生活/ACMAlgorithms/常用算法模板/树状数组/普通法查找第K小.png)

其实就是找到 i 使得 cnt[1]+cnt[2]+cnt[3]+...+cnt[i]>=K 。

前面就是一个前缀和，前缀和是可以O(1)求出的。再配合2分，可以很快找到i，也可以知道前K个数字的和。



### AC代码

~~~go

/*

 */
type ArrTree struct {
	c []int
	n int
}

func ConstructArrTree(n int) *ArrTree {
	return &ArrTree{
		c: make([]int, n+1),
		n: n,
	}
}

func lowbit(x int) int { return x & (-x) }

func (tree *ArrTree) update(x, y int) {
	if x == 0 {
		return
	}
	for i := x; i <= tree.n; i += lowbit(i) {
		tree.c[i] += y
	}
}
func (tree *ArrTree) getsum(x int) int {
	ans := 0
	for i := x; i > 0; i -= lowbit(i) {

		ans += tree.c[i]
	}
	return ans
}

type MKAverage struct {
	sumTree   *ArrTree // 计总和 最小的k个数字的和
	countTree *ArrTree // 计数
	m, k      int
	arr       []int
}

func Constructor(m int, k int) MKAverage {
	return MKAverage{
		sumTree:   ConstructArrTree(int(1e5)+10),
		countTree: ConstructArrTree(int(1e5)+10),
		m:         m,
		k:         k,
		arr:       []int{},
	}
}

func (this *MKAverage) AddElement(num int) {
	this.arr = append(this.arr, num)
	this.sumTree.update(num, num)
	this.countTree.update(num, 1)

	if len(this.arr) > this.m {
		this.sumTree.update(this.arr[0], -this.arr[0])
		this.countTree.update(this.arr[0], -1)
		this.arr = this.arr[1:]
	}
}

// 二分法最小的第K个数字
func findLow(k int, tree *ArrTree) int {
	l, r := 0, tree.n
	best := 0

	for l <= r {
		mid := (l + r) / 2
		if tree.getsum(mid) <= k {
			best = mid
			l = mid + 1
		} else {
			r = mid - 1
		}
	}

	return best
}


func (this *MKAverage) CalculateMKAverage() int {
	if len(this.arr) < this.m {
		return -1
	}
	// 这里查找的是最小的K个，和最小的m-k个。
	low, high:= findLow(this.k, this.countTree), findLow(this.m-this.k, this.countTree)

	sum := this.sumTree.getsum(high) - this.sumTree.getsum(low)
	sum -= (low+1)*(this.k-this.countTree.getsum(low)) // 需要减掉重复的数字
	sum += (high+1)*(this.m-this.k- this.countTree.getsum(high)) // 需要加上少掉的部分。

	return sum/(this.m-2*this.k)
}
~~~

# 七、总结

## 主要内容：

1. 之前有介绍过[前缀和](https://mp.weixin.qq.com/s/iGSFLnttgy9nM2lvRYZVWQ)，它的缺点只能处理固定的数组。树状数组就是用来处理可变数组区间求和问题。

   树状数组利用二进制思想对数组进行分组，可以做到log(n)复杂度完成一次更新和查询。

2. 作用：1）求动态数组区间和；2)经常可以用来解决计数问题 ；3）配合二分思想可以做很多动态数组查询问题，比如第K大数字，最大值，最小值。

在使用树状数组时，最大的特点是就是数组的长度是固定的，所以经常要对数据进行离散化。然后，转化到树状数组模型上。

笔者水平有限，有写得不对或者解释不清楚的地方还望大家指出，我会尽自己最大努力去完善。

下面我精心准备了几个流行网站上的题目（首先AK F.\*ing leetcode），给大家准备了一些题目，供大家练习参考。干他F.\*ing (Fighting?)。

## 推荐阅读

二维数组情况可以参考

- https://baike.baidu.com/item/%E4%BA%8C%E7%BB%B4%E6%A0%91%E7%8A%B6%E6%95%B0%E7%BB%84/20415329?fr=aladdin

- https://blog.csdn.net/zzti_xiaowei/article/details/81053094

# 八、实战训练

## 代码基础训练题

光说不练假把式，学完了怎么也要实操一下吧，快快动用把刚才那4题A了。

1. 可变区间求和应用 https://leetcode-cn.com/problems/range-sum-query-mutable/
2. 离散化计数：https://leetcode-cn.com/problems/count-number-of-teams/
3. 离散化计数：https://leetcode-cn.com/problems/queries-on-a-permutation-with-key/
4. 配合二分法求解第K大: https://leetcode-cn.com/problems/finding-mk-average/

## AK leetcode

leetcode相关题目都在下面了，拿起武器挨个点名呗。

https://leetcode-cn.com/tag/binary-indexed-tree/problemset/ 树状数组相关题目

----

做完以上还觉得不过瘾，我给大家还准备了一些。

## 大神进阶

也可以去vjudge https://vjudge.net/problem 搜索相关题号

hdu

以下将序号替换就是题目链接。

1. http://acm.hdu.edu.cn/showproblem.php?pid=1166
2. http://acm.hdu.edu.cn/showproblem.php?pid=1541
3. http://acm.hdu.edu.cn/showproblem.php?pid=1556
4. http://acm.hdu.edu.cn/showproblem.php?pid=2642
5. http://acm.hdu.edu.cn/showproblem.php?pid=3465
6. http://acm.hdu.edu.cn/showproblem.php?pid=4339
7. http://acm.hdu.edu.cn/showproblem.php?pid=6274

Poj

以下将序号替换就是题目链接。

1. http://poj.org/problem?id=1195
2. http://poj.org/problem?id=1990
3. http://poj.org/problem?id=2155
4. http://poj.org/problem?id=2299
5. http://poj.org/problem?id=2481
6. http://poj.org/problem?id=3067
7. http://poj.org/problem?id=3321
8. http://poj.org/problem?id=3468

