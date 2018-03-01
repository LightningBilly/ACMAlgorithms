
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
	
    public static void main(String[] args) {
        Scanner scan = new Scanner(System.in);
        Map<Integer, Integer> cnt = new HashMap<Integer, Integer>();
        TreeSet<Integer> numset = new TreeSet<Integer>();
        TreeSet<int[]> set = new TreeSet<int[]>(new Comparator<int[]>() {

			@Override
			public int compare(int[] o1, int[] o2) {
				if(o1[1] != o2[1]) return o1[1]>o2[1] ? -1 : 1;
				if(o1[0]==o2[0])return 0;
				return o1[0]<o2[0] ? -1 : 1;
			}        	
		});
        /*
        int [] a = {1,2};
        set.add(a);
        set.add(new int[]{2,3});
        
        System.out.println(set.size());
        //set.remove(new int[]{2,3});
        System.out.println(Arrays.toString(set.first()));
        */
        
        int n=scan.nextInt();
        for(int i=0;i<n;++i)
        {
        	int a = scan.nextInt();
        	if(!cnt.containsKey(a))
        	{
        		cnt.put(a, 1);
        	}
        	else 
        	{
        		cnt.put(a, cnt.get(a)+1);
        	}
        	
        	numset.add(a);
        }
        
        for (Map.Entry<Integer, Integer> kv : cnt.entrySet()) {
			set.add(new int[]{kv.getKey(), kv.getValue()});
			//System.out.println(kv.getKey() + ", " +kv.getValue());
		}
        //System.out.println(Arrays.toString(set.first()));
        
        int ans[] = new int[n];
        int left = n;
        boolean succ = true;
        for(int i=0;i<n;++i, left--)
        {
        	int m = findNext(numset, set, left, ans, i);
        	
        	if(m<0)
        	{
        		succ = false;
        		break;
        	}
        	
        	ans[i] = m;
        	set.remove(new int[]{m, cnt.get(m)});
        	cnt.put(m, cnt.get(m)-1);
        	if(cnt.get(m)==0) numset.remove(m);
        	else set.add(new int[]{m, cnt.get(m)});
        }
        
        if(!succ)
        {
        	System.out.println(-1);
        	return;
        }
        
        for(int i=0;i<n;++i)
        {
        	if(i!=0) System.out.print(" ");
        	System.out.print(ans[i]);        	
        }
        
        System.out.println();
    }
    
    public static int findNext(TreeSet<Integer> numset, TreeSet<int[]> set, int left, int ans[] , int i)
    {
    	int m = set.first()[1];
    	if(m*2-1 > left)
    	{
    		return -1;//代表无解
    	}
    	else if(m*2-1 == left){//代表最多的数要马上输出
    		return set.first()[0];
    	}
    	
    	if(i>0 && numset.first()==ans[i-1])
    	{
    		Iterator<Integer> ite= numset.iterator();
    		ite.next();
    		return ite.next();
    	}
    	return numset.first();
    }
}

/*
4  
2 1 3 3

*/
