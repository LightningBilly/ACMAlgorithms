题目链接 ：http://poj.org/problem?id=1354

题目大意
有n个盒子编号从1到n，以及对应的钥匙，现将钥匙随机排列，然后先拿出前面两把，将剩下的钥匙分别装入到3-n的盒子中。然后用手中的两把钥匙去开对应的盒，拿打开的盒子中的钥匙继续去一对应的盒子。
问最终能把所有的盒子打开的钥匙排列有多少种。

题解：
这题可以用递推解决
当n=2时，答案显然为2.
设dp[i]为有i个盒子时的答案，
dp[2]=2,
dp[3]=2*dp[2]=4//相当于拿第三把钥匙和前面排列好的任意一个互换
dp[i]=(i-1)*dp[i-1] = 2*(i-1)!

注意：这里阶乘的规模较大要用到大数运算。