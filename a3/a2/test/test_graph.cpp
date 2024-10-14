//
// Created by Yulin Wu on 2024-09-28.
//

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "../GeneralException.h"
#include "../Graph.h"

TEST_CASE("Check Graph Computer Correct 1") {
    auto graph = a2::Graph(15);
    graph.addEdge(2, 6, 1);
    graph.addEdge(2, 8, 1);
    graph.addEdge(2, 5, 1);
    graph.addEdge(6, 5, 1);
    graph.addEdge(5, 8, 1);
    graph.addEdge(6, 10, 1);
    graph.addEdge(10, 8, 1);
    std::string result_1 = graph.compute(2, 10);
    bool check_1 = result_1 == "2-8-10" || result_1 == "2-6-10";
    CHECK(check_1);

    std::string result_2 = graph.compute(10, 5);
    bool check_2 = result_2 == "10-6-5" || result_2 == "10-8-5";
    CHECK(check_2);

    std::string result_3 = graph.compute(5, 8);
    CHECK(result_3 == "5-8");
}

TEST_CASE("Check Graph Computer Correct 2") {
    auto graph = a2::Graph(5);
    graph.addEdge(1, 3, 1);
    graph.addEdge(3, 2, 1);
    graph.addEdge(3, 4, 1);
    graph.addEdge(4, 5, 1);
    graph.addEdge(5, 2, 1);
    std::string result_1 = graph.compute(5, 1);
    bool check_1 = result_1 == "5-2-3-1" || result_1 == "5-4-3-1";
    CHECK(check_1);

    std::string result_2 = graph.compute(2, 4);
    bool check_2 = result_2 == "2-3-4" || result_2 == "2-5-4";
    CHECK(check_2);

    std::string result_3 = graph.compute(1, 4);
    CHECK(result_3 == "1-3-4");
}

TEST_CASE("Check No Path Error") {
    auto graph = a2::Graph(15);
    graph.addEdge(2, 6, 1);
    graph.addEdge(2, 8, 1);
    graph.addEdge(2, 5, 1);
    graph.addEdge(6, 5, 1);
    graph.addEdge(5, 8, 1);
    graph.addEdge(6, 10, 1);
    graph.addEdge(10, 8, 1);
    CHECK_THROWS_WITH_AS(graph.compute(1, 2), "2 is not reachable from 1.", a2::GeneralException);
    CHECK_THROWS_WITH_AS(graph.compute(10, 15), "15 is not reachable from 10.", a2::GeneralException);
}

TEST_CASE("Check Add Duplicated Edge") {
    auto graph = a2::Graph(15);
    graph.addEdge(2, 6, 1);
    CHECK_THROWS_WITH_AS(graph.addEdge(2, 6, 1), "edge <2, 6> already added.", a2::GeneralException);
}

TEST_CASE("Check Add Over-Limit Vertex") {
    auto graph = a2::Graph(5);
    CHECK_THROWS_WITH_AS(graph.addEdge(2, 6, 1), "vertex 6 of edge <2, 6> is not defined.", a2::GeneralException);
    CHECK_THROWS_WITH_AS(graph.addEdge(6, 2, 1), "vertex 6 of edge <6, 2> is not defined.", a2::GeneralException);
}

TEST_CASE("Check Add Self Loop") {
    auto graph = a2::Graph(15);
    CHECK_THROWS_WITH_AS(graph.addEdge(2, 2, 1), "edge <2, 2> incident on the same vertex is not allowed.", a2::GeneralException);
}

TEST_CASE("Check Reset Invalid") {
    auto graph = a2::Graph(0);
    CHECK_THROWS_WITH_AS(graph.reset(0), "vertex specification 0 is invalid.", a2::GeneralException);
}