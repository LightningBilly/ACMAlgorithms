

import java.util.Arrays;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedList;
import java.util.Scanner;
import java.util.TreeSet;
import java.util.PriorityQueue;

;

/**
 * Created by cbb on 2017/10/23.
 */
public class Main {

	static int N = 110;
	static long[] x = new long[N], y = new long[N];
	static long[][][] ans = new long[N][N][N];

	public static void main(String[] args) {
		Scanner cin = new Scanner(System.in);

		int n = cin.nextInt();
		int m = cin.nextInt();

		for (int i = 0; i < n; ++i) {
			x[i] = cin.nextLong();
			y[i] = cin.nextLong();
		}

		long res = dp(n, m);
		System.out.println(String.format("%.2f", res / 2.0));
	}

	public static long dp(int n, int m) {
		for (int i = 0; i < n; ++i) {
			for (int j = 0; j < n; ++j) {
				for (int k = 0; k <= m; ++k) {
					ans[i][j][k] = -1;
					if (i < j && k == 2)
						ans[i][j][k] = 0;
				}
			}
		}

		long res = -1;
		for (int k = 2; k < m; ++k) {
			for (int i = 0; i < n; ++i) {
				for (int j = i + k - 1; j < n; ++j) {
					if (ans[i][j][k] < 0)
						continue;
					for (int add = j + 1; add < n; ++add) {
						ans[i][add][k + 1] = Math.max(ans[i][add][k + 1],
								ans[i][j][k] + getArea(i, j, add));
					}
				}
			}
		}

		for (int i = 0; i < n; ++i) {
			for (int j = i + 1; j < n; ++j) {
				res = Math.max(res, ans[i][j][m]);
			}
		}

		return res;
	}

	// S=(1/2)*(x1y2*1+x2y3*1+x3y1*1-x1y3*1-x2y1*1-x3y2*1)
	// xi*yj+xj*yk+xk*yi-xi*yk-xj*yi-xk*yj
	public static long getArea(int i, int j, int k) {
		long res = x[i] * y[j] + x[j] * y[k] + x[k] * y[i] - x[i] * y[k] - x[j]
				* y[i] - x[k] * y[j];
		//System.out.println("tmp: " + res + ", " + i + ", " + j + ", " + k);
		return res;
	}

}

/*
 * 4 4 0 0 1 0 1 1 0 1
 * 
 * 6 3 -1 4 -2 1 -2 -2 1 -3 3 -1 1 2
 * -1 4 
 * -2 -2 
 * 3 -1
 * 6 4 -1 4 -2 1 -2 -2 1 -3 3 -1 1 2
 */