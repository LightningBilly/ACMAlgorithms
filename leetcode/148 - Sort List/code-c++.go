type ListNode struct {
	Val  int
	Next *ListNode
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

func mergeKLists(lists []*ListNode) *ListNode {
	totlen :=len(lists)
	if totlen==0 {return nil}

	for ;totlen>1; {
		newLen :=0

		for i:=0;i<totlen;i, newLen=i+2, newLen+1 {
			if i+1<totlen {
				lists[newLen] = merge(lists[i], lists[i+1])
			} else {
				lists[newLen] = lists[i]
			}
		}

		totlen=newLen
	}

	return lists[0]
}

func sortList(head *ListNode) *ListNode {
	N:=128
	sortedLists :=make([]*ListNode, N)

	for tmp:=head;head!=nil; tmp = head{
		head=head.Next
		tmp.Next=nil

		//遍历sortedLists
		for i:=0;i<N;i++ {
			if sortedLists[i]==nil {
				sortedLists[i]=tmp
				break
			}

			tmp = merge(tmp, sortedLists[i])
			sortedLists[i]=nil
		}
	}
/*
	for i, link:=range sortedLists {
		fmt.Println("sub link", i,": ")
		show(link)
	}
*/
	return mergeKLists(sortedLists)
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

func main() {
	head := getListByArray([]int{3, -1, 22, 34, 54, 54})
	show(head)
	head = sortList(head)
	fmt.Println("sorted")
	show(head)

	head = sortList(nil)
	fmt.Println("sorted")
	show(head)

	head = getListByArray([]int{1})

	head = sortList(head)
	fmt.Println("sorted")
	show(head)
}
