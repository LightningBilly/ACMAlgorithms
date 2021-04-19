

### 数组

定义：存放相同元素的、连续的、固定长度的空间。 

声明： arr := [5]int{} 注意[] 里的数字必须是常量。 

![image-20201122155305640](/Users/chenbinbin/Desktop/slice 图示/数组.png)

传值：以值传递的形式，复制整个数组及其元素（传递时如果数组比较大那么会比较耗费资源） 

~~~go

func printArr(newArr [5]int) {
	newArr[4]=999
	fmt.Printf("new arr addr: %p, %p\n",&newArr, &newArr[0])
	fmt.Println("new arr:",newArr)
}

func main() {
	arr := [5]int{23,12,101}
	printArr(arr)
	fmt.Printf("new arr addr: %p, %p\n",&arr, &arr[0])
	fmt.Println("arr;",arr)
}
~~~

从结果可以看出 newArr 与 arr 的地址是不一样的。newArr[4]重新赋值，对原来的数组也没有影响。
说明在传值时是值传递，完全复制了一份新的数组。如果数组占用内存大这种方法效率是比较低的。这种情况下可以采用slice。



### slice 定义

源码结构

源码位于安装目录下的./go/src/runtime/slice.go

~~~go
type slice struct {
	array unsafe.Pointer // unsafe.Pointer 就是 *int 的别名占4个字节
	len   int
	cap   int
}
~~~

从定义可以看出 一个slice有3个字段组成，内存大小是12字节

array 表示第一个元素的地址

len表示slice有效的元素个数

cap表示slice最大容量

以下是 arr := make([]int, 5, 7) 的示意图

![image-20201122163849069](/Users/chenbinbin/Desktop/slice 图示/slice 底层实现.png)



### slice声明

~~~go
// slice 声明方法 
func main() {
	var s1 []int
	s2 := make([]int, 0)
	s3 := make([]int, 10, 20)
	s4 := []int{1, 2, 3}
	s5 := new([]int) // s5 本身是一个地址, slice的内容与s1一样。

	fmt.Printf("s1 addr:%p\n", &s1)
	println("s1:", s1) // [0/0]0x0 中的内容是[len/cap]array
	fmt.Printf("s2 addr:%p\n", &s2)
	println("s2:", s2) // [0/0]0x16e19c0
	fmt.Printf("s3 addr:%p\n", &s3)
	println("s3:", s3) // [10/20]0xc000214000
	fmt.Printf("s4 addr:%p\n", &s4)
	println("s4:", s4) // [3/3]0xc00020c060
	fmt.Printf("s5 addr:%p\n", s5)
	println("s5:", *s5) // [0/0]0x0
}
~~~

println方法可以打印出slice里面的len/cap/array

#### make与new的区别

从s5与s1到s4中的结果可以看出。

make会返回slice本身，会初始化array。

new会返回slice的指针，不会初始化array, 即array = nil。

![image-20201122170531640](/Users/chenbinbin/Desktop/slice 图示/new slice.png)



#### nil slice与空slice 

对比s1, s2 可以发现nil slice 是指array = nil, 而空slice 是array是有值 的，只是len, cap = 0。

![image-20201122170847994](/Users/chenbinbin/Desktop/slice 图示/nil slice.png)

![image-20201122170936009](/Users/chenbinbin/Desktop/slice 图示/empty slice.png)

### 切片功能

基本形式

slice[A:B] A:起始下标，B: 终止下标， A<=slice.len,B>=A>=0, B<=slice.cap。

slice[A:B:C] A:起始下标，B: 终止下标，C:容量终止下标，A<=slice.len, C>=B>=A>=0, C<=slice.cap。

新的slice len = B-A, cap = C-A

简化形式

slice[A:] , 此时 B = slice.len, C=slice.cap.

slice[A:B] 此时C = slice.cap.

slice[:B] 此时，A=0, C=slice.cap. 

~~~go
// 切片用法
func main() {
	s1 := make ([]int , 10,20)
	s2 := s1[1:3]
	println("s2:",s2) // [2/19]0xc0000e7ee0
	s3 := s1[1:12]
	println("s3:",s3) // [11/19]0xc0000e7ee0
	s4 := s1[1:12:18]
	println("s4:",s4) // [11/17]0xc0000e7ee0
	s5 := s1[:10]
	println("s5:",s5) // [10/20]0xc0000e7ed8
	s6 := s1[:10:18]
	println("s6:",s6) // [10/18]0xc0000e7ed8
	s7 := s3[1:19:20] // panic s3 = [11/19]0xc0000e7ee0 由于新的cap > s3.cap 错误。 
	println("s7:", s7)
}
~~~

### copy特性

~~~go
func copy(dst, src []Type) int

