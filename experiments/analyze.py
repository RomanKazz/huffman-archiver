import csv
import os
import statistics
from collections import defaultdict

try:
    import matplotlib.pyplot as plt
except ModuleNotFoundError:
    plt = None

try:
    import pandas as pd
except ModuleNotFoundError:
    pd = None

SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))

DATA_FILE = os.path.join(SCRIPT_DIR, "benchmark/result.csv")
PLOT_DIR = os.path.join(SCRIPT_DIR, "plots")


def read_stats_without_pandas():
    grouped = defaultdict(list)

    with open(DATA_FILE, newline="") as file:
        for row in csv.DictReader(file):
            key = (row["type"], int(row["size"]))
            grouped[key].append(row)

    stats = []
    for (file_type, size), rows in sorted(grouped.items()):
        compress = [float(row["compress_time"]) for row in rows]
        decompress = [float(row["decompress_time"]) for row in rows]
        ratio = [float(row["ratio"]) for row in rows]

        stats.append(
            {
                "type": file_type,
                "size": size,
                "compress_mean": statistics.mean(compress),
                "compress_std": statistics.stdev(compress)
                if len(compress) > 1
                else 0.0,
                "decompress_mean": statistics.mean(decompress),
                "decompress_std": statistics.stdev(decompress)
                if len(decompress) > 1
                else 0.0,
                "ratio_mean": statistics.mean(ratio),
            }
        )

    return stats


def print_stats(stats):
    header = (
        "type,size,compress_mean,compress_std,"
        "decompress_mean,decompress_std,ratio_mean"
    )
    print(header)
    for row in stats:
        print(
            f"{row['type']},{row['size']},"
            f"{row['compress_mean']:.6f},{row['compress_std']:.6f},"
            f"{row['decompress_mean']:.6f},{row['decompress_std']:.6f},"
            f"{row['ratio_mean']:.6f}"
        )


def plot_with_pandas():
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

    if plt is None:
        print("matplotlib is not installed; skipping plots")
        return

    for file_type in stats["type"].unique():
        subset = stats[stats["type"] == file_type].sort_values("size")
        sizes = subset["size"]

        plt.figure()
        plt.plot(sizes, subset["ratio_mean"], marker="o")
        plt.title(f"Compression ratio ({file_type})")
        plt.xlabel("File size (bytes)")
        plt.ylabel("Compressed size / original size")
        plt.grid(True)
        plt.savefig(f"{PLOT_DIR}/ratio_{file_type}.png")
        plt.close()

        plt.figure()
        plt.plot(sizes, subset["compress_mean"], marker="o")
        plt.title(f"Compression time ({file_type})")
        plt.xlabel("File size (bytes)")
        plt.ylabel("Time (seconds)")
        plt.grid(True)
        plt.savefig(f"{PLOT_DIR}/compress_{file_type}.png")
        plt.close()

        plt.figure()
        plt.plot(sizes, subset["decompress_mean"], marker="o")
        plt.title(f"Decompression time ({file_type})")
        plt.xlabel("File size (bytes)")
        plt.ylabel("Time (seconds)")
        plt.grid(True)
        plt.savefig(f"{PLOT_DIR}/decompress_{file_type}.png")
        plt.close()

    print("Plots saved to", PLOT_DIR)


if pd is None:
    print_stats(read_stats_without_pandas())
    print("pandas is not installed; skipping plots")
else:
    plot_with_pandas()
