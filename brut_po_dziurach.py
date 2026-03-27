import copy

from a import Tetris, W, PIECES

ile_gier = 1000
wyniki = [0] * ile_gier

for gra in range(ile_gier):
    env = Tetris()
    maks_ruchy = 10000

    for _ in range(maks_ruchy):  # maksymalnie 100000 ruchow
        ruchy = []

        for w in range(W):
            for rot in range(len(PIECES[env.current_piece])):
                action = (rot, w)
                test_env = copy.deepcopy(env)
                obs, reward, done = test_env.step(action)

                if done:
                    continue

                liczba_dziur = test_env.ile_dziur()
                ruchy.append((liczba_dziur, -obs["lines"], action))

        if not ruchy:
            wyniki[gra] = env.lines
            break

        naj = min(ruchy, key=lambda ruch: (ruch[0], ruch[1]))
        env.step(naj[2])

for ele in wyniki:
    print("skasowalo", ele, "lini")
print("maks to ", max(wyniki))
print("min to ", min(wyniki))
srednia = 0
for ele in wyniki:
    srednia += ele
srednia /= ile_gier
print("srednia to ", srednia)
wyniki.sort()
print("mediana to ", wyniki[ile_gier//2])

'''
maks to  32
min to  0
srednia to  2.902
mediana to  2
'''
