//
// Created by Joseph Thompson on 2026-10-26.
//

#include "incomparables.h"
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <utility>

namespace py = pybind11;
void check_t_time_increase(
    const ALBP& albp,
    int i,
    int j,
    std::vector<bool>& increasables,
    const std::vector<double>* heads,
    const std::vector<double>* tails,
    int ub
) {
    if (!increasables[i] && !increasables[j])
        return;

    if (
        (!albp.is_pred(i, j) && !albp.is_pred(j, i)) ||
        albp.is_dir_pred(i, j) ||
        albp.is_dir_pred(j, i)
    ) {
        if (albp.get_tt(i) + albp.get_tt(j) > albp.C)
            return;

        if (heads && tails && ub) {
            double p_i = static_cast<double>(albp.get_tt(i)) / albp.C;
            double p_j = static_cast<double>(albp.get_tt(j)) / albp.C;

            double latest_i = ub - std::ceil((*tails)[i] + p_i);
            double latest_j = ub - std::ceil((*tails)[j] + p_j);
            double earliest_i = std::ceil((*heads)[i] + p_i);
            double earliest_j = std::ceil((*heads)[j] + p_j);

            if (latest_i < earliest_j || latest_j < earliest_i)
                return;
        }

        increasables[i] = false;
        increasables[j] = false;
    }
}

bool chain_is_dead(
    const ALBP& albp,
    int ub,
    const std::vector<double>& heads,
    const std::vector<double>& tails,
    int first,
    int second
) {
    constexpr double EPS = 1e-9;

    double p = static_cast<double>(
        albp.get_tt(first) + albp.get_tt(second)
    ) / albp.C;

    double total = heads[first] + p + tails[second];

    return std::ceil(total - EPS) >= ub;
}

Incomparables list_incomparables_albp(
    const ALBP& albp,
    const std::vector<std::pair<int, int>>& banned_edges,
    const std::vector<int>& prev_sol,
    const std::vector<double>& heads ,
    const std::vector<double>& tails ,
    const int ub
) {
    int n = albp.N;

    bool use_bound = !heads.empty() && !tails.empty() && ub != 0;

    std::vector<std::pair<int,int>> valid_edges;
    std::vector<std::pair<int,int>> deduced_edges;
    std::vector<bool> increasables(n, true);

    bool proven_optimal = false;

    std::vector<char> banned(static_cast<size_t>(n) * n, 0);
    for (const auto& e : banned_edges) {
        if (e.first >= 0 && e.first < n && e.second >= 0 && e.second < n)
            banned[static_cast<size_t>(e.first) * n + e.second] = 1;
    }
    auto is_banned = [&](int i, int j) {
        return banned[static_cast<size_t>(i) * n + j] != 0;
    };

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {

            check_t_time_increase(
                albp, i, j, increasables,
                use_bound ? &heads : nullptr,
                use_bound ? &tails : nullptr,
                ub
            );

            if (albp.is_pred(i, j) || albp.is_pred(j, i))
                continue;

            if (use_bound) {
                bool ij_dead = chain_is_dead(albp, ub, heads, tails, i, j);
                bool ji_dead = chain_is_dead(albp, ub, heads, tails, j, i);

                if (ij_dead && ji_dead) {
                    proven_optimal = true;
                    continue;
                }

                if (ji_dead) {
                    deduced_edges.push_back({i + 1, j + 1});
                    continue;
                }

                if (ij_dead) {
                    deduced_edges.push_back({j + 1, i + 1});
                    continue;
                }
            }

            if (
                (prev_sol.empty() || prev_sol[i] > prev_sol[j]) && !is_banned(i, j) ){
                valid_edges.push_back({i + 1, j + 1});
            }

            if (
                (prev_sol.empty() || prev_sol[j] > prev_sol[i]) && !is_banned(j, i)) {
                valid_edges.push_back({j + 1, i + 1});
            }
        }
    }

    return {
        valid_edges,
        deduced_edges,
        increasables,
        proven_optimal
    };
}

