package main

import (
	"fmt"
)

func Merge(arr []int, low, high int) {
	copyArr := make([]int, high-low+1)
	copy(copyArr, arr[low:high+1])
	mid := (low + high) / 2
	for i, j, s := low, mid+1, low; i <= mid || j <= high; s++ {
		// j>high 时，只能取arr[i-low]
		// 否则，就尝试比较arr[i-low]<=arr[j-low]
		if j > high || (i <= mid && copyArr[i-low] <= copyArr[j-low]) {
			arr[s], i = copyArr[i-low], i+1
		} else { // 除以上情况就只能取arr[j-low]
			arr[s], j = copyArr[j-low], j+1
		}
	}
}

func MergeSort(arr []int, low, high int) {
	if low >= high { // 少于一个元素，不用排序
		return
	}

	mid := (low + high) / 2
	MergeSort(arr, low, mid)    // 先使左边有序
	MergeSort(arr, mid+1, high) // 再使右边有序
	Merge(arr, low, high)       // 合并左右两边，使整个有序
}

func main() {
	arr := []int{10, 6, 7, 12, 18, 12, 1, 2, 4, 3}
	MergeSort(arr, 0, len(arr)-1)
	fmt.Println(arr)

	arr = []int{1, 1, 1, 1, 1}
	MergeSort(arr, 0, len(arr)-1)
	fmt.Println(arr)

	arr = []int{}
	MergeSort(arr, 0, len(arr)-1)
	fmt.Println(arr)

	arr = []int{1}
	MergeSort(arr, 0, len(arr)-1)
	fmt.Println(arr)
}