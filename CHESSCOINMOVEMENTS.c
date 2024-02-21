#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include<ctype.h>

char board[8][8] = {
    {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
    {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'-', '-', '-', '-', '-', '-', '-', '-'},
    {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
    {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
};

typedef enum {
    WHITE,
    BLACK
} PlayerColor;

PlayerColor current_player = WHITE;

typedef enum { PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, EMPTY } PieceCode;

bool is_valid_move(int src_row, int src_col, int dest_row, int dest_col) {
	int direction;
    // Check if the source and destination squares are within the boundaries of the board
    if (src_row < 0 || src_row >= 8 || src_col < 0 || src_col >= 8 || dest_row < 0 || dest_row >= 8 || dest_col < 0 || dest_col >= 8) {
        return false;
    }

    // Check if the path is unobstructed
    if (src_row == dest_row && src_col == dest_col) {
        return false;
    }

    // Check if the piece at the source square is the same color as the current player
    char piece = board[src_row][src_col];
    bool is_player_1 = isupper(piece);
    bool is_player_2 = islower(piece);

    if (is_player_1 && current_player == BLACK) {
        return false;
    }

    if (is_player_2 && current_player == WHITE) {
        return false;
    }
    char dest_piece = board[dest_row][dest_col];
    bool is_dest_piece_player_1 = (dest_piece >= 'A' && dest_piece <= 'Z');
    bool is_dest_piece_player_2 = (dest_piece >= 'a' && dest_piece <= 'z');

    if (is_player_1 && is_dest_piece_player_1) {
        return false;
    }

    if (is_player_2 && is_dest_piece_player_2) {
        return false;
    }

    // Check piece-specific movements
    switch (piece) {
       case 'P':
    // White Pawn movement rules
    if (src_col == dest_col && dest_row == src_row - 1 && board[dest_row][dest_col] == '-') {
        return true; // Regular one-square forward move
    }

    if (src_row == 6 && src_col == dest_col && dest_row == src_row - 2 && board[src_row - 1][dest_col] == '-' && board[dest_row][dest_col] == '-') {
        return true; // Initial double-square forward move
    }

    if (dest_row == src_row - 1 && (dest_col == src_col - 1 || dest_col == src_col + 1) && islower(board[dest_row][dest_col])) {
        return true; // Capture diagonally
    }

    break;

case 'p':
    // Black Pawn movement rules
    if (src_col == dest_col && dest_row == src_row + 1 && board[dest_row][dest_col] == '-') {
        return true; // Regular one-square forward move
    }

    if (src_row == 1 && src_col == dest_col && dest_row == src_row + 2 && board[src_row + 1][dest_col] == '-' && board[dest_row][dest_col] == '-') {
        return true; // Initial double-square forward move
    }

    if (dest_row == src_row + 1 && (dest_col == src_col - 1 || dest_col == src_col + 1) && isupper(board[dest_row][dest_col])) {
        return true; // Capture diagonally
    }

            // Add rules for en passant and pawn promotion if needed

            break;

       case 'R':
       case 'r':
    // Rook movement rules
    if (src_row == dest_row || src_col == dest_col) {
        // Check if the path is unobstructed
        int step_row = (dest_row > src_row) ? 1 : (dest_row < src_row) ? -1 : 0;
        int step_col = (dest_col > src_col) ? 1 : (dest_col < src_col) ? -1 : 0;

        int i = src_row + step_row;
        int j = src_col + step_col;

        while (i != dest_row || j != dest_col) {
            if (board[i][j] != '-') {
                return false; // Path is obstructed
            }
            i += step_row;
            j += step_col;
        }

        return true;
    }
    break;


        case 'N':
        case 'n':
            // Knight movement rules
            if ((abs(dest_row - src_row) == 2 && abs(dest_col - src_col) == 1) ||
                (abs(dest_row - src_row) == 1 && abs(dest_col - src_col) == 2)) {
                return true;
            }
            break;

         case 'B':
        case 'b':
    // Bishop movement rules
    if (abs(dest_row - src_row) == abs(dest_col - src_col)) {
        // Check if the diagonal path is unobstructed
        int row_step = (dest_row > src_row) ? 1 : -1;
        int col_step = (dest_col > src_col) ? 1 : -1;

        int i = src_row + row_step;
        int j = src_col + col_step;

        while (i != dest_row && j != dest_col) {
            if (board[i][j] != '-') {
                return false; // Path is obstructed
            }

            i += row_step;
            j += col_step;
        }

        return true;
    }
    break;
        case 'Q':
        case 'q':
            // Queen movement rules (combination of rook and bishop)
            if ((src_row == dest_row || src_col == dest_col) || (abs(dest_row - src_row) == abs(dest_col - src_col))) {
                // Check if the path is unobstructed
                if (src_row == dest_row || src_col == dest_col) {
                    int step;
                    if (src_row == dest_row) {
                        step = (dest_col > src_col) ? 1 : -1;
                    } else {
                        step = (dest_row > src_row) ? 1 : -1;
                    }

                    int i = src_row + step;
                    while (i != dest_row) {
                        if (board[i][src_col] != '-') {
                            return false; // Path is obstructed
                        }
                        i += step;
                    }
                } else { // Diagonal movement
                    int row_step = (dest_row > src_row) ? 1 : -1;
                    int col_step = (dest_col > src_col) ? 1 : -1;

                    int i = src_row + row_step;
                    int j = src_col + col_step;

                    while (i != dest_row) {
                        if (board[i][j] != '-') {
                            return false; // Path is obstructed
                        }

                        i += row_step;
                        j += col_step;
                    }
                }

                return true;
            }
            break;

        case 'K':
        case 'k':
            // King movement rules
            if (abs(dest_row - src_row) <= 1 && abs(dest_col - src_col) <= 1) {
                return true;
            }
            break;
    }

    return false;
}

PlayerColor get_piece_color(PieceCode piece_code) {
    return piece_code % 2 == 0 ? WHITE : BLACK;
}

void eliminate_piece(int row, int col) {
    board[row][col] = EMPTY;
}

void make_move(int src_row, int src_col, int dest_row, int dest_col) {
    // Check if the source square contains a piece
    if (board[src_row][src_col] == EMPTY) {
        printf("No piece in the source square.\n");
        return;
    }

    // Check if the destination square contains a piece of the opposite color
    if (board[dest_row][dest_col] != EMPTY && get_piece_color(board[dest_row][dest_col]) != get_piece_color(board[src_row][src_col])) {
        eliminate_piece(dest_row, dest_col);
    }

    // Move the piece from the source square to the destination square
    board[dest_row][dest_col] = board[src_row][src_col];
    board[src_row][src_col] = '-';
}

void print_board() {
    int i, j;
    printf("\n  0 1 2 3 4 5 6 7\n");
    for (i = 0; i < 8; i++) {
        printf("%d ", i);
        for (j = 0; j < 8; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int src_row, src_col, dest_row, dest_col;

    print_board();

    while (true) {
        printf("\nPlayer %d's turn: \n", current_player + 1);
        printf("\nEnter source row, source column, destination row, and destination column: ");
        scanf("%d %d %d %d", &src_row, &src_col, &dest_row, &dest_col);

        if (is_valid_move(src_row, src_col, dest_row, dest_col)) {
            make_move(src_row, src_col, dest_row, dest_col);

            // Switch the current player
            current_player = current_player == WHITE ? BLACK : WHITE;
        } else {
            printf("\nInvalid move");
        }

        print_board();
    }
    return 0;
}

