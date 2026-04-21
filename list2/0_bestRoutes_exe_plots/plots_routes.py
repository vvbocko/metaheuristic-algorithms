import matplotlib.pyplot as plt
import os

# "wi29", "dj38", "qa194", "uy734", "zi929"
cities = ["wi29", "dj38", "qa194", "uy734", "zi929"] #"mu1979", "ca4663", "tz6117", "eg7146", "ei8246"

# Funkcja pomocnicza do wczytywania współrzędnych z pliku
def read_route(filename):
    x = []
    y = []
    with open(filename, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) >= 2:
                x.append(float(parts[0]))
                y.append(float(parts[1]))
    
    # Dodajemy pierwszy punkt na sam koniec, aby "zamknąć" narysowaną trasę
    if len(x) > 0:
        x.append(x[0])
        y.append(y[0])
        
    return x, y

# Główna pętla generująca wykresy
for city in cities:
    # Tworzymy płótno z 3 podwykresami w jednym rzędzie (1x3)
    # Rozmiar 18x6 cali, żeby wykresy były szerokie i czytelne
    fig, axs = plt.subplots(1, 3, figsize=(18, 6))
    fig.suptitle(f'Porównanie tras dla pliku: {city}.tsp', fontsize=16, fontweight='bold')

    # Konfiguracja naszych trzech wariantów
    tasks = [
        (f'bestRoute_{city}_zad1.txt', 'Zad 1: Invert (2-opt)', axs[0]),
        (f'bestRoute_{city}_zad2.txt', 'Zad 2: Losowy Invert', axs[1]),
        (f'bestRoute_{city}_zad3.txt', 'Zad 3: Swap', axs[2])
    ]

    for filepath, title, ax in tasks:
        if os.path.exists(filepath):
            # Wczytujemy dane
            x, y = read_route(filepath)
            

            ax.plot(x, y, marker='o', markersize=2, linestyle='-', linewidth=0.5, color='blue')
            ax.set_title(title, fontsize=14)
            
            ax.axis('equal')
            

            ax.set_xticks([])
            ax.set_yticks([])
        else:

            ax.set_title(f'{title}\n[BRAK PLIKU]', color='red')
            ax.axis('off')

    plt.tight_layout()
    
    output_filename = f'plot_{city}.png'
    plt.savefig(output_filename, dpi=300)
    print(f'Wygenerowano: {output_filename}')
    
    plt.close()