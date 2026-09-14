//
// Created by Joseph Thompson on 2026-09-08.
//

#include "RepairHoff.h"

#include <cassert>
#include <unordered_map>
#include <unordered_set>
#include "../albp_solution.h"
#include "../ALBP.h"
#include "MultiHoff.h"
#include "salbp_basics.h"


// ALBP make_subgraph(const ALBP& orig) {
//
// }


std::pair<int,int> RepairHoff::process_stations(const std::vector<std::vector<int>> &station_assignments, const std::unordered_set<int>& parents, const std::unordered_set<int>& children) {
    //FInds the leftmost station
    int left_station = std::numeric_limits<int>::max();
    int right_station = 0;

    for (int s = 0; s < station_assignments.size(); ++s) {
        for (auto& task : station_assignments[s]) {
            if (children.contains(task) && s < left_station) {
                left_station = s;
            }
            if (parents.contains(task)   && s > right_station) {
                right_station = s;
            }
        }
    }
    return std::make_pair(left_station, right_station);
}





ALBPSolution RepairHoff::solve( const std::vector<std::vector<int>>&  station_assignments, const std::vector<std::vector<int>>& added_edges, const std::string& how) {
    std::unordered_set<int> parents;
    std::unordered_set<int> children;
    std::vector<std::vector<int>> new_precs;
    orig_solution_.station_assignments = station_assignments;
    orig_solution_.station_to_task();
    orig_solution_.station_to_load(albp_);
    new_precs.reserve(albp_.precedence_relations.size());
    int lb_6 = calc_salbp_1_bin_lbs(albp_.task_time, albp_.C);
    for (const auto& inner : added_edges) {
        parents.insert(inner[0]-1); //Outside is 1 indexed =/
        children.insert(inner[1]-1);
    }
    int left_station;
    int right_station;
    if (how=="right") {    //Repair everything after the first encountered moved task
        std::pair<int,int> s_bounds = process_stations(station_assignments, parents, children);
        left_station = s_bounds.first;
        right_station = station_assignments.size()-1;
    }
    else if (how=="left") {    //Repair everything up to the last encountered moved task
        std::pair<int,int> s_bounds = process_stations(station_assignments, parents, children);
        left_station = 0;
        right_station = s_bounds.second;
    }
    else if (how=="center") { //Make the solution feasible
        std::pair<int,int> s_bounds = process_stations(station_assignments, parents, children);
        left_station = s_bounds.first;
        right_station = s_bounds.second;

    }
    else if (how=="full") { //recaulate the solution
        left_station = 0;
        right_station = station_assignments.size()-1;
    }
    else {
        throw std::invalid_argument("Unrecognized repair method");
    }
    assert(left_station >= 0);
    assert(right_station >= 0);
    std::vector<int> task_times;
    std::vector<std::vector<int>> edges;
    std::vector<int>tasks;
    std::unordered_map<int, int> task_translation;
    int task_ind = 0;
    std::optional<std::vector<int>> new_priorities;
    if (task_priorities_.has_value()) new_priorities.emplace();
    for (size_t i = left_station; i<= right_station; ++i) {
            for (int task : station_assignments[i]) {
                tasks.push_back(task);
                task_translation.insert({task, task_ind});
                task_times.push_back(albp_.task_time[task]);

                if (task_priorities_.has_value() && !task_priorities_.value().empty()) {
                    new_priorities.value().push_back(task_priorities_.value()[task]);
                }

                task_ind++;
            }
        }


    std::unordered_set<int> allowed_set(tasks.begin(), tasks.end());

    for (PrecedenceRelation prec:albp_.precedence_relations) {
        int child = prec.child;
        int parent = prec.parent;
        if (allowed_set.contains(child) && allowed_set.contains(parent)) {

            int child_ind = task_translation[child] ;
            int parent_ind = task_translation[parent];
            new_precs.push_back({parent_ind, child_ind });
            assert(child== tasks[child_ind] );
            assert(parent == tasks[parent_ind] );
        }
        else {
        }

    }
    sub_albp_ = ALBP::type_1(albp_.C, task_times.size(), task_times, new_precs, false, false, false, false);
    MultiHoff solver(  sub_albp_,
           max_attempts_,
           alpha_sched_,
           beta_sched_,
           gamma_,
           new_priorities,
           seed_);

    ALBPSolution  intermediate = solver.solve();

    //updates the original solution
    //Change in stations is restricted to the subproblem, delta is negative in improvement, positive in deterioration
    int delta =   intermediate.n_stations -(right_station - left_station +1 );
    std::vector<std::vector<int>> new_station_assignments;
    new_station_assignments.reserve(station_assignments.size()+delta);
    for (int i = 0; i < left_station; ++i) {
        if (!station_assignments[i].empty()) {
            new_station_assignments.emplace_back(station_assignments[i]);
        }
    }
    for (auto & station_assignment : intermediate.station_assignments) {
        if (! station_assignment.empty()) {
            for (int task_ind = 0; task_ind< station_assignment.size(); ++task_ind) {
                //Convert assignment back to old notation
                station_assignment[task_ind] = tasks[station_assignment[task_ind]];
            }
            new_station_assignments.emplace_back(station_assignment);
        }
    }
    for (int i = right_station+1; i< station_assignments.size(); ++i) {
        if (!station_assignments[i].empty()){
            new_station_assignments.emplace_back(station_assignments[i]);
        }

    }

    orig_solution_.station_assignments = new_station_assignments;
    orig_solution_.n_stations = orig_solution_.station_assignments.size();
    orig_solution_.station_to_task();
    orig_solution_.station_to_load(albp_);
    orig_solution_.method = "RepairHoff";
    if (orig_solution_.n_stations == lb_6) orig_solution_.optimal=true;
    orig_solution_.n_violations = count_violations(albp_, orig_solution_.task_assignment);
    assert(orig_solution_.n_violations == 0);
    assert(orig_solution_.cycle_time <=albp_.C);
    assert(orig_solution_.n_stations >= lb_6);
    return orig_solution_;
}

ALBPSolution rep_mhh_salbp1(const ALBP& albp,  const std::vector<std::vector<int>>&  station_assignments, const std::vector<std::vector<int>>& added_edges, const std::string& how, int max_attempts ,
                        const std::optional<std::vector<float>>& alpha_schedule ,
                        const std::optional<std::vector<float>>& beta_schedule ,
                        std::optional<float> gamma,
                        const std::optional<std::vector<int>>& task_priorities ,
                        const std::optional<unsigned int> seed) {
    RepairHoff RH(albp,
                max_attempts,
                alpha_schedule ,
                beta_schedule ,
                gamma,
                task_priorities,
                seed);
    return RH.solve(station_assignments, added_edges, how);

}
