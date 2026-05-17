<div align="center">

# Solving the Travelling Salesman Problem with Evolutionary Genetic Algorithms

### A C++ Implementation of Nature-Inspired Optimization

[![Language](https://img.shields.io/badge/C%2B%2B-11+-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![Algorithm](https://img.shields.io/badge/Algorithm-Genetic%20Algorithm-FF6F00?style=for-the-badge)](https://en.wikipedia.org/wiki/Genetic_algorithm)
[![Problem](https://img.shields.io/badge/Problem-TSP%20(NP--Hard)-DC143C?style=for-the-badge)](https://en.wikipedia.org/wiki/Travelling_salesman_problem)

**Author: [Mohammad Asadolahi](https://github.com/MohammadAsadolahi)**
Senior Agentic AI Engineer | Focus: Agentic AI Architectures In The Wild

</div>

---

## Why This Matters

The Travelling Salesman Problem (TSP) sits at the intersection of computational complexity theory, operations research, and modern AI. It asks a deceptively simple question: *given a set of cities and the distances between them, what is the shortest possible route that visits each city exactly once and returns to the origin?*

With $n$ cities, the brute-force search space is $\frac{(n-1)!}{2}$ possible tours. At just 20 cities, that's over **60 quadrillion** routes. Exact algorithms collapse under this combinatorial explosion. Genetic Algorithms (GAs) offer an alternative: rather than searching exhaustively, they evolve a population of candidate solutions through selection, recombination, and mutation — mimicking the mechanics of natural evolution to converge on near-optimal solutions.

This project implements a GA-based TSP solver in C++, demonstrating how population-based metaheuristics can navigate intractably large search spaces.

---

## Architecture

```
┌─────────────────────────────────────────────────────┐
│                        main.cpp                     │
│              Graph construction & solver entry       │
└────────────────────────┬────────────────────────────┘
                         │
          ┌──────────────┴──────────────┐
          ▼                             ▼
┌──────────────────┐         ┌─────────────────────┐
│    Graph.cpp     │         │ GeneticTspSolver.cpp │
│                  │         │                      │
│ • Weighted       │         │ • Population init    │
│   directed graph │◄────────│ • Fitness evaluation │
│ • Edge storage   │         │ • Crossover + repair │
│   via std::map   │         │ • Swap mutation      │
│                  │         │ • Elitist selection  │
└──────────────────┘         │ • Generational loop  │
                             └─────────────────────┘
```

| File | Responsibility |
|------|---------------|
| `Graph.cpp` | Weighted directed graph using `std::map<pair<int,int>, int>` for edge storage. |
| `GeneticTspSolver.cpp` | Core evolutionary engine: population management, genetic operators, and the solve loop. |
| `main.cpp` | Constructs test graphs (5-node and 9-node) and runs the solver. |

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
| **Crossover** | Single-point at midpoint with repair | Standard crossover breaks permutation validity; repair phase deduplicates and fills missing cities |
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

### Sample Output (9-node graph)

The algorithm is stochastic, so output varies between runs. A typical run looks like:

```
generation: 1 best of generation: 4 -> 2 -> 0 -> 1 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 25
generation: 2 best of generation: 4 -> 2 -> 1 -> 0 -> 3 -> 5 -> 6 -> 7 -> 8 ->  with cost of: 22
...
generation: 15 best of generation: 0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21
...
 the best solution is:0 -> 1 -> 2 -> 4 -> 3 -> 5 -> 8 -> 7 -> 6 ->  with cost of: 21
```

The full population is printed each generation, followed by the generation's best route. The solver also prints the full population details before each generation summary line.

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

**5-node complete graph** — A fully connected graph on 5 nodes with varying edge weights, used for validation.

**9-node graph** — Two fully connected clusters (5 nodes + 4 nodes) connected by a single bridge edge (weight 7 between nodes 3 and 5). Both clusters are complete graphs internally (all pairs of nodes within each cluster are connected). This topology tests the algorithm's ability to handle non-uniform inter-cluster connectivity.

---

## License

Open source. Use freely for educational and research purposes.

---

<div align="center">
<sub>Built as an exploration of evolutionary computation and combinatorial optimization.</sub>
</div>

this readme is AI assisted generated, so check for mistakes
