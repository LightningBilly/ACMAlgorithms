import java.util.Scanner;
import java.util.Arrays;
import java.util.Queue;
import java.util.LinkedList;

public class Main {
	static int N=500+10;
	static boolean water[][] = new boolean[N][N];
	static boolean vis[][] = new boolean[N][N];
	static int dis[][] = new int[N][N];
	static int B[] = new int[N];
	static int A[] = new int[N];
	static int dir[][] = {{0,1}, {0, -1}, {1, 0}, {-1, 0}};

	static public void main(String args[])
	{
		Scanner cin = new Scanner(System.in);

		int n = cin.nextInt();
		int m = cin.nextInt();

		for(int i=0;i<n-1;++i) B[i]=cin.nextInt();
		for(int i=0;i<m-1;++i) A[i]=cin.nextInt();
		
		int k=cin.nextInt();

		for(int i=0;i<k;++i)
		{
			int a=cin.nextInt();
			a--;
			int b=cin.nextInt();					
			--b;
			water[a][b]=true;
		}

		int cnt = cin.nextInt();

		while(cnt-->0)
		{
			int x,y,p,q;
			x=cin.nextInt();
			--x;
			y=cin.nextInt();
			--y;
			p=cin.nextInt();
			--p;
			q=cin.nextInt();
			--q;

			System.out.println(spfa(x, y, p, q, n, m));
		}
	}

	static class Point {
		int x, y;

		Point(){}
		Point(int a, int b)
		{
			x=a;
			y=b;
		}

		boolean isInRange(int n, int m)
		{
			return 0<=x && x<n && 0<=y && y<m;
		}
	}

	static public int calDis(Point a, Point b)
	{
		int sum=0;
		if(a.x != b.x)
		{
			int x = Math.min(a.x, b.x);
			sum+=B[x];
		}

		if(a.y!=b.y)
		{
			int y=Math.min(a.y, b.y);
			sum+=A[y];
		}

		return sum;
	}

	static public int spfa(int x, int y, int p, int q, int n, int m)
	{
		for(int i=0;i<n;++i)
		{
			for(int j=0;j<m;++j)
			{
				vis[i][j]=false;
				dis[i][j]=Integer.MAX_VALUE;
			}
		}

		Queue<Point> que = new LinkedList<Point>();

		que.add(new Point(x, y));
		vis[x][y]=true;
		dis[x][y]=0;

		while(!que.isEmpty())
		{
			Point f = que.peek();
			que.poll();
			vis[f.x][f.y]=false;

			for(int i=0;i<4;++i)
			{
				Point s = new Point(f.x+dir[i][0], f.y+dir[i][1]);
				if(!s.isInRange(n, m))continue;
				if(water[s.x][s.y])continue;
				int dist = calDis(f, s);
				if(dis[f.x][f.y]+dist < dis[s.x][s.y])
				{
					dis[s.x][s.y]=dis[f.x][f.y]+dist;
					if(!vis[s.x][s.y])
					{
						vis[s.x][s.y]=true;
						que.add(s);
					}
				}
			}
		}

		return dis[p][q]==Integer.MAX_VALUE?-1 : dis[p][q];
	}

}

/*

*/
