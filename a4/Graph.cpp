//
// Created by Yulin Wu on 2024-09-28.
//

#include "Graph.h"
#include "GeneralException.h"
#include <string>
#include <algorithm>

namespace a4 {
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
                std::string message = "undirected edge <";
                message += std::to_string(from);
                message += ", ";
                message += std::to_string(to);
                message += "> already added.";
                throw GeneralException(message);
            } else if (edge.from == to && edge.to == from) {
                std::string message = "undirected edge <";
                message += std::to_string(from);
                message += ", ";
                message += std::to_string(to);
                message += "> already added as edge <";
                message += std::to_string(edge.from);
                message += ", ";
                message += std::to_string(edge.to);
                message += ">.";
                throw GeneralException(message);
            }

        }
        // add edges
        edges.push_back({from, to, weight});  // we only want one direction added.
    }

    std::string Graph::compute() noexcept(false) {
       std::string result = "";
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

    std::string Graph::toString() const {
        std::string result;
        // vertex
        result += "V ";
        result += std::to_string(this->vertexCount);
        result += "\n";
        // edge
        result += "E {";
        for (const auto edge : edges) {
            result += "<";
            result += std::to_string(edge.from);
            result += ",";
            result += std::to_string(edge.to);
            result += ">,";
        }
        result += "}";
        return result;
    }
} // a2