Incomparables filter_incomparables_albp(
    const ALBP& albp,
    const std::vector<std::pair<int, int>> &candidate_edges,
    // 1-based {a, b}: a before b
    const std::vector<std::pair<int, int>> &banned_edges,
    // 0-based
    const std::vector<int>& prev_sol,
    const std::vector<double>& heads,
    const std::vector<double>& tails,
    const int ub
) {
    const int n = albp.N;
    const bool use_bound = !heads.empty() && !tails.empty() && ub != 0;

    std::vector<std::pair<int,int>> valid_edges;
    std::vector<std::pair<int,int>> deduced_edges;
    std::vector<bool> increasables(n, true);
    bool proven_optimal = false;

    valid_edges.reserve(candidate_edges.size());

    std::vector<char> banned(static_cast<size_t>(n) * n, 0);
    for (const auto& e : banned_edges) {
        if (e.first>= 0 && e.first < n && e.second >= 0 && e.second < n)
            banned[static_cast<size_t>(e.first) * n + e.second] = 1;
    }
    auto is_banned = [&](int a, int b) {
        return banned[static_cast<size_t>(a) * n + b] != 0;
    };
    auto try_emit = [&](int x, int y) {
        if ((prev_sol.empty() || prev_sol[x] > prev_sol[y]) && !is_banned(x, y)) {
                valid_edges.push_back({x + 1, y + 1});
        }
    };

    for (const auto& e : candidate_edges) {
        const int a = e.first - 1;
        const int b = e.second - 1;
        if (a < 0 || a >= n || b < 0 || b >= n || a == b) continue;

        if (albp.is_pred(a, b) || albp.is_pred(b, a))
            continue;

        if (use_bound) {
            const bool ab_dead = chain_is_dead(albp, ub, heads, tails, a, b);
            const bool ba_dead = chain_is_dead(albp, ub, heads, tails, b, a);

            if (ab_dead && ba_dead) {
                proven_optimal = true;
                continue;
            }

            if (ab_dead || ba_dead) {
                const int first  = ba_dead ? a : b;
                const int second = ba_dead ? b : a;
                deduced_edges.push_back({first + 1, second + 1});
                continue;
            }
        }

        try_emit (a, b);
        try_emit (b, a);
    }

        for (int task = 0; task < n; ++task) {
            for (const int pred : albp.dir_pred[task]) {
                check_t_time_increase(
                    albp, task, pred, increasables,
                    use_bound ? &heads : nullptr,
                    use_bound ? &tails : nullptr,
                    ub
                );
            }
        }


    return { valid_edges, deduced_edges, increasables, proven_optimal };
}

