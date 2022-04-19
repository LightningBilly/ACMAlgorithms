
/*
前缀和
*/
class PreSum  {
     vector<int>preSum; // 数组 preSum[i]=arr[0]+arr[1]+...+arr[i]。
// 常用有3种：
// 初始化数据, 通过arr, 初始化preSum
    void InitPre(vector<int> arr) {
        preSum.assign(arr.size(), arr[0]);
        for (int i=1;i<arr.size();i++ ){
            preSum[i] = preSum[i-1] + arr[i];
        }
    }



// 查询区间和
    int Sum(int i, int j) {
        if (i <= 0) {
            return preSum[j];
        }
        return preSum[j] - preSum[i-1];
    }
};

/*
// 查询最大子段和
// 这里是普通情况，有时候有特殊要求，比如子段不能为空，子段有可能为负数==
func (ps *PreSum) MaxSubArray() int {
minPre := 0
best := 0

for _, v := range ps.preSum {
best = max(best, v-minPre)
minPre = min(minPre, v)
}

return best
}
*/
