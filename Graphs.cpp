#include "Graphs.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
std::string trim(const std::string& text) {
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

std::vector<std::string> csvFields(const std::string& line) {
    std::vector<std::string> fields;
    std::string field;
    bool quoted = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        const char c = line[i];
        if (c == '"') {
            if (quoted && i + 1 < line.size() && line[i + 1] == '"') {
                field += '"';
                ++i;
            } else {
                quoted = !quoted;
            }
        } else if (c == ',' && !quoted) {
            fields.push_back(trim(field));
            field.clear();
        } else {
            field += c;
        }
    }
    if (quoted) throw std::runtime_error("Unclosed CSV quote");
    fields.push_back(trim(field));
    return fields;
}

int parseWeight(const std::string& field) {
    std::size_t consumed = 0;
    int weight = 0;
    try {
        weight = std::stoi(field, &consumed);
    } catch (const std::exception&) {
        throw std::runtime_error("Invalid edge weight: " + field);
    }
    if (consumed != field.size() || weight < 0)
        throw std::runtime_error("Invalid non-negative edge weight: " + field);
    return weight;
}
} // namespace

Graphs::Graphs() : V(0), E(0), order(0) {
    for (int i = 0; i < Vmax; ++i) {
        val[i] = 0;
        via[i] = -1;
        distance[i] = std::numeric_limits<int>::max();
        processed[i] = false;
        for (int j = 0; j < Vmax; ++j) AdjMatrix[i][j] = 0;
    }
}
Graphs::~Graphs() = default;

char Graphs::Vname(const int s) const {
    if (s < 0 || s >= V || s >= 26) throw std::out_of_range("Vertex index");
    return static_cast<char>('A' + s);
}

void Graphs::getGraph(std::string fname) {
    std::ifstream source(fname);
    if (!source) throw std::runtime_error("Cannot open graph file: " + fname);
    std::string title, heading;
    if (!std::getline(source, title) || !std::getline(source, heading))
        throw std::runtime_error("Graph CSV requires a title and header row");
    const auto columns = csvFields(heading);
    if (columns.size() < 2 || columns[0] != "City")
        throw std::runtime_error("Expected City,A,B,... graph header");
    const int vertices = static_cast<int>(columns.size()) - 1;
    if (vertices > Vmax || vertices > 26)
        throw std::runtime_error("Graph exceeds supported A-Z vertex labels");
    for (int i = 0; i < vertices; ++i)
        if (columns[i + 1] != std::string(1, static_cast<char>('A' + i)))
            throw std::runtime_error("Unexpected vertex column order");

    std::array<std::array<int, Vmax>, Vmax> parsed{};
    std::string line;
    for (int row = 0; row < vertices; ++row) {
        if (!std::getline(source, line)) throw std::runtime_error("Missing graph row");
        const auto cells = csvFields(line);
        if (cells.size() != columns.size() ||
            cells[0] != std::string(1, static_cast<char>('A' + row)))
            throw std::runtime_error("Wrong column count or vertex row label");
        for (int col = 0; col < vertices; ++col)
            parsed[row][col] = parseWeight(cells[col + 1]);
    }
    while (std::getline(source, line))
        if (!trim(line).empty()) throw std::runtime_error("Unexpected extra graph row");
    for (int i = 0; i < vertices; ++i) {
        if (parsed[i][i] != 0) throw std::runtime_error("Diagonal must be zero");
        for (int j = i + 1; j < vertices; ++j)
            if (parsed[i][j] != parsed[j][i])
                throw std::runtime_error("Undirected matrix must be symmetric");
    }
    // Commit only after validation, so malformed input leaves the old graph intact.
    V = vertices;
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j) AdjMatrix[i][j] = parsed[i][j];
    getEdges();
}

void Graphs::dispGraph() const {
    std::cout << "Adjacency Matrix\n";
    for (int i = 0; i < V; ++i) {
        for (int j = 0; j < V; ++j)
            std::cout << std::setw(3) << AdjMatrix[i][j] << ' ';
        std::cout << '\n';
    }
}

void Graphs::getEdges() {
    E = 0;
    for (int i = 0; i < V; ++i)
        for (int j = i + 1; j < V; ++j)
            if (AdjMatrix[i][j] > 0) edges[E++] = Edge{i, j, AdjMatrix[i][j]};
}
int Graphs::No_of_Verices() const { return V; }
int Graphs::No_of_Edges() const { return E; }
void Graphs::printEdge(Edge e) const {
    std::cout << Vname(e.u) << ' ' << Vname(e.v) << ' ' << e.w << '\n';
}
void Graphs::dispEdges() const {
    std::cout << "Graph Edges\n";
    for (int i = 0; i < E; ++i) printEdge(edges[i]);
}

int Graphs::minDistance(int dist[], bool sptSet[]) {
    int selected = -1;
    for (int i = 0; i < V; ++i)
        if (!sptSet[i] && dist[i] != std::numeric_limits<int>::max() &&
            (selected == -1 || dist[i] < dist[selected])) selected = i;
    return selected;
}

void Graphs::shPath(int src) {
    if (src < 0 || src >= V) throw std::out_of_range("Source vertex index");
    const int inf = std::numeric_limits<int>::max();
    for (int i = 0; i < V; ++i) {
        distance[i] = inf;
        via[i] = -1;
        processed[i] = false;
    }
    distance[src] = 0;
    for (int step = 0; step < V; ++step) {
        const int u = minDistance(distance, processed);
        if (u == -1) break; // Remaining vertices are unreachable.
        processed[u] = true;
        for (int v = 0; v < V; ++v) {
            const int weight = AdjMatrix[u][v];
            if (!processed[v] && weight > 0 && distance[u] <= inf - weight &&
                distance[u] + weight < distance[v]) {
                distance[v] = distance[u] + weight;
                via[v] = u;
            }
        }
    }
    std::cout << "Shortest Paths from Node " << Vname(src) << '\n';
    for (int i = 0; i < V; ++i) {
        if (distance[i] == inf) {
            std::cout << "unreachable " << Vname(i) << '\n';
        } else {
            std::cout << distance[i] << ' ';
            printPath(src, i);
            std::cout << '\n';
        }
    }
}

void Graphs::printPath(int src, int dst) const {
    if (dst != src) {
        if (via[dst] < 0) throw std::logic_error("Missing predecessor");
        printPath(src, via[dst]);
    }
    std::cout << Vname(dst);
}
void Graphs::visit(int k) {
    val[k] = ++order;
    std::cout << Vname(k) << ' ';
    for (int i = 0; i < V; ++i)
        if (AdjMatrix[k][i] > 0 && val[i] == 0) visit(i);
}
void Graphs::DFS() {
    order = 0;
    for (int i = 0; i < V; ++i) val[i] = 0;
    std::cout << "DFS: ";
    for (int i = 0; i < V; ++i)
        if (val[i] == 0) visit(i);
    std::cout << '\n';
}
void Graphs::printSolution(int dist[]) {
    std::cout << "Vertex Distance\n";
    for (int i = 0; i < V; ++i) std::cout << Vname(i) << ' ' << dist[i] << '\n';
}
