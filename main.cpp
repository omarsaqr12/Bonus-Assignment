#include "Graphs.h"

#include <exception>
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    const std::string path = argc > 1 ? argv[1] : "TestG.csv";
    try {
        Graphs graph;
        graph.getGraph(path);
        std::cout << "Vertices: " << graph.No_of_Verices()
                  << ", edges: " << graph.No_of_Edges() << '\n';
        graph.dispGraph();
        graph.dispEdges();
        graph.DFS();
        graph.shPath(0);
    } catch (const std::exception& error) {
        std::cerr << "Graph error: " << error.what() << '\n';
        return 1;
    }
}
