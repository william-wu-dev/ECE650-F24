//
// Created by Yulin Wu on 2024-09-28.
//

#include "Graph.h"
#include "GeneralException.h"
#include <string>
#include <algorithm>

namespace a2 {
    Graph::Graph(int vertexCount) : vertexCount(vertexCount) {
    }

    void Graph::addEdge(int from, int to, int weight) noexcept(false) {
        // check vertex validity
        if (from > vertexCount || from <= 0) {
            std::string message = "vertex ";
            message += std::to_string(from);
            message += " of edge <";
            message += std::to_string(from);
            message += ", ";
            message += std::to_string(to);
            message += "> is not defined.";
            throw GeneralException(message);
        }
        if (to > vertexCount || to <= 0) {
            std::string message = "vertex ";
            message += std::to_string(to);
            message += " of edge <";
            message += std::to_string(from);
            message += ", ";
            message += std::to_string(to);
            message += "> is not defined.";
            throw GeneralException(message);

        }
        if (from == to) {
            std::string message = "edge <";
            message += std::to_string(from);
            message += ", ";
            message += std::to_string(to);
            message += "> incident on the same vertex is not allowed.";
            throw GeneralException(message);
        }
        // check duplicated edges
        for (auto & edge : edges) {
            if (edge.from == from && edge.to == to) {
                std::string message = "edge <";
                message += std::to_string(from);
                message += ", ";
                message += std::to_string(to);
                message += "> already added.";
                throw GeneralException(message);
            }
        }
        // add edges
        edges.push_back({from, to, weight});
        edges.push_back({to, from, weight}); // undirected graph should add both opposite-directed edges
    }

    std::string Graph::compute(int from, int to) noexcept(false) {
        // check vertex validity
        if (from > vertexCount || from <= 0) {
            std::string message = "vertex ";
            message += std::to_string(from);
            message += " is not defined.";
            throw GeneralException(message);
        }
        if (to > vertexCount || to <= 0) {
            std::string message = "vertex ";
            message += std::to_string(to);
            message += " is not defined.";
            throw GeneralException(message);
        }
        if (from == to) {
            std::string message = "asking for a shortest path from one vertex ";
            message += std::to_string(from);
            message += " to itself is not allowed.";
            throw GeneralException(message);
        }
        // initialization
        std::vector<int> distances(vertexCount + 1, 0x3f3f3f3f);
        // vertex number starts from 1, initialize distance to inf
        std::vector<int> parents(vertexCount + 1, -1); // initialize parent to -1
        distances[from] = 0;
        // iterate for |V| - 1 times
        bool flag = true; // early stop flag
        for (int i = 1; i <= vertexCount - 1 && flag; i++) {
            flag = false; // if the distance will not change in this iteration, halt
            for (auto edge: edges) {
                // relax job
                int u = edge.from;
                int v = edge.to;
                int weight = edge.weight;
                if (distances[v] > distances[u] + weight) {
                    distances[v] = distances[u] + weight;
                    parents[v] = u; // if relaxed, mark the parent
                    flag = true; // the distance indeed changes
                }
            }
        }

        // check if "to" is reachable from "from"
        if (distances[to] > 0x3f3f3f3f / 2) {
            // "to" cannot be reached from "from" throw exception
            std::string message;
            message += std::to_string(to);
            message += " is not reachable from ";
            message += std::to_string(from);
            message += ".";
            throw GeneralException(message);
        }

        // generate path from "from" to "to"
        std::vector<int> path;
        path.push_back(to);
        int parent = parents[to];
        while (parent != -1) {
            path.push_back(parent);
            parent = parents[parent];
        }
        // reverse this path
        std::reverse(path.begin(), path.end());
        // print out the path
        std::string result;
        for (int i = 0; i < path.size(); i++) {
            if (i) {
                result += "-";
            }
            result += std::to_string(path[i]);
        }
        return result;
    }

    void Graph::reset(int vertexCount) {
        if (vertexCount <= 1) {  // invalid format
            // vertex count should be geq 2. (Post 107)
            std::string message = "vertex specification ";
            message += std::to_string(vertexCount);
            message += " is invalid.";
            throw GeneralException(message);
        }
        this->vertexCount = vertexCount;
        this->edges.clear();
    }
} // a2
