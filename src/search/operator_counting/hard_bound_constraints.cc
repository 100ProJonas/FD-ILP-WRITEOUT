
#include "hard_bound_constraints.h"

#include "../lp/lp_solver.h"
#include "../plugins/plugin.h"

#include <memory>

using namespace std;

namespace operator_counting {

HardBoundConstraints::HardBoundConstraints(
    const shared_ptr<AbstractTask> &task,
    int upper_bound)
    : ConstraintGenerator(task),
      upper_bound(upper_bound) {
}

void HardBoundConstraints::initialize_constraints(
    const shared_ptr<AbstractTask> &task,
    lp::LinearProgram &lp) {

    auto &variables = lp.get_variables();

    TaskProxy task_proxy(*task);
    size_t num_operators = task_proxy.get_operators().size();

    // Apply bounds only to operator-counting variables.
    for (size_t i = 0; i < num_operators; ++i) {
        variables[i].lower_bound = 0.0;
        variables[i].upper_bound = upper_bound;
    }
}

bool HardBoundConstraints::update_constraints(
    [[maybe_unused]] const State &state, [[maybe_unused]] lp::LPSolver &lp_solver) {

    // Bounds are permanent and independent of the state.
    return false;
}

// Plugin registration

class HardBoundConstraintsFeature
    : public plugins::TypedFeature<TaskIndependentConstraintGenerator> {

public:
    HardBoundConstraintsFeature()
        : TypedFeature("hard_bound_constraints") {

        document_title("Hard bound constraints");

        document_synopsis(
            "Restricts all operator-counting LP variables "
            "to values between 0 and a configurable upper bound.");

        add_option<int>(
            "upper_bound",
            "Upper bound for every operator-counting LP variable.",
            "10");
    }

    virtual shared_ptr<TaskIndependentConstraintGenerator>
    create_component(const plugins::Options &opts) const override {

        return components::make_auto_task_independent_component<
            HardBoundConstraints, ConstraintGenerator>(
                opts.get<int>("upper_bound"));
    }
};

static plugins::FeaturePlugin<HardBoundConstraintsFeature> _plugin;

}

// Plugin registration

class HardBoundConstraintsFeature
    : public plugins::TypedFeature<operator_counting::TaskIndependentConstraintGenerator> {
public:
    HardBoundConstraintsFeature(): TypedFeature("hard_bound_constraints") {}

    virtual shared_ptr<operator_counting::TaskIndependentConstraintGenerator>
    create_component(const plugins::Options &opts) const override {
        return components::make_auto_task_independent_component<
            operator_counting::HardBoundConstraints,
            operator_counting::ConstraintGenerator>(opts.get<int>("upper_bound"));
    };

    static plugins::FeaturePlugin<HardBoundConstraintsFeature> _plugin;
};