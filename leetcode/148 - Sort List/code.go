type ListNode struct {
	Val  int
	Next *ListNode
}

func findHalf(head *ListNode) *ListNode {
	if head == nil || head.Next == nil {
		return head
	}
	step2 := head.Next.Next

	for ; step2 != nil; head = head.Next {
		step2 = step2.Next
		if step2 != nil {
			step2 = step2.Next
		}
	}

	return head
}

//递归形式merge
func mergeR(l1, l2 *ListNode) *ListNode {
	if l1 == nil {
		return l2
	}
	if l2 == nil {
		return l1
	}

	if l1.Val <= l2.Val {
		l1.Next = mergeR(l1.Next, l2)
		return l1
	}

	l2.Next = mergeR(l1, l2.Next)
	return l2
}

func merge(l1, l2 *ListNode) *ListNode {
	h := &ListNode{}
	var tail, tmp *ListNode
	tail = h
	for l1 != nil && l2 != nil {
		if l1.Val <= l2.Val {
			tmp, l1 = l1, l1.Next
		} else {
			tmp, l2 = l2, l2.Next
		}
		tail.Next = tmp
		tail = tmp
	}

	if l1 != nil {
		tail.Next = l1
	} else {
		tail.Next = l2
	}
	return h.Next
}

func mergeSort(head *ListNode) *ListNode {
	if head == nil || head.Next == nil {
		return head
	}

	mid := findHalf(head)
	second := mid.Next
	mid.Next = nil
	head = mergeSort(head)
	second = mergeSort(second)
	head = mergeR(head, second)
	return head
}

func sortList(head *ListNode) *ListNode {
	return mergeSort(head)
}

func show(head *ListNode) {
	for ; head != nil; head = head.Next {
		fmt.Printf("%d ", head.Val)
	}
	fmt.Println()
}
func getListByArray(arr []int) *ListNode {
	head := &ListNode{}
	t := head
	for _, n := range arr {
		t.Next = &ListNode{n, nil}
		t = t.Next
	}
	return head.Next
}
