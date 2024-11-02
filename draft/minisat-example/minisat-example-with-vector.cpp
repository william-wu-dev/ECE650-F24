//
// Created by Michael Williams on 2024-11-01.
//
// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"

// defined std::cout
#include <iostream>
#include <vector>

int main(void) {
    // -- allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    std::vector<Minisat::Lit> l;
    l.reserve(4);
    for (auto i = 0; i < 4; i++) {
        l.push_back(Minisat::mkLit(solver->newVar()));
    }


    // create 4 positive literals over 4 new variables
    // const auto& l1 = l[0];
    // const auto& l2 = l[1];
    // const auto& l3 = l[2];
    // const auto& l4 = l[3];

    #define l1 l[0]
    #define l2 l[1]
    #define l3 l[2]
    #define l4 l[3]


    // create 3 positive literals over 3 new variables
    std::vector<Minisat::Lit> b;
    b.reserve(3);
    for (auto i = 0; i < 3; i++) {
        b.push_back(Minisat::mkLit(solver->newVar()));
    }

    // const auto& b0 = b[0];
    // const auto& b1 = b[1];
    // const auto& b2 = b[2];

    #define b0 b[0]
    #define b1 b[1]
    #define b2 b[2]

    // (l1 || b0)
    Minisat::vec<Minisat::Lit> clause1;
    clause1.push(l1);
    clause1.push(b0);
    solver->addClause(clause1);
    // (!b0 || l2 || b1)
    Minisat::vec<Minisat::Lit> clause2;
    clause2.push(~b0);
    clause2.push(l2);
    clause2.push(b1);
    solver->addClause(clause2);
    // (!b1 || l3 || b2)
    Minisat::vec<Minisat::Lit> clause3;
    clause3.push(~b1);
    clause3.push(l3);
    clause3.push(b2);
    solver->addClause(clause3);
    // (!b2|| l4)
    Minisat::vec<Minisat::Lit> clause4;
    clause4.push(~b2);
    clause4.push(l4);
    solver->addClause(clause4);

    bool res = solver->solve();
    std::cout << "The result is: " << res << "\n";

    std::cout << "satisfying assignment is: "
              << "l1=" << Minisat::toInt(solver->modelValue(l1)) << " "
              << "l2=" << Minisat::toInt(solver->modelValue(l2)) << " "
              << "l3=" << Minisat::toInt(solver->modelValue(l3)) << " "
              << "l4=" << Minisat::toInt(solver->modelValue(l4)) << " "
              << "b0=" << Minisat::toInt(solver->modelValue(b0)) << " "
              << "b1=" << Minisat::toInt(solver->modelValue(b1)) << " "
              << "b2=" << Minisat::toInt(solver->modelValue(b2)) << std::endl;

    // more clauses can be added to the solver
    std::cout << "Adding more clauses...\n";
    solver->addClause (~l1);
    solver->addClause (~l2);
    solver->addClause (~l3);
    solver->addClause (~l4);

    // check whether the CNF in the solver is still satisfiable
    res = solver->solve();
    std::cout << "New result is: " << res << "\n";

    // the next line de-allocates existing solver and allocates a new
    // one in its place.
    solver.reset (new Minisat::Solver());

    // at this point the solver is ready. You must create new
    // variable and new clauses
    return 0;
}
