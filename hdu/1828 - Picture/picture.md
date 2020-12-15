
# [hdu1828] picture
* http://acm.hdu.edu.cn/showproblem.php?pid=1828

### 题目描述

在墙上贴着很多长方形状的海报、照片和图片。他们的边都是竖起或者水平的。每个长方形可以被其他长方形所覆盖。所有长方形所组成图形的边界长度为周长。给出长方形信息，计算周长。

![img](/Users/chenbinbin/Documents/show something/self-advance/公众号/imag/hdu/1828-1.jpg)

![img](/Users/chenbinbin/Documents/show something/self-advance/公众号/imag/hdu/1828-2.jpg)

如上图，图2是图1中所有长方形的周长。

### 输入

第一行是一个整数n，表示长方形的个数。

接下来每行有2组x,y整数坐标，第1组表示长方形左下解坐标，第2组表示长方形右上解坐标。

0<=n<5000

x,y 的取值范围是 [-10000,10000] ，每个长方形的面积都是正数。



### 输出

一个非负整数，表示周长。




### 题目剖析 

这是一道与图形学相关的题目。难点在于如何让计算识别出轮廓内部的线段和时间复杂度的优化。

#### 计算机如何识别轮廓和内部线段

计算机是通过扫描线的方式来识别轮廓的。

来看一个例子，

![image-20200411213942018](/Users/chenbinbin/Documents/show something/self-advance/公众号/imag/hdu/1828-3.png)

来看水平方向的线条，红圈里的线段是内部线段，可以发现一个规则。内部线段都是被其他矩形所包围的，或者至少说被一个矩形的2条边所夹住。

扫描过程如下

从上到下扫描，如果遇到一个矩形的第一条边则先检查该边在X轴上没有被覆盖的范围就是有效轮廓，将这一条投影到X轴上。

如果遇到同一个矩形的第二条边，则将第一条边的投影取消，然后检查该边在X轴上没有被覆盖的范围就是有效轮廓。



### 解题思路

### 方法一 哈希表查找法

#### > 分析

> 123
>
> 

* 这道题是一个查找问题。可以通**过哈希表**加速
* 可以通过遍历数组中的每个元素`nums[i]`，查找`target-nums[i]`
* 查找`target-nums[i]`，可以遍历整个数组`nums`，总的时间复杂度为`$O(n^2)$`；也可以使用哈希表记录已查元素及其索引，中的时间复杂度为`$O(n)$`

#### 思路

* 创建哈希表：`<nums[i], index>`。用于存储元素及其索引
* 遍历数组的每个元素`nums[i]`。检查哈希表中是否存在`target-nums[i]`，如果存在，返回两个元素的索引；否则，将当前元素存储到哈希表中，处理下一个元素
* 如果遍历完所有元素，查找失败，返回空数组

#### 注意

* 边界检查：数组为空
* 处理查找失败情况

#### 知识点

* 数组
* 查找
* 哈希表

#### 复杂度

* 时间复杂度：O(n)
* 空间复杂度：O(n)

#### 参考
* https://www.cnblogs.com/grandyang/p/4130379.html

#### 代码实现

```cpp
//#include <unordered_map>
//
//using namespace std;

class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        // 输入检查
        if(nums.empty())
            return {};
        // 创建哈希表 <num, index>，加速查找
        unordered_map<int, int> m;
        for(int i=0; i<nums.size(); i++){
            if(m.count(target - nums[i]))
                return {i, m[target-nums[i]]};
            m[nums[i]] = i; // <num, index>
        }
        return {};
    }
};
```

### 方法二 双指针法

#### 分析

- 先对数组的排序，再用双指针法从两端开始慢慢往中间移
- 复杂度：排序为$O(nlogn)$，遍历数据为$O(n)$，总体为$O(nlogn)$

#### 思路

- 先对数组（带下标信息）排序
- 初始i=0, j=len(arr)-1
- 终止条件 i>=j
- 如果 arr[i].val+arr[j].var=target 返回 {arr[i].ind, arr[j].ind} 作为结果
- 如果arr[i].val+arr[j].var<target, 由于arr[i].val < arr[j].var, 故i++
- 如果arr[i].val+arr[j].var>target, 由于arr[i].val > arr[j].var, 故j--

#### 注意

- 题目要的是下标，所以数字与下标要关联排序
- 加减过程中结果有可能超出int32
- 排序后，arr[i].ind, arr[j].ind 大小关系不是一定的，在最后返回时要做判断

#### 知识点

- 数组
- 排序
- 双指针法

#### 复杂度

- 时间复杂度：$O(nlogn)​$
- 空间复杂度：$O(n)$

#### 参考

#### 代码实现

