package main

import "fmt"

type Node struct {
	Val        int // 值
	Next, Prev *ListNode
}

type List struct {
	head *ListNode // 头指针，是一个Node, 做一个空指针，实际第一个结点是head.Next开始，head.Next==nil 说明链表为空。
	// tail *ListNode // 如果想实现一个队列也可以定义tail 来实现
	// length int // 如果经常要访问长度，可以定义length，将长度计算均摊到增删算法中, 做到做到O(1)
}

// 常规操作
// 初始化List
func (l *List) Init(node *ListNode) {
	l.head = &Node{Val: 0, Next: node}
}

// 数组初始化
func (l *List) InitArr(arr ...int) {
	l.Init(nil)
	tail := l.head
	for _, n := range arr {
		tail.Next = &Node{Val: n}
		tail = tail.Next
	}
}

// 链表遍历
func (l *List) Visit(vis func(*ListNode)) {
	for cur := l.head.Next; cur != nil; cur = cur.Next {
		vis(cur)
	}
}

// 判断是否为空
func (l *List) IsEmpty() bool {
	return l.head.Next == nil
}

// 取长度
func (l *List) Length() int {
	sum := 0
	l.Visit(func(node *ListNode) {
		sum++
	})
	return sum
}

// 获取第i个结点
func (l *List) Get(i int) *ListNode {
	cur := l.head
	for ; i > 0 && cur != nil; i, cur = i-1, cur.Next {
	}
	return cur
}

// 删除第i个结点
func (l *List) Delete(i int) *ListNode {
	pre := l.Get(i - 1)
	if pre == nil || pre.Next == nil {
		// i已经超出了范围无效
		return nil
	}
	elem := pre.Next
	pre.Next = pre.Next.Next
	return elem
}

// 在最前面插入结点
func (l *List) InsertHead(elem *ListNode) {
	elem.Next = l.head.Next
	l.head.Next = elem
}

// 在第i个结点之后插入结点
func (l *List) Insert(i int, elem *ListNode) {
	pre := l.Get(i) // 定义pre 最终指向第i个结点,实际实现中用Get(i)
	if pre == nil {
		// i已经超出了范围无效
		return
	}
	elem.Next = pre.Next
	pre.Next = elem
}

// 反转链表, 可以采用头插法
func (l *List) Reverse() {
	pre := l.head.Next
	l.head.Next = nil
	for pre != nil { // 依次遍历
		temp := pre // 这里一定要先复制一份，要不然当前结点插入到新顺序后，就找不到下一结点了。
		pre = pre.Next
		l.InsertHead(temp)
	}
}

// 双指针相关
// 1、取链表中间结点始第 l.Len()/2向上取整个结点
func (l *List) GetMiddleNode() *ListNode {
	if l.head.Next == nil {
		return nil
	}
	first := l.head  // 每次走一步
	second := l.head // 每次走2步
	/*
		从上表可以看出
		second走到奇数位时，first就要走一步。
	*/
	for second != nil {
		second = second.Next
		if second != nil { // 只要second能走出一步（奇数位），first就往后走
			first = first.Next
			second = second.Next
		}
		// 如果是向下取整则是second走2步后firt走1步
		// if second!=NULL {first<-first.Next}
	}

	return first
}

// 2、取倒数第K个结点
func (l *List) GetTailKthNode(k int) *ListNode {
	last := l.Get(k)
	if last == nil { // length<k
		return nil
	}
	tailKth := l.head
	for ; last != nil; last, tailKth = last.Next, tailKth.Next {
	}
	return tailKth
}

