//
// Created by Yulin Wu on 2024-09-28.
//

#include "Graph.h"
#include "GeneralException.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <minisat/core/Solver.h>
#include <memory>

#define DEBUG_SINGLE false
#define DEBUG_ITERATION true
#define DEBUG_RANDOM false

namespace project {
    /**
     * generate random integer in range [min, max]
     * @param min floor of random integer, inclusive
     * @param max ceiling of random integer, inclusive
     * @return random integer range in [min, max]
     */
    int randint(const int min, const int max) noexcept(false) {
#if DEBUG_RANDOM
        std::cerr << "Random number generator using " << min << " to " << max << std::endl;
#endif

        if (min == max) {
            return min;
        }

        // open /dev/urandom to read
        std::ifstream urandom("/dev/urandom");

        // check that it did not fail
        if (urandom.fail()) {
            throw GeneralException("unable to open /dev/urandom");
        }

        // start reading, unsigned int reading only
        unsigned int x = 0;
        urandom.read(reinterpret_cast<char *>(&x), sizeof(unsigned int));

        // calculated in unsigned then transferred to int. after the modulo, it is guaranteed that the result is in the int
        // range. Unless, b - a + 1 exceed in range. 2147483647
        const int mid = static_cast<int>(x % static_cast<unsigned int>(max - min + 1));

        const int res = mid + min;

        // close random stream
        urandom.close();

        return res;
    }

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

    std::vector<int> Graph::CNFSatVC(const bool *flag) const noexcept(false) {
        if (edges.empty()) {
            const std::string message = "cannot compute vertex cover without edges in graph.";
            throw GeneralException(message);
        }

        // create a solver
        // -- allocate on the heap so that we can reset later if needed
        std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
        // iterate on the size of vertex cover
        for (auto k = 1; k <= vertexCount; k++) {
            if (*flag) {
#if DEBUG_ITERATION
                std::cerr << "Termination Flag Received: " << flag << std::endl;
#endif
                std::vector<int> dummy;
                return dummy;
            }
            // Big Step 1. determine whether this size k is correct
#if DEBUG_ITERATION
            std::cerr << "Iteration on k: " << k << std::endl;
#endif
            // declare and bookkeeping all automic propositions
            // x_{i,j} is true iff vertex i of V is the j_{th} vertex in the vertex cover
            // be careful about the indices
            // must initialize size before assigning, do not create unused var.
            std::vector<std::vector<Minisat::Lit> > atomics(
                vertexCount + 1, std::vector<Minisat::Lit>(k + 1, Minisat::lit_Error));
            for (auto i = 1; i <= vertexCount; i++) {
                for (auto j = 1; j <= k; j++) {
#if DEBUG_SINGLE
                    std::cerr << "Before: " << atomics[i][j].x << std::endl;
#endif
                    atomics[i][j] = Minisat::mkLit(solver->newVar());
#if DEBUG_SINGLE
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
#if DEBUG_SINGLE
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
                // return result
                return result;
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

    std::vector<int> Graph::ApproxVC1() const {
        // copy edges first
        auto copied_edges = this->edges;

        // initialize a result
        std::vector<int> result;

        // while the copy is not empty
        while (!copied_edges.empty()) {
            // compute degree
            std::vector<int> degrees(vertexCount + 1, 0);
            for (const auto edge: copied_edges) {
                degrees[edge.from] += 1;
                degrees[edge.to] += 1;
            }

            // pick the vertex with the maximum degree and add to result
            int maxDegreeVertexIndex = 1;
            for (int i = 2; i <= vertexCount; i++) {
                if (degrees[i] > degrees[maxDegreeVertexIndex]) {
                    maxDegreeVertexIndex = i;
                }
            }
            result.push_back(maxDegreeVertexIndex);

            // erase edges incident on the vertex from copied, iterate backward
            for (int i = static_cast<int>(copied_edges.size() - 1); i >= 0; i--) {
                const auto &edge = copied_edges[i];
                if (edge.from == maxDegreeVertexIndex || edge.to == maxDegreeVertexIndex) {
                    copied_edges.erase(copied_edges.begin() + i);
                }
            }
        }

        // generate result
        std::sort(result.begin(), result.end());
        return result;
    }

    std::vector<int> Graph::ApproxVC2() const {
        // copy edges first
        auto copied_edges = this->edges;

        // initialize a result
        std::vector<int> result;

        // while the copy is not empty
        while (!copied_edges.empty()) {
            // randomly pick an edge
            int edgeIndex = randint(0, static_cast<int>(copied_edges.size() - 1));
#if DEBUG_RANDOM
            std::cerr << "edgeIndex: " << edgeIndex << std::endl;
#endif
            const auto &edgePicked = copied_edges[edgeIndex];

            // add u, v to the result
            const auto u = edgePicked.from;
            const auto v = edgePicked.to;
            result.push_back(u);
            result.push_back(v);

            // erase edges incident on u, v
            for (int i = static_cast<int>(copied_edges.size() - 1); i >= 0; i--) {
                const auto &edge = copied_edges[i];
                if (edge.from == u || edge.to == u || edge.from == v || edge.to == v) {
                    copied_edges.erase(copied_edges.begin() + i);
                }
            }
        }

        // generate result
        std::sort(result.begin(), result.end());
        return result;
    }
} // a2
