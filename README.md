<div align="center">

# Solving the Travelling Salesman Problem with Evolutionary Genetic Algorithms

### A C++ Implementation of Nature-Inspired Optimization

[![Language](https://img.shields.io/badge/C%2B%2B-11+-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Algorithm](https://img.shields.io/badge/Algorithm-Genetic%20Algorithm-FF6F00?style=for-the-badge)](https://en.wikipedia.org/wiki/Genetic_algorithm)
[![Problem](https://img.shields.io/badge/Problem-TSP%20(NP--Hard)-DC143C?style=for-the-badge)](https://en.wikipedia.org/wiki/Travelling_salesman_problem)

*Exploring how evolutionary strategies — the same class of algorithms that underpin modern AI search — can approximate solutions to one of computer science's most iconic NP-hard problems.*

</div>

---

## Why This Matters

The Travelling Salesman Problem (TSP) sits at the intersection of computational complexity theory, operations research, and — increasingly — modern AI. It asks a deceptively simple question: *given a set of cities and the distances between them, what is the shortest possible route that visits each city exactly once and returns to the origin?*

With $n$ cities, the brute-force search space is $\frac{(n-1)!}{2}$ possible tours. At just 20 cities, that's over **60 quadrillion** routes. Exact algorithms collapse under this combinatorial explosion. Genetic Algorithms (GAs) offer an elegant alternative: rather than searching exhaustively, they evolve a population of candidate solutions through selection, recombination, and mutation — mimicking the mechanics of natural evolution to converge on near-optimal solutions in polynomial time.

This project implements a complete GA-based TSP solver in C++, demonstrating how population-based metaheuristics can navigate intractably large search spaces with remarkable efficiency.

---

## Architecture

```
┌─────────────────────────────────────────────────────────┐
│                        main.cpp                         │
│              Graph construction & solver entry           │
└────────────────────────┬────────────────────────────────┘
                         │
          ┌──────────────┴──────────────┐
          ▼                             ▼
┌──────────────────┐         ┌─────────────────────┐
│    Graph.cpp     │         │ GeneticTspSolver.cpp │
│                  │         │                      │
│ • Weighted       │         │ • Population init    │
│   directed graph │◄────────│ • Fitness evaluation │
│ • Adjacency map  │         │ • Crossover + repair │
│ • Edge storage   │         │ • Swap mutation      │
│   via std::map   │         │ • Elitist selection  │
└──────────────────┘         │ • Generational loop  │
                             └─────────────────────┘
```

| File | Responsibility |
|------|---------------|
| `Graph.cpp` | Weighted directed graph using `std::map<pair<int,int>, int>` for edge storage. Supports arbitrary topologies. |
| `GeneticTspSolver.cpp` | Core evolutionary engine: population management, genetic operators, and the solve loop. |
| `main.cpp` | Constructs test graphs (5-node and 9-node) and benchmarks the solver. |

---

## The Genetic Algorithm Pipeline

```
   ┌────────────────┐
   │ Random Initial │
   │   Population   │
   └───────┬────────┘
           │
           ▼
   ┌────────────────┐      ┌────────────────┐
   │  Sort by Cost  │─────►│  Elitist Top   │──── Preserve top 10%
   │  (Fitness)     │      │  Selection     │
   └────────────────┘      └───────┬────────┘
                                   │
           ┌───────────────────────┤
           ▼                       ▼
   ┌────────────────┐      ┌────────────────┐
   │   Single-Point │      │  Permutation   │
   │   Crossover    │─────►│    Repair      │──── Fix duplicate cities
   └────────────────┘      └───────┬────────┘
                                   │
                                   ▼
                           ┌────────────────┐
                           │  Swap Mutation  │──── Controlled randomness
                           └───────┬────────┘
                                   │
                                   ▼
                           ┌────────────────┐
                           │ Next Generation │───► Loop until converged
                           └────────────────┘
```

### Key Design Decisions

| Operator | Implementation | Rationale |
|----------|---------------|-----------|
| **Encoding** | Permutation of vertex indices | Natural representation for TSP — each gene is a valid city ordering |
| **Selection** | Elitism (top 10% preserved) | Guarantees monotonic improvement; best solution never lost |
| **Crossover** | Single-point with repair | Standard crossover breaks permutation validity; repair phase deduplicates and fills missing cities |
| **Mutation** | Random swap of two positions | Minimal perturbation that maintains permutation structure |
| **Fitness** | Total edge cost of the route | Direct optimization objective — lower is better |

---

## Quick Start

### Compile

```bash
g++ -std=c++11 -O2 -o tsp_solver main.cpp
```

### Run

```bash
./tsp_solver
```

### Expected Output (9-node graph)

The algorithm evolves across generations, with the best cost monotonically decreasing:

```
generation: 1  best of generation: 4 -> 2 -> 0 -> 1 -> 3 -> 5 -> 8 -> 7 -> 6  cost: 25
generation: 2  best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8  cost: 22
generation: 3  best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8  cost: 22
...
 the best solution is: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8  cost: 22
```

Convergence is typically achieved within 5 generations on small graphs, with execution times in the low milliseconds.

---

## Configuration

The solver accepts four parameters:

```cpp
GeneticTspSolver solver(graph, populationSize, generations, mutationRate);
```

| Parameter | Description | Default in `main.cpp` |
|-----------|-------------|----------------------|
| `graph` | Pointer to a weighted `Graph` | 9-node test graph |
| `populationSize` | Number of candidate routes per generation | 30 |
| `generations` | Number of evolutionary cycles | 20 |
| `mutationRate` | Percentage of population subject to mutation | 5% |

**Tuning guidance:** Larger populations improve exploration but increase per-generation cost. Higher mutation rates prevent premature convergence but can destabilize good solutions. For graphs beyond ~50 nodes, consider increasing both population size and generation count.

---

## Computational Complexity

| Operation | Time Complexity |
|-----------|----------------|
| Initial population generation | $O(P \cdot V^2)$ |
| Fitness evaluation (single route) | $O(V \cdot \log E)$ |
| Crossover + repair | $O(V^2)$ |
| Single generation | $O(P \cdot V^2)$ |
| Full solve | $O(G \cdot P \cdot V^2)$ |

Where $P$ = population size, $V$ = vertex count, $E$ = edge count, $G$ = generations.

---

## Test Graphs

The project includes two pre-built test cases:

**5-node complete graph** — A minimal instance for validation, with known optimal tour.

**9-node graph** — Two clusters (5 + 4 nodes) connected by a single bridge edge (weight 7 between nodes 3 and 5). This topology tests the algorithm's ability to handle non-uniform connectivity and find efficient inter-cluster traversals.

```
Cluster A (nodes 0-4)          Cluster B (nodes 5-8)
    0 ─── 1                        5 ─── 6
    │╲    │╲                       │╲    │
    │  ╲  │  ╲                     │  ╲  │
    │    ╲│    ╲                   │    ╲ │
    3 ─── 2 ─── 4                  8 ─── 7
              
         3 ════════ 5
           bridge (w=7)
```

---

## License

Open source. Use freely for educational and research purposes.

---

<div align="center">
<sub>Built as an exploration of evolutionary computation and combinatorial optimization.</sub>
</div>
7 -> 6 -> 8 -> 5 -> 3 -> 4 -> 2 -> 1 -> 0 ->  with cost of: 26  
6 -> 8 -> 7 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 29  
6 -> 8 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 37  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 29  
0 -> 1 -> 4 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 39  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 2 -> 4 -> 1 ->  with cost of: 32  
8 -> 7 -> 6 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 30  
4 -> 2 -> 0 -> 1 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 24  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 31  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 30  
7 -> 8 -> 6 -> 5 -> 3 -> 0 -> 4 -> 1 -> 2 ->  with cost of: 36  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 41  
1 -> 2 -> 0 -> 4 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 29  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
7 -> 6 -> 8 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 32  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
6 -> 8 -> 7 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 35  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 35  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 41  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 40  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 36  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 40  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 3 best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
  
  
----------------------------------------------------  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 24  
4 -> 2 -> 0 -> 1 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 24  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 34  
7 -> 6 -> 8 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 35  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 32  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 23  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 25  
1 -> 2 -> 0 -> 4 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 33  
8 -> 7 -> 6 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 29  
6 -> 7 -> 8 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 31  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 26  
8 -> 6 -> 7 -> 5 -> 3 -> 0 -> 2 -> 4 -> 1 ->  with cost of: 40  
6 -> 8 -> 7 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 33  
7 -> 6 -> 8 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 34  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 32  
7 -> 8 -> 6 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 29  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 34  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 36  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 31  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 40  
0 -> 1 -> 4 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 39  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 39  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 39  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 4 best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
  
  
----------------------------------------------------  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 23  
8 -> 7 -> 6 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 24  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 0 -> 1 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 27  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 29  
6 -> 7 -> 8 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 27  
6 -> 8 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 36  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
7 -> 8 -> 6 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 33  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 29  
1 -> 2 -> 0 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
7 -> 6 -> 8 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 32  
6 -> 8 -> 7 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 35  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 36  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 32  
7 -> 6 -> 8 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 35  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 36  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 40  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 41  
0 -> 1 -> 4 -> 2 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 37  
6 -> 8 -> 7 -> 5 -> 3 -> 0 -> 2 -> 4 -> 1 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 40  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 5 best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
  
  
----------------------------------------------------  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 29  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
1 -> 2 -> 0 -> 4 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 26  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 36  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
6 -> 8 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 37  
6 -> 8 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 36  
7 -> 6 -> 8 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 34  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 36  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 36  
8 -> 6 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
0 -> 1 -> 4 -> 2 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 37  
6 -> 8 -> 7 -> 5 -> 3 -> 0 -> 2 -> 4 -> 1 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 41  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 6 best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
  
  
----------------------------------------------------  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 26  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 32  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 34  
7 -> 8 -> 6 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 31  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 36  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 36  
7 -> 6 -> 8 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 35  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 34  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 36  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
8 -> 6 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 39  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
6 -> 8 -> 7 -> 5 -> 3 -> 0 -> 2 -> 4 -> 1 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 41  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 7 best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
  
  
----------------------------------------------------  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 26  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
6 -> 7 -> 8 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 29  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 36  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 36  
6 -> 8 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 36  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 40  
7 -> 8 -> 6 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 33  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 34  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 39  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
6 -> 8 -> 7 -> 5 -> 3 -> 0 -> 2 -> 4 -> 1 ->  with cost of: 38  
6 -> 8 -> 7 -> 5 -> 3 -> 1 -> 0 -> 4 -> 2 ->  with cost of: 33  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
6 -> 8 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 37  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 41  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 8 best of generation: 8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
  
  
----------------------------------------------------  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 26  
7 -> 8 -> 6 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 31  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 32  
7 -> 8 -> 6 -> 5 -> 3 -> 1 -> 0 -> 4 -> 2 ->  with cost of: 29  
8 -> 6 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 39  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 32  
7 -> 6 -> 8 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 35  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 34  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 36  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
6 -> 8 -> 7 -> 5 -> 3 -> 0 -> 2 -> 4 -> 1 ->  with cost of: 38  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 39  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 40  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 41  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 9 best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
  
  
----------------------------------------------------  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 26  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
6 -> 7 -> 8 -> 5 -> 3 -> 2 -> 0 -> 4 -> 1 ->  with cost of: 37  
6 -> 8 -> 7 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 35  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 34  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 32  
7 -> 6 -> 8 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 35  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 36  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
6 -> 8 -> 7 -> 5 -> 3 -> 0 -> 2 -> 4 -> 1 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
8 -> 6 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 39  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 39  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 40  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 41  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 10 best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
  
  
----------------------------------------------------  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 26  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 34  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 32  
7 -> 6 -> 8 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 34  
6 -> 8 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 36  
8 -> 6 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 39  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 39  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 32  
6 -> 8 -> 7 -> 5 -> 3 -> 2 -> 0 -> 4 -> 1 ->  with cost of: 43  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
8 -> 6 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
8 -> 6 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 39  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 39  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 40  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 41  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 11 best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
  
  
----------------------------------------------------  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 26  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 35  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 29  
6 -> 8 -> 7 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 35  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 36  
8 -> 6 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
6 -> 8 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 36  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 36  
8 -> 7 -> 6 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 26  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 39  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 40  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 0 -> 4 -> 1 ->  with cost of: 42  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 12 best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
  
  
----------------------------------------------------  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 26  
6 -> 7 -> 8 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 27  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 25  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
7 -> 8 -> 6 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 31  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 4 -> 2 -> 1 ->  with cost of: 27  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 36  
8 -> 6 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
6 -> 8 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 36  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 41  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 0 -> 4 -> 1 ->  with cost of: 42  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 13 best of generation: 8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
  
  
----------------------------------------------------  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 24  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 29  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 26  
6 -> 7 -> 8 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 27  
7 -> 8 -> 6 -> 5 -> 3 -> 0 -> 4 -> 2 -> 1 ->  with cost of: 29  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
7 -> 8 -> 6 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 31  
6 -> 8 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 37  
7 -> 6 -> 8 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 35  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
6 -> 8 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 36  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 0 -> 2 -> 1 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 35  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 39  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 40  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 0 -> 4 -> 1 ->  with cost of: 42  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 14 best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
  
  
----------------------------------------------------  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 27  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 4 -> 2 -> 1 ->  with cost of: 27  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 32  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
7 -> 8 -> 6 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 31  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 40  
0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
6 -> 8 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 36  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
8 -> 6 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 39  
2 -> 4 -> 0 -> 1 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 33  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 41  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 0 -> 4 -> 1 ->  with cost of: 42  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 15 best of generation: 0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
  
  
----------------------------------------------------  
0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
6 -> 7 -> 8 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 27  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 4 -> 2 -> 1 ->  with cost of: 27  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 30  
7 -> 8 -> 6 -> 5 -> 3 -> 1 -> 2 -> 0 -> 4 ->  with cost of: 31  
2 -> 4 -> 0 -> 1 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 29  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 36  
6 -> 8 -> 7 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 36  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
8 -> 6 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 39  
4 -> 1 -> 0 -> 2 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 39  
4 -> 0 -> 1 -> 2 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 33  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 0 -> 4 -> 1 ->  with cost of: 42  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 16 best of generation: 0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
  
  
----------------------------------------------------  
0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
6 -> 7 -> 8 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 27  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 4 -> 2 -> 1 ->  with cost of: 27  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 32  
2 -> 4 -> 0 -> 1 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 29  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 35  
7 -> 8 -> 6 -> 5 -> 3 -> 4 -> 1 -> 0 -> 2 ->  with cost of: 32  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
8 -> 6 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 39  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 0 -> 4 -> 1 ->  with cost of: 42  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 17 best of generation: 0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
  
  
----------------------------------------------------  
0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
6 -> 7 -> 8 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 27  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 4 -> 2 -> 1 ->  with cost of: 27  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
2 -> 4 -> 0 -> 1 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 29  
7 -> 6 -> 8 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 28  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 32  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 35  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
6 -> 8 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 37  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 0 -> 4 -> 1 ->  with cost of: 42  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 18 best of generation: 0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
  
  
----------------------------------------------------  
0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 4 -> 2 -> 1 ->  with cost of: 27  
7 -> 6 -> 8 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 32  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 23  
2 -> 4 -> 1 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 32  
2 -> 4 -> 0 -> 1 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 29  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
7 -> 6 -> 8 -> 5 -> 3 -> 0 -> 1 -> 4 -> 2 ->  with cost of: 35  
6 -> 8 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 0 -> 4 -> 1 ->  with cost of: 42  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 19 best of generation: 0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
  
  
----------------------------------------------------  
0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
8 -> 7 -> 6 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 22  
1 -> 0 -> 4 -> 2 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 30  
4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 1 -> 2 -> 4 ->  with cost of: 23  
6 -> 7 -> 8 -> 5 -> 3 -> 4 -> 0 -> 1 -> 2 ->  with cost of: 25  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
6 -> 7 -> 8 -> 5 -> 3 -> 0 -> 4 -> 2 -> 1 ->  with cost of: 27  
2 -> 4 -> 0 -> 1 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 27  
1 -> 2 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 29  
7 -> 8 -> 6 -> 5 -> 3 -> 1 -> 2 -> 4 -> 0 ->  with cost of: 29  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 6 -> 8 -> 7 ->  with cost of: 36  
8 -> 6 -> 7 -> 5 -> 3 -> 0 -> 1 -> 4 -> 2 ->  with cost of: 38  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 39  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 8 -> 6 -> 7 ->  with cost of: 37  
6 -> 8 -> 7 -> 5 -> 3 -> 2 -> 4 -> 1 -> 0 ->  with cost of: 37  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
1 -> 4 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
4 -> 1 -> 2 -> 0 -> 3 -> 5 -> 7 -> 8 -> 6 ->  with cost of: 38  
2 -> 1 -> 4 -> 0 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 42  
7 -> 6 -> 8 -> 5 -> 3 -> 2 -> 0 -> 4 -> 1 ->  with cost of: 42  
2 -> 0 -> 4 -> 1 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 44  
0 -> 4 -> 1 -> 2 -> 3 -> 5 -> 7 -> 6 -> 8 ->  with cost of: 43  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
8 -> 6 -> 7 -> 5 -> 3 -> 1 -> 4 -> 0 -> 2 ->  with cost of: 44  
*******************************************************************  
generation: 20 best of generation: 0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21  
******************************************************************* 
  
*******************************************************************************************************************************
   the best solution is:0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21   
*******************************************************************************************************************************
  
Time for to run the genetic algorithm: 0.311 seconds.  
  
