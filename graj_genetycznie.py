import statistics
from genetic import genetic_algorithm, save_weights

if __name__ == "__main__":
    print("=== AI TETRIS – ALGORYTM GENETYCZNY ===\n")

    # Uruchamiamy pełny trening
    best_weights, final_population, final_fitnesses = genetic_algorithm()

    # Zapisujemy najlepsze wagi
    save_weights(best_weights)

    # Statystyki końcowej populacji
    print("\n" + "="*65)
    print("STATYSTYKI KOŃCOWEJ POPULACJI")
    print("="*65)
    print(f"Maksimum:     {max(final_fitnesses):.1f} linii")
    print(f"Średnia:      {statistics.mean(final_fitnesses):.1f} linii")
    print(f"Mediana:      {statistics.median(final_fitnesses):.1f} linii")
    print(f"Osobników:    {len(final_fitnesses)}")
    print("="*65)

    print("\nGotowe! Najlepsze wagi są w pliku best_weights.json")