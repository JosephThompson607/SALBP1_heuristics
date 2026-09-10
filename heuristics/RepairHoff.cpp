//
// Created by Joseph Thompson on 2026-09-08.
//

#include "RepairHoff.h"

#include <unordered_set>
#include "../albp_solution.h"
#include "../ALBP.h"
#include "MultiHoff.h"



// ALBP make_subgraph(const ALBP& orig) {
//
// }


int find_left_station(const std::vector<std::vector<int>> &station_assignments, const std::unordered_set<int>& parents, const std::unordered_set<int>& children) {
    //FInds the leftmost station
    int left = 0;
    for (const auto& station :station_assignments) {
        for (auto& task : station) {
            if (parents.contains(task)) {
                break;
            }
        }
        left++;
    }
    return left;
}

int find_right_station(const std::vector<std::vector<int>> &station_assignments, const std::unordered_set<int>& parents, const std::unordered_set<int>& children) {
    //FInds the leftmost staation
    int right = 0;

    for (auto it = station_assignments.rbegin(); it != station_assignments.rend(); ++it) {
        const std::vector<int>& station = *it;
        for (auto& task : station) {
            if (children.contains(task) || parents.contains(task)) {
                break;
            }
        }
        right++;
    }
    //Still based off left station
    return station_assignments.size()-right;
}

ALBPSolution RepairHoff::solve( const std::vector<std::vector<int>>&  station_assignments, const std::vector<std::vector<int>>& added_edges, const std::string& how) {

    std::unordered_set<int> parents;
    std::unordered_set<int> children;
    for (const auto& inner : added_edges) {
        parents.insert(inner[0]);
        children.insert(inner[1]);
    }

    //Repair everything after the first encountered moved task
    if (how =="right") {
        std::vector<int> tasks;
        int left = find_left_station(station_assignments, parents, children);
        for (size_t i = 0; i< left; ++i) {
            for (int x : station_assignments[i]) {
                tasks.push_back(x);
            }
        }
     }
    else if (how =="left") {
        std::vector<int> tasks;
        int right = find_right_station(station_assignments, parents, children);
        //for
    }
}
