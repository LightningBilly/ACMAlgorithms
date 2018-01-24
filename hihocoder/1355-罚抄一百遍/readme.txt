题目链接 ：http://hihocoder.com/problemset/problem/1355

解题思路：
根据题意，在本题中单词有用的属性是长度。
在每一行的开始处有两个特征，一个是是否有空格，一个是第几个单题为开始。
随着行数的增加肯定会出现循环状态，我们的着要任务是求出循环。
由于文本不超过100个字符，也就是说最多不超过100个单词，那么加有空格的状态，最多有200种状态，再加上空行，最多300多行就会出现循环。即我们最多模拟300多次就可以停止了。所以复杂度是常数级的。
再来看单行中怎么模拟。有两种情况
1.行长度不足以放下一整个句子，这个很简单，只能一个一个放，
3. 行长度可以放下多个整句，这里如果一个一个放肯定会超时，可以直接整句放，留下最后一部分再模拟。

这样我们就可以求出循环的一个节点在哪里出现。
接下来再去找最后一行在哪里
1.先把没有重复的数过去，如果在没有重复部分就足以完成任务了，直接返回行数。
2. 否则 统计出循环部分可以抄多少遍，然后就可以用除法直接得到重复部分的行数，
3. 最后留出一小部分单独统计行数。得到最后一行在循环部分的哪里，
4. 利用之前循环的信息计算出列号 以下是一些例子，最后一个是比较特殊的

注意：
要用长整型，考虑行长度与单词相等的情况

123456789
Good good
 study   
day day   
up Good  
good     
study day
 day up  
Good good


10 9
a b c dd ee f g hhhhh

123456789
a b c dd 
ee f g   
hhhhh a b
 c dd ee 
f g hhhhh
 a b c dd
 ee f g
hhhhh a b

2 5
a b c dd ee f g hhhhh iiiii

12345
a b c
 dd  
ee f 
g
hhhhh

iiiii
a b c



test cases

1000000001 9
Good good study day day up

1000000000 1000000000
Good good study day day up

2 1000000000
Good good study day day up

1 9
a b c dd ee f g hhhhh

2 9
a b c dd ee f g hhhhh

3 9
a b c dd ee f g hhhhh 

1000 1
a

1 5
a b c dd ee f g hhhhh iiiii
