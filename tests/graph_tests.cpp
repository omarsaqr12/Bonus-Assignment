#include "Graphs.h"

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace {
void require(bool condition, const std::string& message) {
    if (!condition) throw std::runtime_error(message);
}

std::string paths(Graphs& graph, int start) {
    std::ostringstream buffer;
    auto* previous = std::cout.rdbuf(buffer.rdbuf());
    try { graph.shPath(start); }
    catch (...) { std::cout.rdbuf(previous); throw; }
    std::cout.rdbuf(previous);
    return buffer.str();
}

bool loadThrows(Graphs& graph, const std::string& path) {
    try { graph.getGraph(path); }
    catch (const std::runtime_error&) { return true; }
    return false;
}
}

int main() {
    try {
        Graphs graph;
        graph.getGraph("TestG.csv");
        require(graph.No_of_Verices() == 7, "sample graph vertex count");
        require(graph.No_of_Edges() == 10, "sample graph edge count");
        const auto sample = paths(graph, 0);
        require(sample.find("66 ABDF\n") != std::string::npos, "sample A-F path");
        require(sample.find("82 ABDFG\n") != std::string::npos, "sample A-G path");
        graph.DFS();

        graph.getGraph("CitiesG.csv");
        require(graph.No_of_Verices() == 14, "larger dataset vertex count");
        require(paths(graph, 0).find("53 AN\n") != std::string::npos,
                "larger dataset shortest path A-N");
        graph.DFS();
        require(loadThrows(graph, "tests/no-such-graph.csv"), "missing file must fail");
        require(graph.No_of_Verices() == 14, "failure preserves previously loaded graph");

        Graphs disconnected;
        disconnected.getGraph("tests/disconnected.csv");
        require(disconnected.No_of_Verices() == 3, "disconnected graph vertex count");
        require(paths(disconnected, 0).find("unreachable C\n") != std::string::npos,
                "disconnected vertex must be reported");
        disconnected.DFS();
        bool invalidSource = false;
        try { disconnected.shPath(3); }
        catch (const std::out_of_range&) { invalidSource = true; }
        require(invalidSource, "source bounds check");
        require(loadThrows(disconnected, "tests/asymmetric.csv"), "reject asymmetric graph");
        require(disconnected.No_of_Verices() == 3, "invalid graph does not replace old data");
        std::cout << "PASS: graph regression checks\n";
    } catch (const std::exception& error) {
        std::cerr << "FAIL: " << error.what() << '\n';
        return 1;
    }
}
