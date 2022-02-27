typedef long long lld;

const lld M = 1e5+2;



int getLowBit(int x) {
return x & (-x);
}

class TreeArray {
public:
    vector<int> sum; // 数组 编号从1开始
    int n;  // 数组大小

    // 初始化，申请数组大空间。
    void Init(int s) {
        this->n = s;
        sum.assign(s+10, 0); // 0号元素不用，n号元素需要使用
    }

    //更新操作
    void Add(int x, int v) {
        for ( ; x <= n; x += getLowBit(x)) { // 沿着线往上找影响到的结点，并更新
            sum[x] += v;
        }
    }

    // 求前缀和
    int QueryPre(int i) {
        int res =0;
        for (; i > 0; i -= getLowBit(i)) {
            res += sum[i];
        }
        return res;
    }

    // 求区间和
    int QuerySum(int i, int j) {
        return QueryPre(j) - QueryPre(i-1);
    }

};


class Solution {
public:
    long long goodTriplets(vector<int>& nums1, vector<int>& nums2) {
        vector<int> ind(nums1.size());
        for(int i=0;i<nums2.size();i++) {
            ind[nums2[i]]=i;
        }

        for(int i=0;i<nums1.size();i++) {
            nums1[i]=ind[nums1[i]];
        }

        TreeArray t;
        t.Init(nums1.size());
        vector<lld>left(nums1.size());
        for(int i=0;i<nums1.size();i++) {
            left[i] = t.QuerySum(1, nums1[i]);
            t.Add(nums1[i]+1, 1);
        }
        
        t.Init(nums1.size());
        vector<lld>right(nums1.size());
        
        for(int i=nums1.size()-1;i>=0;i--) {
            right[i] = t.QuerySum(nums1[i]+2, nums1.size());
            t.Add(nums1[i]+1, 1);
        }
        
        lld ans=0;
        
        for(int i=0;i<left.size();i++){
            ans += left[i]*right[i];
        }
        
        return ans;
    }
};
/*
2 1 0 3
0 2 1 3


[4,0,1,3,2]
 0 2 1 4 3
 0 1 2 3 4
[4,1,0,2,3]
*/