```cpp

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long lld;

vector<int> A;

class LinesTree {
public:
    int i,j;
    int count, line;
    int lbd, rbd;
    int m;
    LinesTree *leftChild, *rightChild;

    void Build(int l, int r);
    void Insert(int l, int r);
    void Delete(int l, int r);
    void UpdateM();
    void UpdateLine();
    // ~LinesTree(){}
};

void LinesTree::Build(int l, int r)
{
    this->i = l;
    this->j = r;
    this->count = 0;
    if (r-l<=1)
    {
        this->leftChild = this->rightChild = NULL;
    }
    else
    {
        int k=(l+r)>>1;
        LinesTree *lc = new(LinesTree);
        lc->Build(l, k);
        this->leftChild = lc;

        LinesTree *rc = new(LinesTree);
        rc->Build(k, r);
        this->rightChild = rc;
    }
}

void LinesTree::Insert(int l, int r)
{
    if (l<=A[i] && A[j]<=r)
    {
        count++;
        return;
    }
    if (j-i==1)
        return;

    int k = (i+j)>>1;
    if (l<A[k]) leftChild->Insert(l,r);
    if (A[k]<r) rightChild->Insert(l, r);
}

void LinesTree::Delete(int l, int r)
{
    if (l<=A[i] && A[j]<=r)
    {
        count--;
        return;
    }
    if (j-i==1)
        return;

    int k = (i+j)>>1;
    if (l<A[k]) leftChild->Delete(l,r);
    if (A[k]<r) rightChild->Delete(l, r);
}

void LinesTree::UpdateM()
{
    if(count > 0)
    {
        m = A[j] - A[i];
        return;
    }

    if (j-i==1){
        m=0;
        return;
    }

    leftChild->UpdateM();
    rightChild->UpdateM();
    m = leftChild->m + rightChild->m;
}

void LinesTree::UpdateLine()
{
    if(count > 0)
    {
        lbd = rbd = line = 1;
        return;
    }

    if (j-i==1){
        lbd = rbd = line = 0;
        return;
    }

    leftChild->UpdateLine();
    rightChild->UpdateLine();
    line = leftChild->line + rightChild->line - leftChild->rbd*rightChild->lbd;
    lbd = leftChild->lbd;
    rbd = rightChild->rbd;
}

class Event{
public:
    int x, y1, y2;
    int op;
    Event(){}
    Event(int x, int y1, int y2, int op):x(x), y1(y1), y2(y2), op(op){}

    bool operator < (const Event &b) const
    {
        if(x!=b.x) return x<b.x;
        return op>b.op;
    }

    string ToString()
    {
        string res = "";
        res += "x: "+ to_string(x) + "|";
        res += "y1: "+ to_string(y1) + "|";
        res += "y2: "+ to_string(y2) + "|";
        res += "op: "+ to_string(op) + "|";

        return res;
    }
};

void solve()
{
    int n;

    while(scanf("%d", &n)!=EOF)
    {
        vector<Event> event(2*n);
        A.clear();
        for(int i=0;i<n;i++)
        {
            int x1, y1, x2, y2;
            scanf("%d%d%d%d", &x1, &y1,&x2,&y2);
            A.push_back(y1);
            A.push_back(y2);
            event[2*i] = Event(x1, y1, y2, 1);
            event[2*i+1] = Event(x2, y1, y2, -1);
        }

        sort(event.begin(), event.end());
        sort(A.begin(), A.end());
        A.erase(unique(A.begin(), A.end()), A.end());

        auto root = new(LinesTree);
        root->Build(0, A.size()-1);
        lld lastM = 0, lastLine=0, lastX = event[0].x;
        lld ans = 0;
        lld hans = 0;
        for (auto ev: event)
        {
            if (ev.op>0) root->Insert(ev.y1, ev.y2);
            else root->Delete(ev.y1, ev.y2);
            root->UpdateLine();
            root->UpdateM();

            hans += lastLine*2*(ev.x-lastX); // 横向长度
            //ans += lastLine*2*(ev.x-lastX); // 横向长度
            ans += abs(root->m - lastM); // 纵向长度
            lastM = root->m;
            lastLine = root->line;
            lastX = ev.x;
        }

        //printf("%lld\n", ans);
        //printf("%lld\n", hans);
        printf("%lld\n", hans+ans);
    }
}
/*
7
-15 0 5 10
-5 8 20 25
15 -4 24 14
0 -6 16 4
2 15 10 22
30 10 36 20
34 0 40 16

 2
 1 -1 5 1
 2 -2 3 2
 */

int main() {

    solve();
    return 0;
}

```



# 相关题目

<https://leetcode-cn.com/problems/3sum/>

