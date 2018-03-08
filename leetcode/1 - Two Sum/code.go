func twoSum(nums []int, target int) []int {
    m:=make(map[int]int)
    var ans []int
    ls:=len(nums)
    for i:=0; i<ls;i++ {
        if v,ok := m[target-nums[i]]; ok{
            ans = []int{v, i}            
            break
        }        
        m[nums[i]]=i
    }
    
    return ans
}