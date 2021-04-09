package main
import "fmt"


func abs(a int) int {
	if a<0 {return -a}
	return a
}


func min(a,b int) int {
	if a<b {return a}
	return b
}


func max(a,b int) int {
	if a>b {return a}
	return b
}


队列 
https://github.com/chain-zhang/gomo/blob/master/dstr/queue.go


// 树状数组
type TreeArr struct {
	n   int
	arr []int
}

func (t *TreeArr) InitTreeArr(n int) {
	t.n = n
	t.arr = make([]int, n+10)
}

func (t *TreeArr) LowBit(x int) int {
	return x & (-x)
}

func (t *TreeArr) Update(x, v int) {
	for ; x > 0 && x <= t.n; x += t.LowBit(x) {
		t.arr[x] += v
	}
}

func (t *TreeArr) Sum(x int) int {
	sum := 0
	for ; x > 0; x -= t.LowBit(x) {
		sum += t.arr[x]
	}

	return sum
}