import java.util.Scanner;
import java.util.Arrays;

public class Main {
	static int x=0;
	static int arr[] = new int[30];
	static int back[] = new int[2000+10];
	static public void main(String args[])
	{
		Scanner cin = new Scanner(System.in);

		int n=cin.nextInt();
		x=cin.nextInt();
		int sum=0;
		for(int i=0;i<n;++i)
		{
			arr[i]=cin.nextInt();
			sum+=arr[i];
		}

		Arrays.fill(back, -1);
		back[0]=0;

		for(int i=0;i<n;++i)
		{
			for(int j=sum;j>=arr[i];--j)
			{
				if(back[j-arr[i]]==-1)continue;
				back[j]=0;
			}
		}

		int ans=-1;

		for(int i=x;i<=sum;i++)
		{
			if(back[i]==0)
			{
				ans=i;
				break;
			}
		}

		System.out.println(ans);

	}

}

/*

*/

