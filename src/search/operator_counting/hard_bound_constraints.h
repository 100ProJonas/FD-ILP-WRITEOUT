#ifndef OPERATOR_COUNTING_HARD_BOUND_CONSTRAINTS_H
#define OPERATOR_COUNTING_HARD_BOUND_CONSTRAINTS_H

#include "constraint_generator.h"

namespace operator_counting {

class HardBoundConstraints : public ConstraintGenerator {
public:
    explicit HardBoundConstraints(
        const std::shared_ptr<AbstractTask> &task,
        int upper_bound);

    void initialize_constraints(
        const std::shared_ptr<AbstractTask> &task,
        lp::LinearProgram &lp) override;

    bool update_constraints(
        const State &state,
        lp::LPSolver &lp_solver) override;

private:
    int upper_bound;
};

}
#endif