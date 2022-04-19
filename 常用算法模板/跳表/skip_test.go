package main

import (
	"fmt"
	"testing"
)

type A struct {
	k, v int
}

func (a A) GetKey() interface{} {
	return a.k
}

func (a A) GetValue() interface{} {
	return a.v
}

func (a A) Less(b KV) bool {
	return a.k<b.GetKey().(int)
}


func TestSkipList(t *testing.T) {

	sl := NewSkipList()
	sl.Add(A{1,2})
	sl.Add(A{5,2})
	sl.Add(A{4,2})
	sl.Add(A{3,2})
	sl.Add(A{1,1})
	sl.Add(A{1,2})
	sl.Add(A{1,1})
	sl.Add(A{1,3})

	sum :=0
	sl.Range(func(e KV) {
		fmt.Printf("%+v ",e)
		sum += e.GetKey().(int)
	})
	fmt.Println("\n",sum)

	fmt.Println(sl.Deletee(A{1,2}))
	fmt.Println(sl.Deletee(A{1,2}))
	fmt.Println(sl.Deletee(A{1,2}))
	fmt.Println(sl.Deletee(A{5,2}))
	sl.Add(A{5,9})

	sl.Range(func(e KV) {
		fmt.Printf("%+v ",e)
	})
	fmt.Println()
}
