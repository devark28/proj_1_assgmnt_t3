import sys
import time

from matrix_io import load_matrix


def add_matrices(a, b, rows, cols):
    result = [[0] * cols for _ in range(rows)]
    for i in range(rows):
        row_a = a[i]
        row_b = b[i]
        row_r = result[i]
        for j in range(cols):
            row_r[j] = row_a[j] + row_b[j]
    return result


def main():
    path_a = sys.argv[1] if len(sys.argv) > 1 else "data/matrix_a.txt"
    path_b = sys.argv[2] if len(sys.argv) > 2 else "data/matrix_b.txt"

    rows_a, cols_a, a = load_matrix(path_a)
    rows_b, cols_b, b = load_matrix(path_b)
    if (rows_a, cols_a) != (rows_b, cols_b):
        raise ValueError("matrix dimensions must match for addition")

    start = time.perf_counter()
    result = add_matrices(a, b, rows_a, cols_a)
    elapsed = time.perf_counter() - start

    checksum = sum(sum(row) for row in result)
    print(f"Matrix size: {rows_a} x {cols_a}")
    print(f"Pure Python addition time: {elapsed:.4f} s")
    print(f"Result checksum: {checksum}")


if __name__ == "__main__":
    main()
