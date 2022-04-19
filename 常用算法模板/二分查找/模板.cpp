
typedef long long lld;

const lld M = 1e5+2;
class Solution {
public:

    bool checkFunc(vector<int>& time, int totalTrips, lld totalTime) {
        lld trips =0;
        for( auto t:time) {
            trips += totalTime/t;
        }

        return trips>=totalTrips;
    }

    long long minimumTime(vector<int>& time, int totalTrips) {
        lld best = 1e14+10;
        lld left=1, right= best;
        while( left<= right) {
                lld mid = (right+left)>>1;
                if ( checkFunc(time, totalTrips, mid)) { // 可行解，贪心查看有没有更好解, 区间往小移
                    best = mid;
                    right = mid-1;
                } else {
                    left = mid+1;
                }
        }

        return best;
    }
};