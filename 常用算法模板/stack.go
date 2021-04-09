
var Stack[55][]int
var top [55]int

func initStack() {
	for i:=0;i<55;i++{
		top[i]=0
		Stack[i]=[]int{}
	}
}

func push(x, i int) {
	 Stack[x] = append(Stack[x], i)
	 top[x]++
}

func topVal(x int) int {
	if top[x]==0 {
		return -1
	}

	return Stack[x][top[x]-1]
}

func pop(x int) {
	if top[x]==0 {
		return
	}

	Stack[x] = Stack[x][:top[x]-1]
	top[x]--
}