//
// Created by Yulin Wu on 2024-09-28.
//

#include "Graph.h"
#include "GeneralException.h"
#include <string>
#include <algorithm>
#include <iostream>
#include <minisat/core/Solver.h>

#define DEBUG false

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
        for (auto &edge: edges) {
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
        edges.push_back({from, to, weight}); // we only want one direction added.
    }

    std::string Graph::compute() const noexcept(false) {
        if (edges.empty()) {
            const std::string message = "cannot compute vertex cover without edges in graph.";
            throw GeneralException(message);
        }

        // create a solver
        // -- allocate on the heap so that we can reset later if needed
        std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
        // iterate on the size of vertex cover
        for (auto k = 1; k <= vertexCount; k++) {
            // Big Step 1. determine whether this size k is correct

            // declare and bookkeeping all automic propositions
            // x_{i,j} is true iff vertex i of V is the j_{th} vertex in the vertex cover
            // be careful about the indices
            // must initialize size before assigning, do not create unused var.
            std::vector<std::vector<Minisat::Lit> > atomics(
                vertexCount + 1, std::vector<Minisat::Lit>(k + 1, Minisat::lit_Error));
            for (auto i = 1; i <= vertexCount; i++) {
                for (auto j = 1; j <= k; j++) {
#if DEBUG
                    std::cerr << "Before: " << atomics[i][j].x << std::endl;
#endif
                    atomics[i][j] = Minisat::mkLit(solver->newVar());
#if DEBUG
                    std::cerr << "After: " << atomics[i][j].x << std::endl;
#endif
                }
            }

            // Condition 1: At least one vertex is the j_{th} vertex in the vertex cover:
            for (auto j = 1; j <= k; j++) {
                Minisat::vec<Minisat::Lit> clause;
                for (auto i = 1; i <= vertexCount; i++) {
                    clause.push(atomics[i][j]);
                }
                solver->addClause(clause);
            }

            // Condition 2: No one vertex can appear twice in a vertex cover.
            // In other words, it is not the case that vertex m appears both in positions p and q of the vertex cover.
            // "i" denotes the vertex, "p", "q" is the position in vertex cover, p < q
            for (auto i = 1; i <= vertexCount; i++) {
                for (auto p = 1; p <= k - 1; p++) {
                    for (auto q = p + 1; q <= k; q++) {
                        // Minisat::vec<Minisat::Lit> clause;
                        // clause.push(~atomics[i][p]);
                        // clause.push(~atomics[i][q]);
                        // solver->addClause(clause);
                        solver->addClause(~atomics[i][p], ~atomics[i][q]);
                    }
                }
            }

            // Condition 3: No more than one vertex appears in the mth position of the vertex cover.
            // "j" denotes the position, "p", "q" is the names of vertices, p < q
            for (auto j = 1; j <= k; j++) {
                for (auto p = 1; p <= vertexCount - 1; p++) {
                    for (auto q = p + 1; q <= vertexCount; q++) {
                        // Minisat::vec<Minisat::Lit> clause;
                        // clause.push(~atomics[p][j]);
                        // clause.push(~atomics[q][j]);
                        // solver->addClause(clause);
                        solver->addClause(~atomics[p][j], ~atomics[q][j]);
                    }
                }
            }

            // Condition 4: Every edge is incident to at least one vertex in the vertex cover.
            for (const auto edge: edges) {
                const auto from = edge.from;
                const auto to = edge.to;
                Minisat::vec<Minisat::Lit> clause;
                for (auto j = 1; j <= k; j++) {
                    clause.push(atomics[from][j]);
                    clause.push(atomics[to][j]);
                }
                // for (auto j = 1; j <= k; j++) {
                //     clause.push(atomics[to][j]);
                // }
                solver->addClause(clause);
            }

            // solve it now
            bool res = solver->solve();

            // Big Step 2. if is correct, generate and return result
            if (res) {
#if DEBUG
                for (auto j = 1; j <= k; j++) {
                    for (auto i = 1; i <= vertexCount; i++) {
                        // const auto value = Minisat::toInt(solver->modelValue(atomics[i][j]));
                        std::cerr << "atomics[" << i << "][" << j << "]: " << Minisat::toInt(solver->modelValue(atomics[i][j]))
                        << " sign: " << Minisat::sign(atomics[i][j])
                        << " var: " << Minisat::var(atomics[i][j])
                        << " var value: " << Minisat::toInt(solver->modelValue(Minisat::var(atomics[i][j])))
                        << std::endl;
                    }
                }
#endif
                std::vector<int> result; // contains index of vertices in vertex cover, ascending order
                // generate result
                for (auto j = 1; j <= k; j++) {
                    for (auto i = 1; i <= vertexCount; i++) {
                        const auto value = Minisat::toInt(solver->modelValue(atomics[i][j]));
                        if (value == 0) {
                            // WARNING: 0 in value means assign true to the variable, this is mind-blowing!
                            // means vertex "i" is in the vertex cover.
                            result.push_back(i);
                            break;
                        }
                    }
                }
                // sort result
                std::sort(result.begin(), result.end());
                // convert result to string
                std::string result_str;
                for (size_t ii = 0; ii < result.size(); ii++) {
                    if (ii) {
                        result_str += " ";
                    }
                    result_str += std::to_string(result[ii]);
                }
                // return result
                return result_str;
            } else {
                // Big Step 2.1 else, reset solver and iterate on the next size
                // reset the solver each time after we change the size of vertex cover.
                // the next line de-allocates existing solver and allocates a new
                // one in its place.
                solver.reset(new Minisat::Solver());
            }
        }

        // raise error, must find a vertex cover already; because there must be one vertex cover with size < vertexCount
        std::string message = "no vertex cover found, might be internal error.";
        throw GeneralException(message);
    }

    void Graph::reset(int vertexCount) {
        if (vertexCount <= 1) {
            // invalid format
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
        for (const auto edge: edges) {
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
