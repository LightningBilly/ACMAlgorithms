package main
import "fmt"


func abs(a int) int {
	if a<0 {return -a}
	return a
}
func checkFunc(houses []int, heaters []int, radius int) bool {
	i, j:=0,0
	for ;i<len(houses) && j<len(heaters); {
		if abs(houses[i] - heaters[j])<=radius{
			i++
		}else {j++}
	}
	return i==len(houses)
}

func findRadius(houses []int, heaters []int) int {
	// 排序
	sort.Slice(houses, func(i, j int) bool {
		return houses[i]<houses[j]
	})
	sort.Slice(heaters, func(i, j int) bool {
		return heaters[i]<heaters[j]
	})
	best := int(10e9)
	left, right := 0, int(10e9)
	for left<= right {
		mid := (right+left)>>1
		if checkFunc(houses, heaters, mid) { // 可行解，贪心查看有没有更好解, 区间往小移
			best, right = mid, mid-1
		} else {
			left = mid+1
		}
	}

	return best
}

/*
[1,2,3]
[2]
[2]
[2]
[1,2,3,4]
[1,4]
[1,5]
[2]
[4,3,2,1]
[1,4]
*/}