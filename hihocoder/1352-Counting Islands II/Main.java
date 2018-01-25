import java.util.Scanner;
import java.util.Arrays;
import java.util.Queue;
import java.util.LinkedList;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;
import java.util.HashSet;

public class Main {
	static final int N = (int)1e6+10;
	static int father[] = new int[N];
	static int dir[][] = new int[][]{ {1,0},{-1, 0},{0,1},{0,-1} };

	static int getIndex(int x, int y)
	{
		return 1000*x+y;
	}

	static int findFather(int x)
	{
		while(x!=father[x]) x=father[x];
		return x;
	}

	static boolean join(int x, int y)
	{
		if(father[x]==-1 || father[y]==-1)return false;
		x=findFather(x);
		y=findFather(y);
		if(x==y)return false;
		father[x]=y;
		return true;
	}

	static public void main(String args[])
	{
		Scanner cin = new Scanner(System.in);
		Arrays.fill(father, -1);

		int sum=0;

		int n=cin.nextInt();

		while(n-- > 0)
		{
			int x, y;
			x=cin.nextInt();
			y=cin.nextInt();

			int ind = getIndex(x, y);
			if(father[ind]!=-1)
			{
				System.out.println(sum);
				continue;
			}
			father[ind]=ind;
			sum++;

			for(int i=0;i<4;++i)
			{
				int nx = x+dir[i][0];
				int ny=y+dir[i][1];
				if(nx<0 || nx>=1000 || ny<0 || ny>=1000)continue;
				if(join(ind, getIndex(nx, ny)))sum--;
			}

			System.out.println(sum);
		}
	}
}

/*
test cases:

3
0 0
1 1
1 0

2
1 1
1 1

3
1 1
1 0
0 0
*/
