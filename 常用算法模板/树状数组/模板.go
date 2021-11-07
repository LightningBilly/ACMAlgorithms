
func getLowBit(x int) int {
	return x & (-x)
}

type TreeArray struct {
	sum []int // 数组 编号从1开始
	n   int   // 数组大小
}

// 初始化，申请数组大空间。
func (t *TreeArray) Init(n int) {
	t.n = n
	t.sum = make([]int, n+1) // 0号元素不用，n号元素需要使用
}

//更新操作
func (t *TreeArray) Add(x, v int) {
	for ; x <= t.n; x += getLowBit(x) { // 沿着线往上找影响到的结点，并更新
		t.sum[x] += v
	}
}

// 求前缀和
func (t *TreeArray) QueryPre(i int) int {
	res := 0
	for ; i > 0; i -= getLowBit(i) {
		res += t.sum[i]
	}
	return res
}

// 求区间和
func (t *TreeArray) QuerySum(i, j int) int {
	return t.QueryPre(j) - t.QueryPre(i-1)
}

