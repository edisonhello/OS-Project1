# NTU CSIE OS Spring 2020 Project 1 - Process Scheduler

A simple process scheduler simulator. Written in C.

## Compilation

```
$ make
```

## Execution

```
$ sudo ./scheduler
```

The scheduler reads process specs from stdin, in format:

```
S // the scheduling policy, one of the following strings: FIFO, RR, SJF, PSJF.
N // the number of processes
N1 R1 T1
N2 R2 T2
...
Nn Rn Tn
// Ni - a character string with a length less than 32 bytes, specifying the name of the i-th process.
// Ri - a non-negative integer, specifying the ready time of the i-th process.
// Ti - a non-negative integer, specifying the execution time of the i-th process.
```