std::pair<std::vector<std::string>, py::array_t<double>> fast_edge_stats(const ALBP& albp,
                                                                        const std::vector<std::vector<int>>& valid_edges,
                                                                        const std::vector<double>& pw,
                                                                        const std::vector<double>& rpw,
                                                                        const std::vector<float>& heads,
                                                                        const std::vector<float>& tails,
                                                                        int ub,
                                                                        int lb,
                                                                        const std::vector<int>& task_assignment)
                                                        {
    // Doubles so the divisions below are not integer divisions
    const double C = albp.C;
    const double N = albp.N;
    const int n_tasks = static_cast<int>(albp.task_time.size());

    // ---- Graph-level stats ----
    double max_task_time = 0.0;
    double min_task_time = C;
    long long total_predecessors = 0, total_dir_pred = 0;
    int n_no_predecessors = 0, n_no_successors = 0;
    std::vector<double> station_loads(ub, 0.0);
    for (int task = 0; task < n_tasks; ++task) {
        const double t_time = albp.task_time[task];

        max_task_time = std::max(max_task_time, t_time);
        min_task_time = std::min(min_task_time, t_time);

        const size_t n_pred = albp.pred[task].size();
        total_predecessors += n_pred;
        total_dir_pred     += albp.dir_pred[task].size();
        if (n_pred == 0)                   ++n_no_predecessors;
        if (albp.suc[task].size() == 0)    ++n_no_successors;

        station_loads[task_assignment[task]] += t_time;
    }
    const double OS  = 2.0 * total_predecessors / (N * (N - 1.0));
    const double AIP = total_dir_pred / N;

    const double dod_den = total_dir_pred + n_no_predecessors - 1.0;
    const double DOD = (dod_den == 0.0) ? 0.0
                     : 1.0 - (total_dir_pred + n_no_predecessors - N) / dod_den;

    const double doc_den = total_dir_pred + n_no_successors - 1.0;
    const double DOC = (doc_den == 0.0) ? 0.0
                     : 1.0 - (total_dir_pred + n_no_successors - N) / doc_den;

    const double gap         = (static_cast<double>(ub) - lb) / (lb + 0.00001);
    const double max_div_c   = max_task_time / C;
    const double min_div_c   = min_task_time / C;
    const double max_div_min = max_task_time / (min_task_time + 0.001);

    // ---- Column names (index in this list = column index used below) ----
    std::vector<std::string> names = {
        /*  0 */ "par_pw_child_rpw_div_c",
        /*  1 */ "par_rpw_child_pw_div_c",
        /*  2 */ "par_t_chi_h_div_lb",
        /*  3 */ "chi_t_par_h_div_lb",
        /*  4 */ "pop_linkage",
        /*  5 */ "solution_distance",
        /*  6 */ "par_n_dir_sucs",
        /*  7 */ "par_n_dir_preds",
        /*  8 */ "par_degree",
        /*  9 */ "par_share_descendants",
        /* 10 */ "par_share_ancestors",
        /* 11 */ "par_load_div_c",
        /* 12 */ "chi_n_dir_sucs",
        /* 13 */ "chi_n_dir_preds",
        /* 14 */ "chi_degree",
        /* 15 */ "chi_share_descendants",
        /* 16 */ "chi_share_ancestors",
        /* 17 */ "chi_load_div_c",
        /* 18 */ "max_div_c",
        /* 19 */ "min_div_c",
        /* 20 */ "max_div_min",
        /* 21 */ "OS",
        /* 22 */ "AIP",
        /* 23 */ "DOD",
        /* 24 */ "DOC",
        /* 25 */ "gap",
        /* 26 */ "par_t_time_div_c",
        /* 27 */ "chi_t_time_div_c",
        /* 28 */ "sum_t_time_div_c",
        /* 29 */ "parent",
        /* 30 */ "child"
    };
    const size_t n_cols  = names.size();
    const size_t n_edges = valid_edges.size();

    py::array_t<double> features({n_edges, n_cols});
    auto X = features.mutable_unchecked<2>();

    // ---- Edge-level stats ----
    for (size_t i = 0; i < n_edges; ++i) {
        const int par = valid_edges[i][0]-1;
        const int chi = valid_edges[i][1]-1;

        const double par_n_anc = albp.pred[par].size();
        const double par_n_des = albp.suc[par].size();
        const double chi_n_anc = albp.pred[chi].size();
        const double chi_n_des = albp.suc[chi].size();
        const double par_t_time_div_c = albp.task_time[par] / C;
        const double chi_t_time_div_c = albp.task_time[chi] / C;
        const int par_out = static_cast<int>(albp.dir_suc[par].size());
        const int par_in  = static_cast<int>(albp.dir_pred[par].size());
        const int chi_out = static_cast<int>(albp.dir_suc[chi].size());
        const int chi_in  = static_cast<int>(albp.dir_pred[chi].size());

        X(i, 0)  = (pw[par] + rpw[chi]) / C;
        X(i, 1)  = (rpw[par] + pw[chi]) / C;
        X(i, 2)  = (heads[chi] + tails[par]) / static_cast<float>(lb);
        X(i, 3)  = (heads[par] + tails[chi]) / static_cast<float>(lb);
        X(i, 4)  = (par_n_anc + chi_n_des) / N;
        X(i, 5)  = std::abs(task_assignment[par] - task_assignment[chi]) / static_cast<double>(ub);

        X(i, 6)  = par_out;
        X(i, 7)  = par_in;
        X(i, 8)  = par_out + par_in;
        X(i, 9)  = par_n_des / N;
        X(i, 10) = par_n_anc / N;
        X(i, 11) = station_loads[task_assignment[par]] / C;

        X(i, 12) = chi_out;
        X(i, 13) = chi_in;
        X(i, 14) = chi_out + chi_in;
        X(i, 15) = chi_n_des / N;
        X(i, 16) = chi_n_anc / N;
        X(i, 17) = station_loads[task_assignment[chi]] / C;

        X(i, 18) = max_div_c;
        X(i, 19) = min_div_c;
        X(i, 20) = max_div_min;
        X(i, 21) = OS;
        X(i, 22) = AIP;
        X(i, 23) = DOD;
        X(i, 24) = DOC;
        X(i, 25) = gap;
        X(i, 26) = par_t_time_div_c;
        X(i, 27) = chi_t_time_div_c;
        X(i, 28) = par_t_time_div_c + chi_t_time_div_c;
        X(i, 29) = par;
        X(i, 30) = chi;
    }

    return {std::move(names), std::move(features)};
}
