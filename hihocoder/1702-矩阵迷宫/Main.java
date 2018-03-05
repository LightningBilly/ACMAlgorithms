
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Scanner;
import java.util.TreeSet;
import java.util.Map;

/**
 * Created by cbb on 2017/10/23.
 */
public class Main {

	static int dir[][] = {{0, 1}, {1, 0}};
	static int maze[][];
	static int dp[][][][];
	static int maxStep = 16;
    public static void main(String[] args)
    {
    	Scanner cin = new Scanner(System.in);
    	
    	int n=cin.nextInt();
    	
    	maze = new int[n][n];
    	dp = new int[n][n][maxStep][2];
    	
    	for(int i=0;i<n;++i)
    	{
    		for(int j=0;j<n;++j) {
    			maze[i][j]=cin.nextInt();
    			for(int s=0;s<maxStep;s++)
    			{
    				dp[i][j][s][0] = dp[i][j][s][1]=Integer.MAX_VALUE;
    			}
    		}
    	}
    	
    	int ans = getAns(n);
    	System.out.println(ans);
    }
    
    static int getAns(int n)
    {
    	dp[0][0][0][0]=maze[0][0];
    	dp[0][0][0][1]=maze[0][0];
    	
    	for(int i=0;i<n;++i)
    	{
    		for(int j=0;j<n;++j)
    		{
    			for(int s=0;s<maxStep;++s)
    			{
    				for(int d=0;d<2;++d)
    				{
    					int t=dp[i][j][s][d];
    					if(t==Integer.MAX_VALUE)continue;
    					
    					//向原方向走
    					int cor[] = getNextStep(i, j, d, 0);
    					if(isInRange(cor, n))
    					{
    						dp[cor[0]][cor[1]][s][d] = Math.min(dp[cor[0]][cor[1]][s][d], t + maze[cor[0]][cor[1]]);
    					}
    					
    					//变换方向
    					cor = getNextStep(i, j, d, 1);
    					if(s+1<maxStep && isInRange(cor, n))
    					{
    						dp[cor[0]][cor[1]][s+1][d^1] = Math.min(dp[cor[0]][cor[1]][s+1][d^1], t + maze[cor[0]][cor[1]] + (1<<s));
    					}
    				}
    			}
    		}
    	}
    	
    	int ans=Integer.MAX_VALUE;
    	
    	for(int s=0;s<maxStep;++s)
    	{
    		for(int d=0;d<2;++d)
    		{
    			ans = Math.min(dp[n-1][n-1][s][d], ans);
    		}
    	}
    	
    	return ans;
    }
    
    static boolean isInRange(int cor[], int n)
    {
    	if(cor[0]>=n || cor[1]>=n)return false;
    	return true;
    }
    
    static int [] getNextStep(int i, int j, int d, int isForward/*是否原方向，0为是*/)
    {
    	d^=isForward;//是否变化方向，0不变，1变
    	int ans[] = {i+dir[d][0], j+dir[d][1]};
    	
    	return ans;
    }
}

/*
4  
2 1 3 3

5
1 2 3 4 5
5 4 3 2 1
3 4 2 1 5
3 2 3 4 1
1 1 1 1 1
5
1 1 3 4 5
5 1 1 2 1
3 4 2 1 5
3 2 3 4 1
1 1 1 1 1

5
1 1 3 4 5
5 1 1 2 1
3 4 1 1 5
3 2 1 4 1
1 1 1 1 1

*/
