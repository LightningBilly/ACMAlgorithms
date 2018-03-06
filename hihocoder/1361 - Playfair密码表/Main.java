

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
	public static void main(String[] args)
	{
		Scanner cin = new Scanner(System.in);

		boolean [] used = new boolean[300];
		used['J']=true;
		String s = cin.next();

		char [][] maze = new char[6][6];
		int col=0, row=0;

		for(char c: s.toCharArray())
		{
			if(c=='J')c='I';
			if(used[c])continue;
			used[c]=true;
			maze[row][col++]=c;
			if(col==5){
				row++;
				col=0;
			}
		}

		for(char i='A';i<='Z';++i)
		{
			if(used[i])continue;
			maze[row][col++]=i;
			if(col==5){
				row++;
				col=0;
			}
		}

		for(int i=0;i<5;++i)
		{
			for(int j=0;j<5;++j)
			{
				System.out.print(maze[i][j]);
			}

			System.out.println();
		}
	}
}

/*



*/
