# Project 1: Investigating a Suspicious Binary

Four small Linux systems programming exercises: ELF forensics, syscall
tracing, a CPython C extension, and Unix signal handling. Each question
lives in its own directory with source code, generated command output,
and a short report. All C code builds with a plain `gcc` command, no
build system is used.

## q1_suspicious_binary

`data_sync.c` simulates a local file-sync tool (walks a source
directory, copies files newer than their backup copy, logs every action
with a timestamp). It is inspected with `objdump` and `nm` only, treated
as an unknown binary that should not be executed as part of the
analysis.

```
gcc -Wall -o data_sync data_sync.c
objdump -f data_sync > outputs/objdump_file_header.txt
objdump -h data_sync > outputs/objdump_sections.txt
objdump -d data_sync > outputs/objdump_disassembly.txt
nm -D data_sync       > outputs/nm_dynamic_symbols.txt
nm data_sync          > outputs/nm_all_symbols.txt
```

## q2_syscall_monitoring

`backup_tool.c` creates a backup file, writes records to it, and reads
them back, logging each step. Traced with `strace` to map its C library
calls (`fopen`, `fprintf`, `fgets`, ...) to the underlying Linux system
calls.

```
gcc -Wall -o backup_tool backup_tool.c
strace -f -tt -o outputs/strace_output.txt ./backup_tool
```

## q3_python_extension

Matrix addition implemented two ways: `pure_matrix_add.py` (plain Python
nested loops) and `matrixadd.c` (a CPython C extension), benchmarked
against each other on a real 3850 x 3970 matrix pair (~15.3 million
cells) taken from prior sparse-matrix coursework.

```
python3 setup.py build_ext --inplace
python3 benchmark.py data/matrix_a.txt data/matrix_b.txt
```

Result: the C extension is about 2.9x faster than pure Python.

## q4_signal_service

`monitor_service.c` is a long-running service that prints a status line
every 5 seconds and reacts to `SIGINT`, `SIGUSR1`, and `SIGTERM` sent via
`kill`, using `sigaction()`.

```
gcc -Wall -o monitor_service monitor_service.c
./monitor_service &
kill -SIGUSR1 $!
kill -SIGTERM $!
```
