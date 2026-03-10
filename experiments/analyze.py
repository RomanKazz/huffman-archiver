import os

import matplotlib.pyplot as plt
import pandas as pd

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

DATA_FILE = os.path.join(SCRIPT_DIR, "benchmark/result.csv")
PLOT_DIR = os.path.join(SCRIPT_DIR, "plots")

os.makedirs(PLOT_DIR, exist_ok=True)

df = pd.read_csv(DATA_FILE)

stats = (
    df.groupby(["type", "size"])
    .agg(
        compress_mean=("compress_time", "mean"),
        compress_std=("compress_time", "std"),
        decompress_mean=("decompress_time", "mean"),
        decompress_std=("decompress_time", "std"),
        ratio_mean=("ratio", "mean"),
    )
    .reset_index()
)

print(stats)

for t in stats["type"].unique():
    sub = stats[stats["type"] == t].sort_values("size")

    sizes = sub["size"]

    plt.figure()
    plt.plot(sizes, sub["ratio_mean"], marker="o")
    plt.title(f"Compression ratio ({t})")
    plt.xlabel("File size (bytes)")
    plt.ylabel("Compressed size / original size")
    plt.grid(True)
    plt.savefig(f"{PLOT_DIR}/ratio_{t}.png")
    plt.close()

    plt.figure()
    plt.plot(sizes, sub["compress_mean"], marker="o")
    plt.title(f"Compression time ({t})")
    plt.xlabel("File size (bytes)")
    plt.ylabel("Time (seconds)")
    plt.grid(True)
    plt.savefig(f"{PLOT_DIR}/compress_{t}.png")
    plt.close()

    plt.figure()
    plt.plot(sizes, sub["decompress_mean"], marker="o")
    plt.title(f"Decompression time ({t})")
    plt.xlabel("File size (bytes)")
    plt.ylabel("Time (seconds)")
    plt.grid(True)
    plt.savefig(f"{PLOT_DIR}/decompress_{t}.png")
    plt.close()

print("Plots saved to", PLOT_DIR)
