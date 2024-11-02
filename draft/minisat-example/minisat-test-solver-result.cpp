//
// Created by Michael Williams on 2024-11-02.
//
//
// Created by Michael Williams on 2024-11-02.
//
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

#define DEBUG false

int main(void) {
    // -- allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());

    const int vertexCount = 2;
    const int k = 4;
    std::vector<std::vector<Minisat::Lit>> atomics_bookkeeping(vertexCount + 1, std::vector<Minisat::Lit>(k + 1, Minisat::lit_Error));
    for (auto i = 1; i <= vertexCount; i++) {
        for (auto j = 1; j <= k; j++) {
#if DEBUG
            std::cout << "Before: " << atomics_bookkeeping[i][j].x << std::endl;
#endif
            atomics_bookkeeping[i][j] = Minisat::mkLit(solver->newVar());
#if DEBUG
            std::cout << "After: " << atomics_bookkeeping[i][j].x << std::endl;
#endif
        }
    }

    #define l1 atomics_bookkeeping[1][1]
    #define l2 atomics_bookkeeping[1][2]
    #define l3 atomics_bookkeeping[1][3]
    #define l4 atomics_bookkeeping[1][4]

    #define b0 atomics_bookkeeping[2][1]
    #define b1 atomics_bookkeeping[2][2]
    #define b2 atomics_bookkeeping[2][3]

    solver->addClause(l1);
    solver->addClause(l2);
    solver->addClause(l3);
    solver->addClause(l4);
    solver->addClause(b0);
    solver->addClause(b1);
    solver->addClause(b2);


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
