# file-diff-analyzer

# File Diff Analyzer

A command-line tool written in C that compares two files and identifies 
byte-level differences using two distinct methods — byte-by-byte reading 
and memory buffer loading — both benchmarked in milliseconds using 
gettimeofday().

## How it works

**Step 1 — Byte-by-byte comparison:**
Reads one byte at a time from each file using Unix read() syscalls and 
writes any differing bytes from file1 to differencesFoundInFile1.txt.

**Step 2 — Memory buffer comparison:**
Loads both files entirely into dynamically allocated arrays using malloc(), 
compares them index by index, and writes differing bytes from file2 to 
differencesFoundInFile2.txt.

## Performance Insight

Step 2 is significantly faster than Step 1 on large files. For example, 
on a large Alice in Wonderland-sized text file:

| Method | Time |
|--------|------|
| Step 1 (byte-by-byte) | ~113ms |
| Step 2 (memory buffer) | ~0.7ms |

Step 1 is slower because each byte requires a separate read() syscall, 
which involves a context switch between user space and kernel space. Step 2 
loads the entire file into memory in one syscall, making comparisons 
much faster since they happen entirely in RAM with no repeated syscall 
overhead.

## Tech Stack

- Language: C
- System calls: open, close, read, write, stat
- Memory: Dynamic allocation with malloc and free (no memory leaks)
- Timing: gettimeofday()
- Build tool: Make
- Debugging: GDB, Valgrind

## Getting Started

**Requirements:** GCC and Make on a Unix/Linux system.

**Build:**
```bash
make
```

**Run:**
```bash
./proj3.out <file1> <file2>
```

**Example:**
```bash
./proj3.out input1.txt input2.txt
```

**Sample output:**
