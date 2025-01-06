import sys
from collections import defaultdict
import matplotlib.pyplot as plt

def count_line_lengths(file_path):
    line_length_counts = defaultdict(int)

    with open(file_path, 'r') as file:
        for line in file:
            line = line.strip()
            base_count = sum(1 for char in line if char in 'ACTG')
            line_length_counts[base_count] += 1

    return line_length_counts

def plot_line_length_counts(line_length_counts, width=8, height=4):
    lengths = sorted(line_length_counts.keys())
    counts = [line_length_counts[length] for length in lengths]

    plt.figure(figsize=(width, height))  # Set the figure size here
    plt.bar(lengths, counts)
    plt.xlabel('Number of Bases')
    plt.ylabel('Number of Lines')
    plt.title('Distribution of Line Lengths by Number of Bases')
    plt.xlim(min(lengths), max(lengths))
    plt.show()

def main():
    if len(sys.argv) != 2:
        print("Usage: python analyze.py <input_file>")
        sys.exit(1)

    input_file = sys.argv[1]
    line_length_counts = count_line_lengths(input_file)

    print("Line length occurrences (number of bases):")
    for length, count in sorted(line_length_counts.items()):
        print(f"{length} bases: {count} lines")

    plot_line_length_counts(line_length_counts)

if __name__ == "__main__":
    main()