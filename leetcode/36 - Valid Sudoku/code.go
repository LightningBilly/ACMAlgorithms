func isValid(board[][] byte, row int, col int, sr int, sc int) bool {
    m := make(map[byte]bool)
    
    for i:=0;i<row;i++ {
        for j:=0;j<col;j++ {
            c := board[sr+i][sc+j]
            if c=='.' {
                continue
            }
            
            if _, ok := m[c]; ok {
                return false;
            }
            
            m[c] = true
        }
    }
    
    return true;
}

func isValidSudoku(board [][]byte) bool {
    for i:=0;i<9;i++ {
        if !isValid(board, 1, 9, i, 0) {
            return false
        }
        if !isValid(board, 9, 1, 0, i) {
            return false
        }
    }
    
    for i:=0;i<9; i+=3 {
        for j:=0;j<9; j+=3 {
            if !isValid(board, 3, 3, i, j) {
                return false
            }
        }
    }
    
    return true;
}