// 最终会调用以下方法
func slicecopy(toPtr unsafe.Pointer, toLen int, fmPtr unsafe.Pointer, fmLen int, width uintptr) int {
	if fmLen == 0 || toLen == 0 {
		return 0
	}

	n := fmLen
	if toLen < n { // 复制长度取新早slice最小的长度
		n = toLen
	}

	if width == 0 {
		return n
	}

	if raceenabled {
		...
	}
	if msanenabled {
		...
	}

	size := uintptr(n) * width
	if size == 1 { // common case worth about 2x to do here
		// TODO: is this still worth it with new memmove impl?
		*(*byte)(toPtr) = *(*byte)(fmPtr) // known to be a byte pointer
	} else {
		memmove(toPtr, fmPtr, size)
	}
	return n
}
~~~

该方法是将src数据复制到dst。

复制的特性是只会复制前 min(dst.len, src.len) 个元素。

~~~go
// 切片用法
func main() {
	s1 := []int{1, 2, 3, 4, 5}
	s2 := make([]int, 4, 6)
	fmt.Println(copy(s2, s1))
	fmt.Println(s2) // [1 2 3 4]
	s3 :=  make([]int, 6, 6)
	fmt.Println(copy(s3, s1))
	fmt.Println(s3) // [1 2 3 4 5 0]
}
~~~

### slice扩展特性

使用append时，如果新的len>cap则slice底层数据会重新生成。否则不变。

~~~go
// 切片用法
func main() {
	s1 := make([]int, 3, 6)
	for i, _:=range s1 {
		s1[i]=i+1
	}
	fmt.Println(s1) // [1 2 3]
	println("before append:",s1) // before append: [3/6]0xc0000a2000
	s1=append(s1, 4,5,6) // [1 2 3 4 5 6]
	println("after append:",s1 ) // after append: [6/6]0xc0000a2000
	fmt.Println(s1)
	s1=append(s1, 7)
	println("after append overflow:",s1 ) // after append overflow: [7/12]0xc0000a6000
	fmt.Println(s1) // [1 2 3 4 5 6 7]
}
~~~

可以看到当len<=6时底层数组是不变的，一旦超过就会重新生成数据。

cap增长规则：

~~~go
func growslice(et *_type, old slice, cap int) slice {
	if raceenabled {
		callerpc := getcallerpc()
		racereadrangepc(old.array, uintptr(old.len*int(et.size)), callerpc, funcPC(growslice))
	}
	if msanenabled {
		msanread(old.array, uintptr(old.len*int(et.size)))
	}

	if cap < old.cap {
		panic(errorString("growslice: cap out of range"))
	}

	if et.size == 0 {
		// append should not create a slice with nil pointer but non-zero len.
		// We assume that append doesn't need to preserve old.array in this case.
		return slice{unsafe.Pointer(&zerobase), old.len, cap}
	}

	newcap := old.cap
	doublecap := newcap + newcap // 原slice 2倍
	if cap > doublecap { // 新容量比原来的2倍还要大，直接使用新容量
		newcap = cap
	} else {
		if old.len < 1024 { // 原来长度<1024， 则以2倍增长
			newcap = doublecap
		} else {
			// Check 0 < newcap to detect overflow
			// and prevent an infinite loop.
			for 0 < newcap && newcap < cap { // 如果原来容量大于0则以1/4速度增长直到达到新容量需求
				newcap += newcap / 4
			}
		}
	}
	...

	return slice{p, old.len, newcap}
}
~~~

从源码中可以看出当cap>2*old.cap时，直接取cap作为新slice的cap.

否则当old.len< 1024, 则以2倍速增长。

否则就以1/4*old.len 增长，直到超过需求cap.



### 二维slice理解

```go
// 二维数组理解
func main() {
   s1 := [][]int{[]int{1}, []int{2,3}}
   s2 := make([][]int, 2,4)
   println(s1) // [2/2]0xc00012fee8
   println(s2[0]) // [0/0]0x0
}
```

可以理解为底层是一个 []int 为元素的数组。如图。

![image-20201124192733515](/Users/chenbinbin/Desktop/slice 图示/slice2array.png)

### 值传递 

```go
func printArr(newArr []int) {
   println("new arr:",newArr) // new arr: [2/2]0xc000098010
   fmt.Printf("new arr addr:%p\n", &newArr[0]) // new arr addr:0xc000098010
}

func main() {
   s1 := []int{2,3}
   printArr(s1)
   println("s1 arr:",s1) // s1 arr: [2/2]0xc000098010
   fmt.Printf("s1 addr:%p\n", &s1[0]) // s1 addr:0xc000098010
}
```

slice传递时，是以值传递的形式，这里说的值指的是slice本身。从上面可以看出newArr的地址和s1的地址并不一致，说明是复制了一个slice, 但其指向的底层数组是一样的。