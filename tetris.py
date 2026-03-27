"""Simple Tetris in pure Python.

This version is intentionally short and easy to read.
Instead of moving the piece left/right step by step, you choose:
- rotation
- target column

The piece is then dropped straight down into the board.

Public state:
- board          -> 20x10 matrix, 0 = empty, 1 = locked block
- current_piece  -> current tetromino kind, e.g. 'T'
- next_piece     -> next tetromino kind
- score          -> simple score
- lines          -> cleared line count
- game_over      -> True if no move is possible

Action format:
    (rotation, column)

Example:
    obs, reward, done = env.step((1, 4))
"""

import random

W, H = 10, 20

PIECES = {
    "I": [
        [(0, 0), (0, 1), (0, 2), (0, 3)],
        [(0, 1), (1, 1), (2, 1), (3, 1)],
    ],
    "O": [
        [(0, 0), (0, 1), (1, 0), (1, 1)],
    ],
    "T": [
        [(0, 0), (0, 1), (0, 2), (1, 1)],
        [(0, 1), (1, 0), (1, 1), (2, 1)],
        [(1, 0), (1, 1), (1, 2), (0, 1)],
        [(0, 0), (1, 0), (1, 1), (2, 0)],
    ],
    "L": [
        [(0, 0), (1, 0), (2, 0), (2, 1)],
        [(1, 0), (1, 1), (1, 2), (0, 2)],
        [(0, 0), (0, 1), (1, 1), (2, 1)],
        [(0, 0), (0, 1), (0, 2), (1, 0)],
    ],
    "J": [
        [(0, 1), (1, 1), (2, 1), (2, 0)],
        [(0, 0), (0, 1), (0, 2), (1, 2)],
        [(0, 0), (0, 1), (1, 0), (2, 0)],
        [(0, 0), (1, 0), (1, 1), (1, 2)],
    ],
    "S": [
        [(0, 1), (0, 2), (1, 0), (1, 1)],
        [(0, 0), (1, 0), (1, 1), (2, 1)],
    ],
    "Z": [
        [(0, 0), (0, 1), (1, 1), (1, 2)],
        [(0, 1), (1, 0), (1, 1), (2, 0)],
    ],
}

PIECE_NAMES = list(PIECES)

class Tetris:
    def __init__(self, seed=None):
        self.rng = random.Random(seed)
        self.reset()

    def reset(self):
        self.board = [[0] * W for _ in range(H)]
        self.score = 0
        self.lines = 0
        self.game_over = False
        self.current_piece = self._rand_piece()
        self.next_piece = self._rand_piece()
        return self.observation()

    def _rand_piece(self):
        return self.rng.choice(PIECE_NAMES)

    def observation(self):
        return {
            "board": [row[:] for row in self.board],
            "current_piece": self.current_piece,
            "next_piece": self.next_piece,
            "score": self.score,
            "lines": self.lines,
            "game_over": self.game_over,
        }

    def _shape(self, kind, rotation):
        shape_list = PIECES[kind]
        return shape_list[rotation % len(shape_list)]

    def _fits(self, kind, rotation, row, col):
        for dr, dc in self._shape(kind, rotation):
            r, c = row + dr, col + dc
            if r < 0 or r >= H or c < 0 or c >= W:
                return False
            if self.board[r][c]:
                return False
        return True

    def _drop_row(self, kind, rotation, col):
        row = 0
        if not self._fits(kind, rotation, row, col):
            return None
        while self._fits(kind, rotation, row + 1, col):
            row += 1
        return row

    def _place(self, kind, rotation, col):
        row = self._drop_row(kind, rotation, col)
        if row is None:
            self.game_over = True
            return 0

        for dr, dc in self._shape(kind, rotation):
            self.board[row + dr][col + dc] = 1

        cleared = self._clear_lines()
        reward = 1 + cleared * 10
        self.score += reward
        self.lines += cleared
        return reward

    def _clear_lines(self):
        new_board = [row for row in self.board if not all(row)]
        cleared = H - len(new_board)
        while len(new_board) < H:
            new_board.insert(0, [0] * W)
        self.board = new_board
        return cleared

    def step(self, action):
        """Action = (rotation, column)."""
        if self.game_over:
            return self.observation(), 0, True

        rotation, column = action
        reward = self._place(self.current_piece, rotation, column)

        if not self.game_over:
            self.current_piece = self.next_piece
            self.next_piece = self._rand_piece()

            # If the next piece cannot spawn anywhere near the top, end the game.
            if self._drop_row(self.current_piece, 0, W // 2 - 2) is None:
                self.game_over = True

        return self.observation(), reward, self.game_over

    def render(self):
        view = [row[:] for row in self.board]
        print("+" + "-" * W + "+")
        for row in view:
            print("|" + "".join("#" if x else "." for x in row) + "|")
        print("+" + "-" * W + "+")
        print("piece:", self.current_piece, "next:", self.next_piece, "score:", self.score, "lines:", self.lines)
        if self.game_over:
            print("GAME OVER")

    def wzgledne_wysokosci(self):
        wysokosci = [0] * W #wysokosci jakie sa w naszej planszy
        for w in range(W): 
            for h in range(H):
                if self.board[w][h]:
                    wysokosci[w] = H - h
        minimalna_wysokosc_w_plansz = min(wysokosci)
        for w in range(W):
            wysokosci[w] -= minimalna_wysokosc_w_plansz
    
    def ile_dziur(self): #ess algorytmika sie przydala ess 
        wyn = 0
        for w in range(W):
            ile = 0
            for h in range(H - 1, -1, -1):
                wyn += ile * self.board[h][w]
                ile += (self.board[h][w] ^ 1)
        return wyn

    
env = Tetris()
print(env.ile_dziur())
for i in range (3):
    action = (0, 5)
    obs, reward, done = env.step(action)
env.render()
print(env.ile_dziur())
