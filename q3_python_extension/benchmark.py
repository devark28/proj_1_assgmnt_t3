import sys
import time

import matrixadd
from matrix_io import load_matrix
from pure_matrix_add import add_matrices


def main():
    path_a = sys.argv[1] if len(sys.argv) > 1 else "data/matrix_a.txt"
    path_b = sys.argv[2] if len(sys.argv) > 2 else "data/matrix_b.txt"

    rows_a, cols_a, a = load_matrix(path_a)
    rows_b, cols_b, b = load_matrix(path_b)
    if (rows_a, cols_a) != (rows_b, cols_b):
        raise ValueError("matrix dimensions must match for addition")

    print(f"Matrix size: {rows_a} x {cols_a} ({rows_a * cols_a:,} cells)")

    start = time.perf_counter()
    python_result = add_matrices(a, b, rows_a, cols_a)
    python_time = time.perf_counter() - start

    start = time.perf_counter()
    c_result = matrixadd.add(a, b)
    c_time = time.perf_counter() - start

    assert python_result == c_result, "pure Python and C extension results differ"

    print(f"Pure Python: {python_time:.4f} s")
    print(f"C extension: {c_time:.4f} s")
    print(f"Speedup: {python_time / c_time:.1f}x")


if __name__ == "__main__":
    main()
