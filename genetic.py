import random
import json
import time
import statistics
from copy import deepcopy
from tetris import Tetris, W, H, PIECES

# ====================== PARAMETRY GLOBALNE ======================
GAMES_PER_INDIVIDUAL = 5          # ile pełnych gier gra każdy osobnik (zmniejszasz szum)
EARLY_STOPPING_PATIENCE = 15      # ile generacji bez poprawy → zatrzymujemy trening
POP_SIZE = 40
GENERATIONS = 100                 # maksymalna liczba generacji (early stopping i tak przerwie wcześniej)

# ====================== POMOCNICZE FUNKCJE ======================

def get_heights(board):
    heights = [0] * W
    for w in range(W):
        for r in range(H):
            if board[r][w]:
                heights[w] = H - r
                break
    return heights


def get_bumpiness(heights):
    return sum(abs(heights[i] - heights[i + 1]) for i in range(W - 1))


def get_max_height(heights):
    return max(heights) if heights else 0


def get_holes(board):
    wyn = 0
    for w in range(W):
        ile = 0
        for h in range(H - 1, -1, -1):
            wyn += ile * board[h][w]
            ile += (board[h][w] ^ 1)
    return wyn


def simulate_move(board, kind, rotation, col):
    shape = PIECES[kind][rotation % len(PIECES[kind])]
    row = 0
    if not all(0 <= row + dr < H and 0 <= col + dc < W and board[row + dr][col + dc] == 0
               for dr, dc in shape):
        return None, 0, False

    while True:
        next_row = row + 1
        if not all(0 <= next_row + dr < H and 0 <= col + dc < W and board[next_row + dr][col + dc] == 0
                   for dr, dc in shape):
            break
        row = next_row

    temp_board = [row[:] for row in board]
    for dr, dc in shape:
        temp_board[row + dr][col + dc] = 1

    new_board = [row[:] for row in temp_board if not all(row)]
    cleared = H - len(new_board)
    while len(new_board) < H:
        new_board.insert(0, [0] * W)

    return new_board, cleared, True


def evaluate_after_move(new_board, cleared, weights):
    heights = get_heights(new_board)
    bumpiness = get_bumpiness(heights)
    holes = get_holes(new_board)
    max_h = get_max_height(heights)

    return (
        weights[0] * bumpiness +
        weights[1] * holes +
        weights[2] * max_h +
        weights[3] * cleared
    )


def choose_best_action(env, weights):
    if env.game_over:
        return None

    kind = env.current_piece
    board = env.board
    best_score = float('-inf')
    best_action = None

    num_rots = len(PIECES[kind])
    for rot in range(num_rots):
        for col in range(-3, W + 3):
            result = simulate_move(board, kind, rot, col)
            if result[2]:
                new_board, cleared, _ = result
                score = evaluate_after_move(new_board, cleared, weights)
                if score > best_score:
                    best_score = score
                    best_action = (rot, col)
    return best_action


def play_one_game(weights, seed=None):
    """Jedna pełna gra."""
    if seed is None:
        seed = random.randint(0, 999999)
    env = Tetris(seed=seed)
    while not env.game_over:
        action = choose_best_action(env, weights)
        if action is None:
            break
        env.step(action)
    return env.lines


def play_game(weights):
    """Gra GAMES_PER_INDIVIDUAL razy i zwraca średnią liczbę linii."""
    results = [play_one_game(weights) for _ in range(GAMES_PER_INDIVIDUAL)]
    return statistics.mean(results)


# ====================== ALGORYTM GENETYCZNY ======================

def genetic_algorithm():
    """Pełny trening z early stopping."""
    rng = random.Random(42)
    population = [[rng.uniform(-2.0, 2.0) for _ in range(4)] for _ in range(POP_SIZE)]

    best_overall = None
    best_fitness = -1
    no_improvement = 0

    print(f"=== ROZPOCZYNAM TRENING (populacja={POP_SIZE}, gry na osobnika={GAMES_PER_INDIVIDUAL}) ===\n")

    for gen in range(GENERATIONS):
        fitnesses = []
        for i, weights in enumerate(population):
            avg_lines = play_game(weights)
            fitnesses.append(avg_lines)

            if avg_lines > best_fitness:
                best_fitness = avg_lines
                best_overall = weights[:]
                no_improvement = 0
            else:
                no_improvement += 1

            print(f"  Gen {gen+1:3d} | Osobnik {i+1:2d}/{POP_SIZE} → {avg_lines:.1f} linii (best: {best_fitness:.1f})")

        # statystyki generacji
        current_max = max(fitnesses)
        current_mean = statistics.mean(fitnesses)
        current_median = statistics.median(fitnesses)
        print(f"--- Generacja {gen+1} zakończona | max={current_max:.1f}  mean={current_mean:.1f}  median={current_median:.1f} ---\n")

        # early stopping
        if no_improvement >= EARLY_STOPPING_PATIENCE:
            print(f"EARLY STOPPING aktywowany po {gen+1} generacjach (brak poprawy przez {EARLY_STOPPING_PATIENCE} gen.)")
            break

        # tworzenie nowej populacji
        sorted_pop = sorted(zip(population, fitnesses), key=lambda x: x[1], reverse=True)
        new_pop = [ind[:] for ind, _ in sorted_pop[:max(3, POP_SIZE//5)]]  # elita

        while len(new_pop) < POP_SIZE:
            p1 = rng.choice(sorted_pop[:POP_SIZE//2])[0]
            p2 = rng.choice(sorted_pop[:POP_SIZE//2])[0]
            child = [(a + b) / 2 + rng.uniform(-0.3, 0.3) for a, b in zip(p1, p2)]
            if rng.random() < 0.35:
                idx = rng.randint(0, 3)
                child[idx] += rng.uniform(-0.8, 0.8)
            new_pop.append(child)

        population = new_pop

    print("\nTRENING ZAKOŃCZONY!")
    print(f"Najlepsze wagi: {best_overall}")
    print(f"Najlepszy wynik: {best_fitness:.1f} linii")

    # zwracamy najlepsze wagi + całą końcową populację (do statystyk)
    return best_overall, population, fitnesses


def save_weights(weights, filename="best_weights.json"):
    with open(filename, "w") as f:
        json.dump(weights, f)
    print(f"Wagi zapisane do {filename}")


def load_weights(filename="best_weights.json"):
    with open(filename) as f:
        return json.load(f)