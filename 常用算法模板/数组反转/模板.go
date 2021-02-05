package main
import "fmt"

func isNeedSwap(b type) bool {
	
}

// 异或法
func swap(arr []type, i, j int) {
  // 注意i==j时以下算法会失效
  // 注意i==j时以下算法会失效
  // 注意i==j时以下算法会失效，想想为什么，临时变量法就没有这样的麻烦
  if i==j {return} // 注意i==j时以下算法会失效
	arr[i] = arr[i] ^ arr[j] // 保存2者的异或
	arr[j] = arr[i] ^ arr[j] // 将arr[j] 变成 原来arr[i]的值，arr[i]^arr[j]^arr[j] = arr[i],
	arr[i] = arr[i] ^ arr[j] // 现在arr[j]是原来的arr[i]，故arr[i]^arr[j]^arr[i] = arr[j]
}

// 反转arr区间[i,j]
func reverseCommon(arr []type, i, j int)  {
	for ;i<j; i, j = i+1, j-1 { // 每交换完一个都往里移一个
  	swap(arr, i, j)
  }
}

// 反转arr区间[i,j]中的特定元素
func reverseSpecial(arr []type, i, j int) {
	for ; i < j; i, j = i+1, j-1 { // 每交换完一个都往里移一个
		for ; i < j && !isNeedSwap(arr[i]); i++ { // 查找到第1个需要交换的元素
		}
		for ; i < j && !isNeedSwap(arr[j]); j-- { // 查找最后一个需要交换的元素
		}

		//if i < j {  // 这里可以去除判断，极端情况是 i==j 相当于没有交换
			swap(arr, i, j)
		//}
	}
}

/*
可以看出前面的反转数组reverseCommon是这个算法的特殊情况，
相当于isNeedSwap一直是true, 
所以里面的for循环不起作用
*/