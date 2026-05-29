import matplotlib.pyplot as plt
import os

cities = ["ca4663"] 
# Kiedy policzysz duże pliki, podmienisz to na: ["wi29" "mu1979", "ca4663", "tz6117", "eg7146", "zi929", "ei8246"]

def read_route(filename):
    x, y = [], []
    with open(filename, 'r') as f:
        for line in f:
            parts = line.strip().split()
            if len(parts) >= 2:
                x.append(float(parts[0]))
                y.append(float(parts[1]))
    if len(x) > 0:
        x.append(x[0])
        y.append(y[0])
    return x, y

for city in cities:
    fig, axs = plt.subplots(1, 3, figsize=(18, 6))
    fig.suptitle(f'Porównanie algorytmów dla pliku: {city}.tsp', fontsize=16, fontweight='bold')

    tasks = [
        (f'bestRoute_{city}_zad1.txt', 'Local Search', axs[0]),
        (f'bestRoute_{city}_tabuD.txt', 'Tabu Search', axs[1]),
        (f'bestRoute_{city}_sim3.txt', 'Symulowane Wyżarzanie', axs[2])
    ]

    for filepath, title, ax in tasks:
        if os.path.exists(filepath):
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
    output_filename = f'plot_porownanie_{city}.png'
    plt.savefig(output_filename, dpi=300)
    print(f'Wygenerowano: {output_filename}')
    plt.close()