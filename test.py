import gymnasium as gym
import matplotlib.pyplot as plt
from IPython import display
import time

# 1. Zmieniamy render_mode na "rgb_array", aby pobierać klatki obrazu
env = gym.make(
    "ALE/Tetris-v5", 
    render_mode="rgb_array", 
    repeat_action_probability=0.0  # Wymuszamy 0% szans na zablokowanie klawisza
)
observation, info = env.reset()

# 2. Przygotowujemy wykres matplotlib do wyświetlania gry
plt.figure(figsize=(5, 6))
img = plt.imshow(env.render())
plt.axis('off') # Ukrywamy osie X i Y

print("Rozpoczynam podgląd gry...")

# 3. Pętla gry (ustawiona na max 20000 kroków)
for step in range(20000):
    action = env.action_space.sample() # Losowa akcja
    observation, reward, terminated, truncated, info = env.step(action)
    
    # 4. Aktualizujemy obraz na ekranie
    img.set_data(env.render())
    display.display(plt.gcf())
    display.clear_output(wait=True) # Czyścimy poprzednią klatkę
    
    # Małe opóźnienie, żeby gra nie leciała za szybko
    time.sleep(0.05)
    
    # 5. ZMIANA: Zamiast resetować grę, po prostu przerywamy pętlę
    if terminated or truncated:
        print(f"Gra zakończona! Bot przegrał po {step} krokach.")
        break

# Sprzątamy po sobie
env.close()
plt.close() # Usuwa "puste" okienko wykresu po zakończeniu
print("Koniec symulacji.")
