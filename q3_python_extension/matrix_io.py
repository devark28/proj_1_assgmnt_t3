import re

ENTRY_PATTERN = re.compile(r"\((\d+),\s*(\d+),\s*(-?\d+)\)")


def load_matrix(path):
    with open(path) as f:
        rows_line = f.readline().strip()
        cols_line = f.readline().strip()
        rows = int(rows_line.split("=")[1])
        cols = int(cols_line.split("=")[1])

        entries = []
        for line in f:
            match = ENTRY_PATTERN.match(line.strip())
            if not match:
                continue
            r, c, value = (int(x) for x in match.groups())
            entries.append((r, c, value))
            rows = max(rows, r + 1)
            cols = max(cols, c + 1)

        matrix = [[0] * cols for _ in range(rows)]
        for r, c, value in entries:
            matrix[r][c] = value

    return rows, cols, matrix
