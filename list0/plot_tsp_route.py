import matplotlib.pyplot as plt

def plot_tsp_route(filename, country_name, output_image):
    x_coords = []
    y_coords = []

    try:
        with open(filename, 'r') as file:
            for line in file:
                parts = line.split()
                if len(parts) == 2:
                    x_coords.append(float(parts[0]))
                    y_coords.append(float(parts[1]))
    except FileNotFoundError:
        print(f"Zignorowano: Nie znaleziono pliku {filename}. Wygeneruj go najpierw w C++.")
        return

    plt.figure(figsize=(10, 6))
    
    plt.plot(x_coords, y_coords, marker='o', linestyle='none', color='blue', markersize=5)

    for i in range(len(x_coords) - 1):
        plt.plot([x_coords[i], x_coords[i+1]], 
                [y_coords[i], y_coords[i+1]], 
                color='blue', linestyle='-', alpha=0.4, linewidth=2)
    
    if x_coords and y_coords:
        plt.plot(x_coords[0], y_coords[0], marker='s', color='crimson', markersize=8, label='Start')

    plt.title(f'Najlepsza znaleziona trasa ({country_name})', fontsize=14)
    plt.xlabel('Współrzędna X')
    plt.ylabel('Współrzędna Y')
    plt.legend()
    plt.grid(True, linestyle='--', alpha=0.6)
    plt.tight_layout()

    plt.savefig(output_image, dpi=300)
    print(f"Sukces: Zapisano wykres dla {country_name} jako {output_image}")
    plt.close() 

if __name__ == "__main__":

    panstwa = [
        ("bestRoute_wi29.txt", "Western Sahara", "plot_wi29.png"),
        ("bestRoute_dj38.txt", "Djibouti", "plot_dj38.png"),
        ("bestRoute_qa194.txt", "Qatar", "plot_qa194.png"),
        ("bestRoute_uy734.txt", "Uruguay", "plot_uy734.png"),
        ("bestRoute_zi929.txt", "Zimbabwe", "plot_zi929.png")
    ]

    for plik_txt, nazwa, plik_png in panstwa:
        plot_tsp_route(plik_txt, nazwa, plik_png)