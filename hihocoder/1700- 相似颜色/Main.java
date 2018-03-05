
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
	
	static int num[] = new int[300];// ¶ÔÓ¦Êý×Ö
	static String numdex = "0123456789abcdefg";
	
	public static void main(String[] args){
		makeNum();
		Scanner cin = new Scanner(System.in);
		String s = cin.next();
		
		System.out.print('#');
		for(int i=1;i<7;i+=2)
		{
			char c1 = s.charAt(i);
			char c2 = s.charAt(i+1);
			int k=num[c1]*16+num[c2];
			int x = getClose(k);
			System.out.print(numdex.charAt(x));
		}
    }
	
	static void makeNum()
	{
		for(int i='0';i<='9';++i) num[i]=i-'0';
		for(int i='a';i<='z';++i) num[i]=i-'a'+10;
	}
	
	static int getSq(int a, int b)
	{
		return (a-b)*(a-b);
	}
	
	static int getClose(int k)
	{
		int ans=0;
		for(int i=0;i<16;++i)
		{
			if(getSq(ans*16+ans, k)>getSq(i*16+i, k)) ans=i;
		}
		
		return ans;
	}

}

/*

#000000

*/