func moveZeroes(nums []int)  {
    i, j:=0,0
    len := len(nums)
    
    for ; j<len; j++ {
        if nums[j]!=0 {
            nums[i]=nums[j]
            i++
        }
    }
    
    for ;i<len; i++ {
        nums[i]=0
    }
}