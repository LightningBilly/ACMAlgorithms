
func abs(a int) int {
	if a < 0 {
		return -a
	}
	return a
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

/*
前缀和
*/
type PreSum struct {
	preSum []int // 数组 preSum[i]=arr[0]+arr[1]+...+arr[i]。
}

// 常用有3种：
// 初始化数据, 通过arr, 初始化preSum
func (ps *PreSum) InitPre(arr []int) {
	ps.preSum = make([]int, len(arr))
	for i, v := range arr {
		if i == 0 {
			ps.preSum[0] = v
		} else {
			ps.preSum[i] = ps.preSum[i-1] + v
		}
	}
}

// 查询区间和
func (ps *PreSum) Sum(i, j int) int {
	if i <= 0 {
		return ps.preSum[j]
	}
	return ps.preSum[j] - ps.preSum[i-1]
}

// 查询最大子段和
// 这里是普通情况，有时候有特殊要求，比如子段不能为空，子段有可能为负数==
func (ps *PreSum) MaxSubArray() int {
	minPre := 0
	best := 0

	for _, v := range ps.preSum {
		best = max(best, v-minPre)
		minPre = min(minPre, v)
	}

	return best
}

