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

type node struct {
	l, r                  int // 代表树结点代表的区间范围
	leftChild, rightChild *node
	maxVal                int
}

type SegmentTree struct {
	nodes []node // 事先申请结点，加事内存分配
	root  int    //根结点编号
}

// 初始化线段树，分配内存大小, 构造树型
func (tree *SegmentTree) Init(l, r int) {
	tree.nodes = make([]node, (r-l+1)*4)
	tree.root = 1 //
	tree.buildNode(l, r, tree.root)
}

// 构造树型
func (tree *SegmentTree) buildNode(l, r, root int) *node {
	if l > r {
		return nil
	}

	mid := (l + r) >> 1
	tree.nodes[root].l, tree.nodes[root].r = l, r
	tree.nodes[root].maxVal = 0
	if l == r {
		return &tree.nodes[root]
	}
	// 构造左右子树
	tree.nodes[root].leftChild = tree.buildNode(l, mid, root<<1)
	tree.nodes[root].rightChild = tree.buildNode(mid+1, r, root<<1|1)
	return &tree.nodes[root]
}

func (tree *SegmentTree) InsertSegment(l, r, weight int) {
	tree.insert(l, r, weight, tree.root)
}

func (tree *SegmentTree) insert(l, r, weight, root int)  {
	if l > tree.nodes[root].r || r < tree.nodes[root].l {
		return
	}

	if l <= tree.nodes[root].l && tree.nodes[root].r <= r {
		tree.nodes[root].maxVal = weight
		return
	}

	tree.insert(l, r, weight, root<<1)
	tree.insert(l, r, weight, root<<1|1)
	/*
		更新本区间的最大值
	*/
	tree.nodes[root].maxVal = max(tree.nodes[root<<1].maxVal , tree.nodes[root<<1|1].maxVal)
}

func (tree *SegmentTree) Query(l, r int) int {
	return tree.query(l, r, tree.root)
}

func (tree *SegmentTree) query(l, r, root int) int {
	if l > tree.nodes[root].r || r < tree.nodes[root].l {
		return math.MinInt32
	}

	if l <= tree.nodes[root].l && tree.nodes[root].r <= r {
		return tree.nodes[root].maxVal
	}

	leftVal := tree.query(l, r, root<<1)
	rightVal := tree.query(l, r, root<<1|1)

	return max(leftVal, rightVal)
}

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

func maxResult(nums []int, k int) int {
	seg := &SegmentTree{}
	seg.Init(0, len(nums))
	dp := make([]int, len(nums)+10)
	for i, v := range nums {
		if i == 0 {
			dp[0] = v
		} else {
			maxPreVal := seg.Query(i-k, i-1)
			dp[i] = v+maxPreVal
		}
		seg.InsertSegment(i, i, dp[i])
	}

	return dp[len(nums)-1]
}

// 离散化，去重
func getInd (obstacles []int) map[int]int{
	temp := make([]int, len(obstacles))
	copy(temp, obstacles)
	sort.Ints(temp)
	ind := map[int]int{}
	for i, j := 0, 1; i<len(temp);i++ {
		if _, ok:= ind[temp[i]];ok {
			continue
		}
		ind[temp[i]]=j-1
		j++
	}
	
	return ind
}



func getInd (obstacles []int) (map[int]int, map[int]int){
	temp := make([]int, len(obstacles))
	copy(temp, obstacles)
	sort.Ints(temp)
	ind := map[int]int{}
	num := map[int]int{}
	for i, j := 0, 1; i<len(temp);i++ {
		if _, ok:= ind[temp[i]];ok {
			continue
		}
		ind[temp[i]]=j-1
		num[j-1]=temp[i]
		j++
	}

	return ind, num
}
