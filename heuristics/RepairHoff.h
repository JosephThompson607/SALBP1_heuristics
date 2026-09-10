//
// Created by Joseph Thompson on 2026-09-08.
//

#ifndef REPAIRHOFF_H
#define REPAIRHOFF_H
#include "../albp_solution.h"
#include "../ALBP.h"
#include "./MultiHoff.h"


class RepairHoff: public MultiHoff {
public:
    explicit RepairHoff(const ALBP& albp, int max_attempts = 5000,
                        const std::optional<std::vector<float>>& alpha_schedule = std::nullopt,
                        const std::optional<std::vector<float>>& beta_schedule = std::nullopt,
                        std::optional<float> gamma= std::nullopt,
                        const std::optional<std::vector<int>>& task_priorities = std::nullopt,
                        const std::optional<unsigned int> seed= std::nullopt):
    MultiHoff(  albp, max_attempts, alpha_schedule, beta_schedule, gamma, task_priorities,seed),
    sub_albp_(albp){};

    ALBPSolution solve(const std::vector<std::vector<int>>&  station_assignments, const std::vector<std::vector<int>>& added_edges, const std::string& how);
private:
    ALBP sub_albp_;
};



#endif //REPAIRHOFF_H
