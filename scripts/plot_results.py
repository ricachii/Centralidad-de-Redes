"""
Genera graficos de barras para cada dataset desde results/csv/.
Ejecutar desde cualquier directorio:
    python scripts/plot_results.py
"""
import pandas as pd
import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import glob
import os

# Paths siempre relativos a la raiz del proyecto (donde esta este script/../)
ROOT    = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
CSV_DIR = os.path.join(ROOT, "results", "csv")
OUT_DIR = os.path.join(ROOT, "results", "plots")
os.makedirs(OUT_DIR, exist_ok=True)


def load_csv(path):
    """Devuelve (info_dict, metrics_df) desde un CSV generado por ExperimentRunner."""
    lines = open(path, encoding="utf-8").readlines()

    # Primera fila: cabecera de dataset; segunda: valores
    header = lines[0].strip().split(",")
    values = lines[1].strip().split(",")
    info = dict(zip(header, values))

    # Metricas empiezan en la linea 3 (despues de la linea en blanco)
    from io import StringIO
    metrics_text = "".join(lines[3:])
    df = pd.read_csv(StringIO(metrics_text))
    return info, df


def plot_mean_time(info, df, out_path):
    fig, ax = plt.subplots(figsize=(11, 5))

    colors = plt.cm.tab10.colors
    bars = ax.bar(
        df["metric"], df["mean_ms"],
        yerr=df["variance_ms"] ** 0.5,
        capsize=5, color=colors[:len(df)],
        edgecolor="black", linewidth=0.6
    )

    # Etiqueta con el valor encima de cada barra
    for bar, val in zip(bars, df["mean_ms"]):
        label = f"{val:.1f}" if val < 1000 else f"{val/1000:.1f}s"
        ax.text(
            bar.get_x() + bar.get_width() / 2,
            bar.get_height() * 1.02,
            label, ha="center", va="bottom", fontsize=8
        )

    ax.set_xlabel("Metrica", fontsize=11)
    ax.set_ylabel("Tiempo medio (ms)", fontsize=11)
    ax.set_title(
        f"Tiempo de ejecucion - {info['dataset']}  "
        f"({int(float(info['vertices'])):,} vertices, "
        f"{int(float(info['edges'])):,} aristas)",
        fontsize=12
    )
    ax.set_yscale("log")
    ax.yaxis.set_major_formatter(ticker.FuncFormatter(
        lambda x, _: f"{x:.0f}ms" if x < 1000 else f"{x/1000:.0f}s"
    ))
    plt.xticks(rotation=20, ha="right", fontsize=9)
    plt.tight_layout()
    plt.savefig(out_path, dpi=150)
    plt.close()
    print(f"Guardado: {out_path}")


def plot_comparison(datasets, out_path):
    """Grafico comparativo de tiempos entre datasets (escala log)."""
    fig, ax = plt.subplots(figsize=(12, 5))

    metrics = datasets[0][1]["metric"].tolist()
    x = range(len(metrics))
    width = 0.35
    colors = ["steelblue", "darkorange"]

    for i, (info, df) in enumerate(datasets):
        offset = (i - len(datasets) / 2 + 0.5) * width
        ax.bar(
            [xi + offset for xi in x], df["mean_ms"],
            width=width, label=info["dataset"],
            color=colors[i], edgecolor="black", linewidth=0.5
        )

    ax.set_yscale("log")
    ax.set_xticks(list(x))
    ax.set_xticklabels(metrics, rotation=20, ha="right", fontsize=9)
    ax.set_ylabel("Tiempo medio (ms) - escala log", fontsize=11)
    ax.set_title("Comparacion de tiempos entre datasets", fontsize=12)
    ax.legend()
    ax.yaxis.set_major_formatter(ticker.FuncFormatter(
        lambda x, _: f"{x:.0f}ms" if x < 1000 else f"{x/1000:.0f}s"
    ))
    plt.tight_layout()
    plt.savefig(out_path, dpi=150)
    plt.close()
    print(f"Guardado: {out_path}")


# ── Main ──────────────────────────────────────────────
csv_files = glob.glob(os.path.join(CSV_DIR, "*.csv"))

if not csv_files:
    print(f"No se encontraron CSVs en {CSV_DIR}")
    print("Ejecuta primero: .\\build\\centralidad.exe")
    exit(1)

datasets = []
for path in sorted(csv_files):
    info, df = load_csv(path)
    datasets.append((info, df))
    name = info["dataset"]
    plot_mean_time(info, df, os.path.join(OUT_DIR, f"{name}_tiempos.png"))

if len(datasets) >= 2:
    plot_comparison(datasets, os.path.join(OUT_DIR, "comparacion.png"))

print(f"\nPlots en: {OUT_DIR}")
