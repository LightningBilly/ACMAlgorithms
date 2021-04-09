func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}

func max(a, b int) int {
	if a > b {
		return a
	}
	return b
}

type Palindrome struct {
	rad []int
	arr []byte
	s   string
}

func (p *Palindrome) Init(s string) {
	p.s = s
	p.makeRad(s)
}

func (p *Palindrome) makeRad(s string) {
	arr := make([]byte, len(s)*2+3)
	// 构造新数组
	arr[0], arr[1], arr[len(arr)-1] = '(', '#', ')'
	for i, v := range s {
		arr[i*2+2], arr[i*2+3] = byte(v), '#'
	}
	p.arr = arr
	//fmt.Println(string(arr))

	rad := make([]int, len(arr))
	// 计算半径
	for k, j, i := 0, 0, 1; i < len(arr)-1; i += k {
		for ; arr[i-1-j] == arr[i+1+j]; j++ {}
		rad[i] = j // 通过扩展得到当前字符回文半径
		//for k=1; k <= rad[i] && i-rad[i]!=i-k-rad[i-k]; k++ { // 通过移项可以得到下面简化条件
		for k = 1; k <= j && rad[i-k] != rad[i]-k; k++ {
			rad[i+k] = min(rad[i-k], rad[i]-k)
		}
		/*
			rad[i]=j
			上面退出循环2种情况
			1. k>rad[i], 说明k>j ==> j=0
			2. rad[i-k]==rad[i]-k ==> j=j-k
		*/
		j = max(0, j-k) // j可以继续前面已经扩展的结果。
	}
	p.rad = rad
	//fmt.Println(rad)
}

// 检查s[i:j+1]是不是回文
func (p *Palindrome) Check(i, j int) bool {
	// 原来的i对应到新数组的i*2+2, 新数组中心点是(i*2+2 + j*2+2)/2 = i+j+2
	// 要判断半径是不是>= j-i+1
	mid := i + j + 2
	return p.rad[mid] >= j-i+1
}







func makeDp(s string) [][]bool {

	// 预申请内存
	dp := make([][]bool, len(s))
	for i := 0; i < len(dp); i++ {
		dp[i] = make([]bool, len(s))
	}

	/*
		i,j 的循环值一定要注意。
		要保证在计算dp[i][j]时，dp[i+1][j-1]已经被计算过了。
		思考一下如果i从0开始会怎么样。？？？
	*/
	for i := len(s) - 1; i >= 0; i-- {
		for j := i; j < len(s); j++ {
			dp[i][j] = false //默认为false
			// 只有一个字符是回文
			if i == j {
				dp[i][j] = true
				continue
			}

			dp[i][j] = s[i] == s[j] && (i+1 >= j-1 || dp[i+1][j-1]) // i+1 >= j-1 时说明 s[i+1:j] 只有一个字符，或者空。
		}
	}
	return dp
}

