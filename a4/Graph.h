//
// Created by Yulin Wu on 2024-09-28.
//

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

namespace a4 {
    struct Edge {
        int from; // vertex incident from the edge
        int to; // vertex incident to the edge
        int weight; // weight of this edge
    };

    class Graph {
    private:
        int vertexCount; // count the number of vertices; vertex id start at 1
        std::vector<Edge> edges;
        // store the edges information; we will use bellman-ford algorithm; sequence doesn't matter
    public:
        explicit Graph(int vertexCount);

        void addEdge(int from, int to, int weight);

        std::string compute() const;

        void reset(int vertexCount);

        std::string toString() const;
    };
} // a2

#endif //GRAPH_H
