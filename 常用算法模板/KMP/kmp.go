
type KMP struct {
	next    []int
	pattern string
}

// a b a c a b a
// 0 0 1 0 1 2 3

func (k *KMP) makeNext(pattern string) []int {
	k.pattern = pattern
	k.next = make([]int, len(pattern))

	for i, j := 1, 0; i < len(pattern); i++ {
		for j > 0 && pattern[j] != pattern[i] {
			j = k.next[j-1]
		}

		if pattern[j] == pattern[i] {
			j++
		}
		k.next[i] = j
	}

	return k.next
}

func (k *KMP) find(s string) []int {
	j := 0
	res := []int{}
	for i, c := range s {
		for j > 0 && c != int32(k.pattern[j]) {
			j = k.next[j-1]
		}
		if c == int32(k.pattern[j]) {
			j++
		}
		if j == len(k.pattern) {
			res = append(res, i-len(k.pattern)+1)
			j = k.next[j-1]
		}
	}

	return res
}
