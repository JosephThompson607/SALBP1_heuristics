//
// Created by Joseph Thompson on 2026-09-08.
//

#ifndef REPAIRHOFF_H
#define REPAIRHOFF_H
#include <unordered_set>

#include "../albp_solution.h"
#include "../ALBP.h"
#include "./MultiHoff.h"


class RepairHoff {
public:
    explicit RepairHoff(const ALBP& albp, int max_attempts = 5000,
                        const std::optional<std::vector<float>>& alpha_schedule = std::nullopt,
                        const std::optional<std::vector<float>>& beta_schedule = std::nullopt,
                        std::optional<float> gamma= std::nullopt,
                        const std::optional<std::vector<int>>& task_priorities = std::nullopt,
                        const std::optional<unsigned int> seed= std::nullopt):
            albp_(albp),
           max_attempts_(max_attempts),
           alpha_sched_(alpha_schedule),
           beta_sched_(beta_schedule),
           gamma_(gamma),
           task_priorities_(task_priorities.value_or(std::vector<int>{})),
           seed_(seed),
           sub_albp_(albp) ,
                orig_solution_(albp_.N){}
            ;

    ALBPSolution solve(const std::vector<std::vector<int>>&  station_assignments, const std::vector<std::vector<int>>& added_edges, const std::string& how);
private:
    const ALBP& albp_;
    const int max_attempts_ = 5000;
    const std::optional<std::vector<float>>& alpha_sched_ ;
    const std::optional<std::vector<float>>& beta_sched_ ;
    std::optional<float> gamma_;
    const std::optional<std::vector<int>> task_priorities_;
    const std::optional<unsigned int> seed_;
    ALBPSolution orig_solution_;
    std::pair<int,int> process_stations(const std::vector<std::vector<int>> &station_assignments, const std::unordered_set<int>& parents, const std::unordered_set<int>& children);

   // ALBPSolution solve_one_pass(int left);

    ALBP sub_albp_;
};
ALBPSolution rep_mhh_salbp1(const ALBP& albp,  const std::vector<std::vector<int>>&  station_assignments, const std::vector<std::vector<int>>& added_edges, const std::string& how, int max_attempts = 5000,
                        const std::optional<std::vector<float>>& alpha_schedule = std::nullopt,
                        const std::optional<std::vector<float>>& beta_schedule = std::nullopt,
                        std::optional<float> gamma= std::nullopt,
                        const std::optional<std::vector<int>>& task_priorities = std::nullopt,
                        std::optional<unsigned int> seed= std::nullopt);



#endif //REPAIRHOFF_H
