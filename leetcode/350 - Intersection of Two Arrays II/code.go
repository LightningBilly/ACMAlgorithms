func intersect(nums1 []int, nums2 []int) []int {
    m1:=make(map[int]int)
    
    for _,v := range nums1 {
        if _, ok := m1[v]; ok{
            m1[v]++ 
        } else {
            m1[v]=1
        }
    }
    
    res := []int{}
    
    for _, v:=range nums2 {
        if _, ok :=m1[v]; ok{
            res = append(res, v);
            m1[v]--
            if m1[v]==0 {
                delete(m1, v);
            }
        }
    }
    
    return res;
}