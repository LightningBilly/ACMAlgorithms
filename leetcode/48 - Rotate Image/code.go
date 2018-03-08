func rotateCycle(A [][]int, r1, r2, c1, c2 int) {
    len := r2-r1
    for i:=0;i<len;i++ {
        A[r1+i][c1], A[r2][c1+i], 
        A[r2-i][c2], A[r1][c2-i]=
        A[r2][c1+i], A[r2-i][c2], 
        A[r1][c2-i], A[r1+i][c1]
    }
}


func rotate(matrix [][]int)  {
    for i, j:=0, len(matrix)-1; i<j; i, j=i+1, j-1 {
        rotateCycle(matrix, i, j, i, j)
    }
}

/*
[[1,2,3],[4,5,6],[7,8,9]]
[[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,16]]
[]
[[1,2,3,4,17],[5,6,7,8,18],[9,10,11,12,19],[13,14,15,16,20],[21,22,23,24,25]]
[[1]]
[[1,2],[3,4]]

*/