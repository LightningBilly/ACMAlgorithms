

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
	static long cnt[] = new long[N];// cnt[n]��������Ϊi���Ķ���������̬�ж����֣�ͨ��dp��� , cnt[0]=1; cnt[n] = sum(cnt[j-1]*cnt[n-j]) j=[1,n];
	
	public static void main(String [] args)
	{
		for(int i=0;i<N;++i) {
			getCnt(i);//�����cntע��Ҫ��С�����󡣺���Ҫ�õ�ǰ��Ľ��
			//System.out.println(i+" "+cnt[i]);
		}
		//System.out.println(cnt[30]);
		
		Scanner cin = new Scanner(System.in);
		int n=cin.nextInt();
		long k=cin.nextLong();//k���ܳ���int��Χ
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
	 * ������������ҳ���������еڴ�ĸ�
	 * Ȼ���ٱ�����������������
	 */
	static void findKthRoot(long k, int l, int r)
	{
		if(l>r) return;//����
		if(l==r){//��һ�����
			anslist[ind++]=orilist[l];
			return;
		}
		
		//�Ե�ǰ�����ֽ��д�С���������Ȼ����һ����ĳ����Ϊ���������ж����ֿ��ܣ������õ��Ƕ�
		PriorityQueue<int [] > que = new PriorityQueue<int[]>(new Comparator<int []>() {
			@Override
			public int compare(int[] o1, int[] o2) {
				// TODO Auto-generated method stub
				return o1[1]-o2[1];
			}
		});
		
		for(int i=l;i<=r;++i)//�����±��ֵ��Ԫ��
		{
			que.add(new int[]{i, orilist[i]});
		}
		
		int p=-1;//��¼root�±�
		while(!que.isEmpty())//��С����ȡ��
		{
			int [] f = que.poll();
			//System.out.println(Arrays.toString(f));
			int i=f[0];
			
			long t = cnt[i-l]*cnt[r-i];//�����Ե�ǰ�±������Ϊ���ж��������
			if(t>=k){//��ǰ�Ѿ����������������˳�
				p=i;
				break;
			}
			else k-=t;//���м���
		}
		
		//ȷ������ֵ
		anslist[ind++]=orilist[p];
		
		//������ÿ�仯1���������仯cnt[r-p]; ������Ҫ�ȵ�k/cnt[r-p]+(k%cnt[r-p]>0?1:0)
		findKthRoot(k/cnt[r-p]+(k%cnt[r-p]>0?1:0), l, p-1);
		
		//������ѡ�� (k-1)%cnt[r-p]+1
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