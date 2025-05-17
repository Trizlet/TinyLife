# 🧬 TinyLife

A simple C++ implementation of Conway’s Game of Life that runs in your terminal.

## 🛠 Build & Run

```bash
g++ -std=c++11 -O2 -o tinylife tinylife.cpp
./tinylife
```

Customize the simulation by passing **rows**, **columns**, and **delay (ms)** as arguments:

```bash
./tinylife 50 50 250
```

_(Here: 50 rows, 50 columns, 250 ms between generations.)_

## 📝 Notes

- Defaults to a 20 × 20 grid with a 250 ms delay if no arguments are given
- Currently, only random grid generation is supported
- Just a fun little project. not meant to be fancy!
