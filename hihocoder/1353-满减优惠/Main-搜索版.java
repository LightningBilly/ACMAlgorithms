import java.util.Scanner;

public class Main {
	static int x=0;
	static int arr[] = new int[30];
	static int ans=-1;
	static public void main(String args[])
	{
		Scanner cin = new Scanner(System.in);

		int n=cin.nextInt();
		x=cin.nextInt();

		for(int i=0;i<n;++i)arr[i]=cin.nextInt();

		dfs(n-1, 0);
		System.out.println(ans);
	}

	static public void dfs(int n, int sum)
	{
		if(n==-1)
		{
			if(sum>=x && (ans==-1 || sum<ans))ans=sum;
			return;
		}

		if(ans>0 && sum>=ans)return;

		dfs(n-1, sum);
		dfs(n-1, sum+arr[n]);
	}
}