type myHeap [][2]int

func (h *myHeap) Less(i, j int) bool {
	if (*h)[i][0] != (*h)[j][0] {
		return (*h)[i][0] < (*h)[j][0]
	} // 先取结尾数小的
	return (*h)[i][1] < (*h)[j][1] // 再取连续个数最小的
}

func (h *myHeap) Swap(i, j int) {
	(*h)[i], (*h)[j] = (*h)[j], (*h)[i]
}

func (h *myHeap) Len() int {
	return len(*h)
}

func (h *myHeap) Pop() (v interface{}) {
	*h, v = (*h)[:h.Len()-1], (*h)[h.Len()-1]
	return
}

func (h *myHeap) Push(v interface{}) {
	*h = append(*h, v.([2]int))
}

func isPossible(nums []int) bool {
	h := &myHeap{} // 大顶堆

	for _, v := range nums {
				
    		// 根据条件，不一定每次都取到 d[0]=x-1, 所以要循环
        for h.Len()>0 {
            top := heap.Pop(h).([2]int)
            if top[0]==v { // 一样，重新起一组，说明d[0], 最小都 是=v了，想接在某个序列上是不可能了，只能是另起一段
                heap.Push(h, top)
                heap.Push(h, [2]int{v, 1})
                break
            } else if top[0]<v-1 { // 这一段已经无法接上了，要判断是不是超过3了
                if top[1]<3 {return false}
            } else { // top[0] == v-1 top[1]++, 以v结尾，并长度+1
                top[0]++
                top[1]++
                heap.Push(h, top)
                break
            }
        }
		
		if h.Len() == 0 {
			heap.Push(h, [2]int{v, 1})
			continue
		}
		
	}
	
	// 判断最终在堆里的序列长度是不是都超过
	for h.Len()>0 {
		top := heap.Pop(h).([2]int)
		if top[1]<3{return false}
	}
	
	return true
}