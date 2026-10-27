//
// Created by Joseph Thompson on 2026-10-26.
//

#include "incomparables.h"
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
    const std::vector<std::vector<int>>& banned_edges,
    const std::vector<int>& prev_sol,
    const std::vector<double>& heads ,
    const std::vector<double>& tails ,
    const int ub
) {
    int n = albp.N;

    bool use_bound = !heads.empty() && !tails.empty() && ub != 0;

    std::vector<std::vector<int>> valid_edges;
    std::vector<std::vector<int>> deduced_edges;
    std::vector<bool> increasables(n, true);

    bool proven_optimal = false;

    auto is_banned = [&](int i, int j) {
        return std::find(
            banned_edges.begin(),
            banned_edges.end(),
            std::vector<int>{i, j}
        ) != banned_edges.end();
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