
import "math/rand"
/*
type KV interface {
	GetKey() interface{}
	GetValue() interface{}
	Less(b KV) bool
}*/

type KV struct {
	k int
}

func (a KV) GetValue() int {
	return a.k
}

func (a KV) GetKey() int {
	return a.k
}

func (a KV) Less(b KV) bool {
	return a.k<b.GetKey()
}

const SKIPLIST_MAXLEVEL = 16

/*
以1/4的概率递增，最多32层
*/
func slRandomLevel() int {
	level := 1
	for (rand.Int() & 0xFFFF) < (0xFFFF >> 2) {
		level++
	}

	if level < SKIPLIST_MAXLEVEL {
		return level
	}
	return SKIPLIST_MAXLEVEL
}

// 利用跳表实现可排序的map
/*跳表节点定义*/
type leveInfo struct {
	// 前进指针
	forward *skiplistNode
	// 跨度
	span int64
}

type skiplistNode struct {
	// value
	elem KV
	// 后退指针
	backward *skiplistNode
	// 层
	level []leveInfo
}

type Skiplist struct {
	// 跳跃表头指针
	header *skiplistNode
	tail   *skiplistNode
	// 表中节点的数量
	length int64
	// 表中层数最大的节点的层数
	level int
}

func slCreateNode(level int, e KV) *skiplistNode {
	node := &skiplistNode{
		elem:     e,
		backward: nil,
		level:    make([]leveInfo, level),
	}
	return node
}

func NewSkipList() *Skiplist {
	list := &Skiplist{
		length: 0,
		level:  1,
		header: slCreateNode(SKIPLIST_MAXLEVEL, KV{0}),
		tail:   nil,
	}

	return list
}

/*
添加元素
*/
func (list *Skiplist) Add(e KV) {
	var (
		// 存储搜索路径
		update [SKIPLIST_MAXLEVEL]*skiplistNode
		x      *skiplistNode
		// 存储经过的节点跨度
		rank [SKIPLIST_MAXLEVEL]int64
	)

	var i int

	// 1. 逐步降级寻找目标节点，得到 "搜索路径"
	for x, i = list.header, list.level-1; i >= 0; i-- {
		// 前缀和rank[i]代表 update[i]之前经过的结点个数
		if i == list.level-1 {
			rank[i] = 0
		} else {
			rank[i] = rank[i+1]
		}

		// 比较elem与e 找到当前层比e小的最后一个结点
		for ; x.level[i].forward != nil && x.level[i].forward.elem.Less(e); x = x.level[i].forward {
			rank[i] += x.level[i].span
		}

		update[i] = x
	}

	// 2. 生成新节点高度，超过了当前最大高度需要对高出部分初始化
	level := slRandomLevel()
	for i = list.level; i < level; i++ {
		rank[i] = 0
		update[i] = list.header
		update[i].level[i].span = list.length
	}
	if level > list.level {
		list.level = level
	}

	// 3. 创建新节点
	x = slCreateNode(level, e)

	// 4. 重排向前指针
	for i = 0; i < level; i++ {
		x.level[i].forward = update[i].level[i].forward
		update[i].level[i].forward = x

		// 更新跨度
		x.level[i].span = update[i].level[i].span - (rank[0] - rank[i])
		update[i].level[i].span = rank[0] - rank[i] + 1
	}

	for i = level; i < list.level; i++ {
		update[i].level[i].span++
	}

	// 向后结点更新
	if update[0] != list.header {
		x.backward = update[0]
	}

	if x.level[0].forward != nil {
		x.level[0].forward.backward = x
	} else {
		list.tail = x
	}
	list.length++
}

// 删除一个结点
func (list *Skiplist) slDeleteNode(x *skiplistNode, update [SKIPLIST_MAXLEVEL]*skiplistNode) {
	for i := 0; i < list.level; i++ {
		// 被删除节点在第i层有节点，则update[i]为被删除节点的前一个节点
		if update[i].level[i].forward == x {
			// 步长 = 原步长 + 被删除节点步长 - 1（被删除节点）
			update[i].level[i].span += x.level[i].span - 1
			// 指针越过被删除节点
			update[i].level[i].forward = x.level[i].forward
		} else {
			// 被删除节点在第i层无节点，则 步长 = 原步长 - 1(被删除节点)
			update[i].level[i].span -= 1
		}
	}
	if x.level[0].forward != nil {
		// 更新被删除节点下一节点的后退指针
		x.level[0].forward.backward = x.backward
	} else {
		list.tail = x.backward
	}
	for list.level > 1 && list.header.level[list.level-1].forward == nil {
		list.level--
	}
	list.length--
}

// 删除结点
func (list *Skiplist) Deletee(e KV) bool {
	var (
		// 存储搜索路径
		update [SKIPLIST_MAXLEVEL]*skiplistNode
		x      *skiplistNode
	)

	x = list.header;
	for i := list.level-1; i >= 0; i-- {
		for ; x.level[i].forward != nil && x.level[i].forward.elem.Less(e); x = x.level[i].forward {}
		update[i] = x;
	}

	x = x.level[0].forward;
	if x!=nil && !x.elem.Less(e) && !e.Less(x.elem) {
		list.slDeleteNode(x, update);
		return true;
	}
	return false; /* not found */
}

// 遍历结点
func (list *Skiplist) Range(f func(e KV)) {
	for x := list.header.level[0].forward; x != nil; x = x.level[0].forward {
		f(x.elem)
	}
}



var cnt =0

func minimumDeviation(nums []int) int {
	cnt++
	// if cnt==76{return 0}
	m:=map[int]int{}
	odd := []int{};
	for _, n:= range nums {
		m[n]=1;
	}

	sl := NewSkipList()

	for k:= range m {
		sl.Add(KV{k})
		if (k&1)>0 {
			odd = append(odd, k)
		}
	}

	sort.Ints(odd);
	if len(m)==1 {return 0;}


	ans := sl.tail.elem.GetKey()-   sl.header.level[0].forward.elem.GetKey()

	for _, n:=range odd{
		sl.Deletee(KV{n})
		sl.Add(KV{2*n})
		t := sl.tail.elem.GetKey() -   sl.header.level[0].forward.elem.GetKey()
		if t< ans {
			ans = t
		}
	}

	for s :=sl.tail.elem.GetKey() ;(s&1)==0; s =sl.tail.elem.GetKey() {
		sl.Deletee(KV{s})
		sl.Add(KV{s>>1})
		t := sl.tail.elem.GetKey() -   sl.header.level[0].forward.elem.GetKey()
		if t< ans {
			ans = t
		}
	}

	return ans;
}