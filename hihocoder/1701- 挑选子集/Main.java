
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
	
	static int MOD = 1000000009;
	
	public static void main(String[] args){
		Scanner cin  = new Scanner(System.in);
		
		int n=cin.nextInt();
		int m=cin.nextInt();
		int k=cin.nextInt();
		
		int dp[][] = new int [110][110];
		
		int num[] = new int [n];
		for(int i=0;i<n;++i)
		{
			num[i]=cin.nextInt();
		}
		
		Arrays.sort(num);
		for(int i=1;i<=n;++i)//长度为i
		{
			for(int j=0;j<n;++j)//以第j个元素结尾
			{
				if(i==1)
				{
					dp[i][j]=1;
					continue;
				}
				
				for(int pre=0;pre<j;++pre)
				{
					if((num[j]-num[pre])%k!=0)continue;
					dp[i][j]= (dp[i][j]+dp[i-1][pre])%MOD;
				}
			}
		}
		
		int ans=0;
		for(int i=0;i<n;++i) ans = (ans+dp[m][i])%MOD;
		
		System.out.println(ans);
    }
	
}

/*
5 3 2  
1 2 5 4 3

5 3 1  
1 2 5 4 3

5 4 1  
1 2 5 4 3

2 2 1
1 1

4 2 1
1 1 1 1

10 3 2
1 2 3 4 5 6 7 8 9 10

*/