func main() {
	arr := []int{2, 3, 4, 5, 6, 7, 7, 8, 8}
	// InitArr 测试
	fmt.Println("InitArr 测试")
	l := &List{}
	l.InitArr(arr...)
	l.Visit(func(node *ListNode) {
		fmt.Printf(", %d", node.Val)
	})
	fmt.Println()

	fmt.Println("InitArr 测试 end")
	fmt.Println("--------------------------")
	fmt.Println("Init 测试")
	l.Init(l.head.Next)
	l.Visit(func(node *ListNode) {
		fmt.Printf(", %d", node.Val)
	})
	fmt.Println()

	fmt.Println("Init 测试 end")
	fmt.Println("--------------------------")

	fmt.Println("IsEmpty 测试")
	fmt.Println(l.IsEmpty())
	l.Init(nil)
	fmt.Println(l.IsEmpty())
	fmt.Println()

	fmt.Println("IsEmpty 测试 end")
	fmt.Println("--------------------------")

	fmt.Println("Length 测试")
	fmt.Println(l.Length())
	for i := 1; i < 10; i++ {
		l.InsertHead(&Node{Val: i})
		fmt.Println(l.Length())
	}
	l.Visit(func(node *ListNode) {
		fmt.Printf(", %d", node.Val)
	})

	fmt.Println("\nLength 测试 end")
	fmt.Println("--------------------------")

	fmt.Println("Get 测试")
	fmt.Println(l.Length())
	for i := 1; i < 11; i++ {
		node := l.Get(i)
		fmt.Printf("%+v, %p\n", node, node)
	}

	fmt.Println("\nGet 测试 end")
	fmt.Println("--------------------------")

	fmt.Println("Delete 测试")
	fmt.Println(l.Length())
	node := l.Delete(11)
	fmt.Printf("%+v, %p\n", node, node)
	node = l.Delete(9)
	fmt.Printf("%+v, %p\n", node, node)
	node = l.Delete(8)
	fmt.Printf("%+v, %p\n", node, node)
	node = l.Delete(2)
	fmt.Printf("%+v, %p\n", node, node)
	node = l.Delete(1)
	fmt.Printf("%+v, %p\n", node, node)
	node = l.Delete(5)
	fmt.Printf("%+v, %p\n", node, node)

	fmt.Println("\nDelete 测试 end")
	fmt.Println("--------------------------")

	fmt.Println("InsertHead 测试")

	l.Visit(func(node *ListNode) {
		fmt.Printf(", %d", node.Val)
	})
	fmt.Println()
	l.InsertHead(&Node{Val: 100})
	l.InsertHead(&Node{Val: 101})
	l.InsertHead(&Node{Val: 99})

	l.Visit(func(node *ListNode) {
		fmt.Printf(", %d", node.Val)
	})
	fmt.Println("\nInsertHead 测试 end")
	fmt.Println("--------------------------")

	fmt.Println("InsertHead 测试")

	l.Visit(func(node *ListNode) {
		fmt.Printf(", %d", node.Val)
	})
	fmt.Println()
	l.Insert(1, &Node{Val: 1100})
	l.Insert(3, &Node{Val: 1101})
	l.Insert(2, &Node{Val: 199})

	l.Visit(func(node *ListNode) {
		fmt.Printf(", %d", node.Val)
	})
	fmt.Println("\nInsertHead 测试 end")
	fmt.Println("--------------------------")

	fmt.Println("Reverse 测试")

	l.Reverse()
	l.Visit(func(node *ListNode) {
		fmt.Printf(", %d", node.Val)
	})
	fmt.Println()
	l.Reverse()
	l.Visit(func(node *ListNode) {
		fmt.Printf(", %d", node.Val)
	})

	fmt.Println("\nReverse 测试 end")
	fmt.Println("--------------------------")

	fmt.Println("GetMiddleNode 测试")

	l.Init(nil)
	fmt.Printf("%+v, %p \n", l.GetMiddleNode(), l.GetMiddleNode())
	for i := 1100; i < 1111; i++ {
		l.InsertHead(&Node{Val: i})
		fmt.Printf("%+v, %p \n", l.GetMiddleNode(), l.GetMiddleNode())
		fmt.Printf("%+v\n", l.Get((l.Length()+1)/2))
		l.Visit(func(node *ListNode) {
			fmt.Printf(", %d", node.Val)
		})
		fmt.Println("len:", l.Length())
	}

	fmt.Println("\nGetMiddleNode 测试 end")
	fmt.Println("--------------------------")

	fmt.Println("GetTailKthNode 测试")

	l.Visit(func(node *ListNode) {
		fmt.Printf(", %d", node.Val)
	})
	fmt.Println("len:", l.Length())

	node = l.GetTailKthNode(15)
	fmt.Printf("%+v, %p\n", node, node)
	for i := 1; i <= l.Length(); i++ {
		node := l.GetTailKthNode(i)
		fmt.Printf("%+v, %p\n", node, node)
	}

	fmt.Println("\nGetTailKthNode 测试 end")
	fmt.Println("--------------------------")
}