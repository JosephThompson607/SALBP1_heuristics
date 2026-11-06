//
// Created by Joseph Thompson on 2026-10-26.
//

#ifndef INCOMPARABLES_H
#define INCOMPARABLES_H
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
namespace py = pybind11;


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
    std::vector<std::pair<int, int>> valid_edges;
    std::vector<std::pair<int, int>> deduced_edges;
    std::vector<bool> increasables;
    bool proven_optimal;
};
Incomparables list_incomparables_albp(
    const ALBP& albp,
    const std::vector<std::pair<int,int>>& banned_edges = {},
    const std::vector<int>& prev_sol = {},
    const std::vector<double>& heads = {},
    const std::vector<double>& tails = {},
    int ub = 0
);
Incomparables filter_incomparables_albp(
    const ALBP& albp,
    const std::vector<std::pair<int, int>> &candidate_edges,
    // 1-based {a, b}: a before b
    const std::vector<std::pair<int, int>> &banned_edges = {},
    // 0-based
    const std::vector<int>& prev_sol = {},
    const std::vector<double>& heads={},
    const std::vector<double>& tails={},
    int ub = 0
);
std::pair<std::vector<std::string>, py::array_t<double>>
fast_edge_stats(const ALBP& albp,
                const std::vector<std::vector<int>>& valid_edges,
                const std::vector<double>& pw,
                const std::vector<double>& rpw,
                const std::vector<float>& heads,
                const std::vector<float>& tails,
                int ub,
                int lb,
                const std::vector<int>& task_assignment);

#endif //INCOMPARABLES_H
