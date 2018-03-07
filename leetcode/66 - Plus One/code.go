//slice翻转
func reverse(src []int){
    if src == nil {
        return
    }
    count := len(src)
    
    for i,j:=0, count-1; i<j; i, j = i+1, j-1 {
        src[i], src[j]= src[j], src[i]
    }
}

func plusOne(digits []int) []int {
    var len int = len(digits)
    
    res := []int{}
    for k,i:=1, len-1; i>=0 || k!=0; i, k = i-1, k/10 {
        if i>=0 {
            k+=digits[i]
        }
        
        res = append(res, k%10)        
        
    }
    
    reverse(res)

    return res
}