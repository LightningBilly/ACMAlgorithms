package main
import "fmt"

/*
并查集，判连通用
*/
type UnionFindSet struct {
	father  []int // 存储结点的父亲
	height  []int // 存储结点的父亲
	nodeNum int   // 总结点个数
}

func (us *UnionFindSet) InitUnionSet(n int) {
	us.nodeNum = n+1 // 不加也可以，有人喜欢以0开头
	us.father = make([]int, us.nodeNum)
	us.height = make([]int, us.nodeNum)
	for i, _ := range us.father {
		us.father[i] = i
		us.height[i] = 1
	}
}

// 查询父结点
func (us *UnionFindSet) Find(x int) int {
	for us.father[x] != x {
		x = us.father[x]
	}
	return x
}

//合并结点
func (us *UnionFindSet) Union(x, y int) bool {
	x = us.Find(x)
	y = us.Find(y)
	if x == y {
		return false
	}
	us.father[x] = y
	return true
}

func (us *UnionFindSet) FindV2(x int) int {
	root := x // 保存好路径上的头结点
	for us.father[root] != root {
		root = us.father[root]
	}
	/*
	从头结点开始一直往根上遍历
	把所有结点的father直接指向root。
	*/
	for us.father[x] != x {
		// 一定要先保存好下一个结点，下一步是要对us.father[x]进行赋值
		temp := us.father[x]
		us.father[x] = root
		x = temp
	}

	return root
}

/*
需要加入height[]属性，初始化为1.
*/
//合并结点
func (us *UnionFindSet) UnionV2(x, y int) bool {
	x = us.Find(x)
	y = us.Find(y)
	if x == y {
		return false
	}
	if us.height[x]<us.height[y] {
		us.father[x]=y
	} else if us.height[x]>us.height[y] {
		us.father[y]=x
	} else {
		us.father[x] = y
		us.height[y]++
	}
	return true
}


func findCircleNum(M [][]int) int {
	us := &UnionFindSet{}
	us.InitUnionSet(len(M))
	for i,row:=range M{
		for j,isFriend :=range row {
			if isFriend==1 {us.UnionV2(i,j)} // 是朋友则合并
		}
	}

	friendCircle :=0
	for i:=0;i<len(M);i++ {
		if us.FindV2(i)==i { friendCircle++} // 查看集合代表个数
	}

	return friendCircle
}
/*
[[1,1,0],[1,1,0],[0,0,1]]
[[1]]
[[1,0,0],[0,1,0],[0,0,1]]
[[1,0,1],[0,1,0],[1,0,1]]
[[1,0,1,0],[0,1,0,0],[1,0,1,1],[0,0,1,1]]
*/

func main() {
	us:= &UnionFindSet{}
	us.InitUnionSet(11)
	us.UnionV2(1, 2)
	us.UnionV2(10, 9)
	us.UnionV2(7,8)
	us.UnionV2(8,3)
	us.UnionV2(9,8)

	for i:=0;i<=10; i++  {
		fmt.Println(i, us.FindV2(i))
	}
}