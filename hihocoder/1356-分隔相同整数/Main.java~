import java.util.Scanner;
import java.util.Arrays;
import java.util.Map;
import java.util.HashMap;

public class Main {
	static int m, n;
	static int N=1000+10;
	static long count[]=new long[N];
	static int head[]=new int[N], tail[]=new int[N];
	static int wordlen[]=new int[N],
			   wc/*单词总数*/, 
			   start, end/*循环开始与结束行号*/,
			   headword[]=new int[N], 
			   tl/*所有单词总长度*/;
	static long totline;//总行数，列数

	static public void main(String args[])
	{
		Scanner cin = new Scanner(System.in);
		wc=0;
		n = cin.nextInt();
		m = cin.nextInt();
		tl=0;
		while(cin.hasNext())
		{
			String s = cin.next();
			wordlen[wc++]=s.length();
			tl+=s.length();
			//System.out.println(s);
		}
		//System.out.println(wc);
		getCycle();
		//System.out.println(start+"-"+end);
		int line = findLastLine();
		int col = findCol(line);

		System.out.println(totline+" "+col);
	}

	//计算循环
	static public void getCycle()
	{
		Map<Integer, Integer> cycleline = new HashMap();
		tail[0]=1;
		count[0]=0;
		int cur=0, line=0;

		while(true)
		{
			line++;
			head[line]=tail[line-1]^1;
			count[line]=count[line-1];
			int len=m;

			headword[line]=cur;
			//System.out.println(line + "|tail:" + head[line] + ", len :" + len);
			if(len-head[line]<wordlen[cur])
			{
				tail[line]=1;
				continue;
			}
			len-= head[line]+wordlen[cur];
			//System.out.println(line + "|tail:" + tail[line] + ", len :" + len);
			cur=(cur+1) % wc;
			if(cur==0)++count[line];

			count[line] += len/(tl + wc); //单行有循环直接除掉，否则复杂度高
			len %= (tl+wc);

			while(len>wordlen[cur]) //一个一个往里加
			{
				len -= 1+wordlen[cur];
				cur = (cur+1)%wc;
				if(cur==0)++count[line];
			}

			tail[line]=(len>0) ? 1:0;
			//System.out.println(line + "|tail:" + tail[line] + ", len :" + len);
			//判断是否则重复有循环了
			//这里利用行首单词与行首是否有空格作为判断依据
			if(cycleline.containsKey(headword[line]*10 + head[line])){
				//System.out.println("c:" + line + "|" + (headword[line]*10 + head[line]) );
				break;
			}
			cycleline.put(headword[line]*10 + head[line], line);
		}

		start = cycleline.get(headword[line]*10 + head[line]);
		end = line-1;
	}

	//获取最后一行在循环中的位置， 以总行数
	static public int findLastLine()
	{
		int i=0;
		for(i=1;i<start && count[i]<n; ++i);
		n-= count[i-1];
		totline=i;
		if(i<start) return i;//还没有到重复的地方就结束了
		long cyclenum = count[end]-count[start-1];
		long cyclelines = end-start+1;
		long sum=start-1+((n-1)/cyclenum)*cyclelines;//起始加上中间重复的

		//剩下一小段未重复的
		n-= (n-1)/cyclenum*cyclenum;
		for(i=start; count[i]-count[start-1]<n;++i);
		n -= count[i-1]-count[start-1];//剩下n遍
		totline=sum+i-start+1;//有可能超整型
		return i;
	}

	//获取列数，有可能超整型
	static public int findCol(int line)
	{
		int len = head[line];
		int cur = headword[line];
		len += wordlen[cur];
		cur = (cur+1)%wc;

		if(cur==0) n--;

		if(n>1)
		{
			len += (n-1)*(tl + wc);
			n=1;
		}

		while(n>0)
		{
			len += 1+wordlen[cur];

			cur = (cur+1)%wc;
			if(cur==0)--n;
		}

		return len;
	}
}

/*
test cases

1000000001 9
Good good study day day up

1000000000 1000000000
Good good study day day up

2 1000000000
Good good study day day up

1 9
a b c dd ee f g hhhhh

2 9
a b c dd ee f g hhhhh

3 9
a b c dd ee f g hhhhh 

1000 1
a

1 5
a b c dd ee f g hhhhh iiiii

*/
