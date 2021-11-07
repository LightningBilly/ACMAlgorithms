package main

import (
	"fmt"
)


func Partition(arr []int, low, high int) int {
	pivot := arr[low]
	for low<high {
		for low<high && arr[high] >= pivot {high--}// 一定要有等于判断，否则会死循环
		arr[low]=arr[high]
		for low<high && arr[low] <= pivot {low++} // 一定要有等于判断，否则会死循环
		arr[high]=arr[low]
	}

	arr[low]=pivot
	return low
}

func QuickSort(arr []int, low, high int) {
	if low>=high { // 少于一个元素，不用排序
		return
	}

	mid := Partition(arr, low, high) // 取到中间点
	QuickSort(arr, low, mid-1) // 排序左边
	QuickSort(arr, mid+1, high) // 排序右边
}


func getLeastNumbers(arr []int, k int) []int {
	low, high := 0, len(arr)-1
	for k>0 {// k 有可能为0
		mid := Partition(arr, low, high) // 对当前low, high进行排序，看mid-low与k的关系
		preNum := mid-low+1
		if  preNum == k { // arr[low, mid] 刚好满足K个元素
			return arr[:mid+1] // arr[mid]是最后一个元素 slice是前闭后开
		} 
		
		if preNum >k{ // 数量太多了mid往后的不用考虑了
			high = mid-1
		} else {
			low = mid+1
			// arr[low, mid] 已经被选中，k要减少
			k -= preNum
		}
	}
	
	return nil
}


func main() {
	arr := []int{10,6,7,12,18,12,1,2,4,3}
	QuickSort(arr, 0, len(arr)-1)
	fmt.Println(arr)

	arr = []int{1,1,1,1,1}
	QuickSort(arr, 0, len(arr)-1)
	fmt.Println(arr)


	arr = []int{}
	QuickSort(arr, 0, len(arr)-1)
	fmt.Println(arr)

	arr = []int{1}
	QuickSort(arr, 0, len(arr)-1)
	fmt.Println(arr)
}

