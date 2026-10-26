//
// Created by Joseph Thompson on 2026-10-26.
//

#ifndef INCOMPARABLES_H
#define INCOMPARABLES_H
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>

#include "ALBP.h"

void check_t_time_increase( const ALBP& albp,
    int i,
    int j,
    std::vector<bool>& increasables,
    const std::vector<double>* heads = nullptr,
    const std::vector<double>* tails = nullptr,
    int ub = 0);
bool chain_is_dead(
    const ALBP& albp,
    int ub,
    const std::vector<double>& heads,
    const std::vector<double>& tails,
    int first,
    int second
);
struct Incomparables {
    std::vector<std::vector<int>> valid_edges;
    std::vector<std::vector<int>> deduced_edges;
    std::vector<bool> increasables;
    bool proven_optimal;
};
Incomparables list_incomparables_albp(
    const ALBP& albp,
    const std::vector<std::vector<int>>& banned_edges = {},
    const std::vector<int>& prev_sol = {},
    const std::vector<double>& heads = {},
    const std::vector<double>& tails = {},
    int ub = 0
);


#endif //INCOMPARABLES_H
