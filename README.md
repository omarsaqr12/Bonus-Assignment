# Weighted graph algorithms (C++ course assignment)

A small **undirected, positively weighted graph** exercise using an adjacency matrix. It loads a city-labelled graph from CSV, lists edges, performs depth-first traversal across all components, and prints shortest distances **and paths** from vertex A using Dijkstra's algorithm. This is a course exercise, not a general-purpose routing service.

## Build and run

From the repository root, with a C++17 compiler:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -I. main.cpp Graphs.cpp -o graph_demo
./graph_demo TestG.csv
./graph_demo CitiesG.csv
```

On Windows, use `graph_demo.exe` instead of `./graph_demo`. You may give another graph CSV path as the first argument. A missing or invalid file prints a diagnostic and returns a non-zero status.

To run the regression checks:

```sh
g++ -std=c++17 -Wall -Wextra -Wpedantic -I. tests/graph_tests.cpp Graphs.cpp -o graph_tests
./graph_tests
```

The tests exercise the seven-vertex sample, the supplied fourteen-vertex data, an unreachable vertex, invalid source indices, invalid/missing input, and preservation of the previously loaded graph after a failed load. GitHub Actions builds and tests on pull requests and pushes to `main`, including a run under an undefined-behavior sanitizer. The tests are not a formal proof of algorithm correctness.

## Repository map

- [`main.cpp`](main.cpp): CLI example and error reporting.
- [`Graphs.h`](Graphs.h) and [`Graphs.cpp`](Graphs.cpp): adjacency matrix, CSV validation, DFS and Dijkstra.
- [`Edge.h`](Edge.h): the edge value type used by the graph.
- [`TestG.csv`](TestG.csv): seven-vertex reference input; [`Sample.txt`](Sample.txt) records the original sample's expected shortest paths.
- [`CitiesG.csv`](CitiesG.csv): fourteen-vertex supplied input.
- [`tests/`](tests/): regression harness and malformed/disconnected fixtures.
- [`2211 Asn6 F23.pdf`](2211%20Asn6%20F23.pdf): original assignment handout (retained unchanged).

## Format and assumptions

The current loader expects a title row, followed by a `City,A,B,...` header and a square CSV adjacency matrix with matching `A`, `B`, ... row labels. The graph must have 1–26 vertices, zero diagonal, symmetric non-negative integer weights, and 0 for no edge. Because `Vname()` returns a single character, vertex labels beyond Z are not supported even though the legacy storage arrays have capacity for 50 vertices. Edges must have positive weights; zero-weight edges are not represented. Large path sums that exceed the C++ `int` range are not supported.

The implementation uses an adjacency matrix and an O(V²) Dijkstra scan; it is not optimized for large sparse graphs. A disconnected vertex is printed as `unreachable`. The historical Windows executable and IDE files remain unchanged; build from the source above instead of relying on the old binary or machine-specific debugger paths.

## Attribution and provenance

This repository contains course-provided graph/edge scaffolding and assignment data together with the student's implementation. The exact division of authorship within the original scaffolding is not independently established. Original assignment materials and Git history are retained. No benchmark, independent grading result, or production deployment is claimed.
