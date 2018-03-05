

import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Scanner;
import java.util.TreeSet;
import java.util.PriorityQueue;;

/**
 * Created by cbb on 2017/10/23.
 */
public class Main {

	static int N = 33;
	static int anslist[] = new int[N];
	static int ind=0;
	static int orilist[] = new int[N];
	static long cnt[] = new long[N];// cnt[n]代表结点数为i个的二叉树的形态有多少种，通过dp求得 , cnt[0]=1; cnt[n] = sum(cnt[j-1]*cnt[n-j]) j=[1,n];
	
	public static void main(String [] args)
	{
		for(int i=0;i<N;++i) {
			getCnt(i);//先求出cnt注意要从小到大求。后面要用到前面的结果
			//System.out.println(i+" "+cnt[i]);
		}
		//System.out.println(cnt[30]);
		
		Scanner cin = new Scanner(System.in);
		int n=cin.nextInt();
		long k=cin.nextLong();//k可能超出int范围
		for(int i=0;i<n;++i)orilist[i]=cin.nextInt();
		
		ind=0;
		findKthRoot(k, 0, n-1);
		for(int i=0;i<n;++i) System.out.println(anslist[i]);
	}
	
	static void getCnt(int n)
	{
		if(n==0)
		{
			cnt[n]=1;
			return;
		}
		
		cnt[n]=0;
		for(int i=1;i<=n;++i)
		{
			cnt[n]+=cnt[i-1]*cnt[n-i];
		}
	}
	
	/*
	 * 利用先序遍历找出所有情况中第大的根
	 * 然后再遍历左子树和右子树
	 */
	static void findKthRoot(long k, int l, int r)
	{
		if(l>r) return;//空树
		if(l==r){//就一佧结点
			anslist[ind++]=orilist[l];
			return;
		}
		
		//对当前的数字进行从小到大遍历，然后算一下以某数字为根的树还有多少种可能，这里用的是堆
		PriorityQueue<int [] > que = new PriorityQueue<int[]>(new Comparator<int []>() {
			@Override
			public int compare(int[] o1, int[] o2) {
				// TODO Auto-generated method stub
				return o1[1]-o2[1];
			}
		});
		
		for(int i=l;i<=r;++i)//加入下标和值的元组
		{
			que.add(new int[]{i, orilist[i]});
		}
		
		int p=-1;//记录root下标
		while(!que.isEmpty())//从小到大取出
		{
			int [] f = que.poll();
			//System.out.println(Arrays.toString(f));
			int i=f[0];
			
			long t = cnt[i-l]*cnt[r-i];//计算以当前下标的数字为根有多少种情况
			if(t>=k){//当前已经可以满足种数，退出
				p=i;
				break;
			}
			else k-=t;//从中减掉
		}
		
		//确定根的值
		anslist[ind++]=orilist[p];
		
		//左子树每变化1，整颗树变化cnt[r-p]; 左子树要先第k/cnt[r-p]+(k%cnt[r-p]>0?1:0)
		findKthRoot(k/cnt[r-p]+(k%cnt[r-p]>0?1:0), l, p-1);
		
		//右子树选第 (k-1)%cnt[r-p]+1
		findKthRoot((k-1)%cnt[r-p]+1, p+1, r);
		
	}
}

/*
5 1  
5  
4  
1  
3  
2
1 4 5 2 3

5 4
5  
4  
1  
3  
2
1 5 4 3 2

5 5
5  
4  
1  
3  
2
2 1 4 5 3

2 1
2 1

2 2
2 1

3 1
3 2 1
--1 2 3

3 2
3 2 1
--1 3 2

3 3
3 2 1
--2 3 1

3 4
3 2 1
--3 1 2

3 5
3 2 1
--3 2 1

10 16796
1 2 3 4 5 6 7 8 9 10



*/