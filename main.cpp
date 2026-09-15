#include <assert.h>

#include "ALBP.h"
#include "albp_solution.h"
#include "ils.h"
#include "vdls.h"
#include <iostream>
#include <filesystem>
#include "Hoff.h"
#include "MultiHoff.h"
#include "salbp_basics.h"
#include "tabu.h"
#include <chrono>
#include <RepairHoff.h>

int default_run() {
    ALBP problem;
    namespace fs = std::filesystem;

    if (!problem.loadFromFile("/Users/letshopethisworks2/Documents/phd_paper_material/MMABPWW/SALBP_benchmark/medium data set_n=50/instance_n=50_108.alb")) {
        return 1;
    }
    std::cout << "Loaded " << problem.N << " tasks, cycle time " << problem.C << "\n";

    //runs the local search algorithm
    ALBPSolution result =iterated_local_search(problem, 50000,10, 0.5);
    std::cout << "Here is the result" << std::endl;
    result.print();
    return 0;
}

int python_constructor_test() {
    int C = 10;
    int N = 5;
    std::vector<int> task_times = {1, 2, 3, 4, 5};

    // Precedence constraints: each pair is (pred, succ), using 1-based indexing
    std::vector<std::vector<int>> precedence = {
        {1, 2},
        {1, 3},
        {2, 4},
        {3, 5}
    };
    std::vector<int> test_assignments = {0,1,2,3,4};
    ALBP albp = ALBP::type_1(C, N, task_times, precedence);
    ALBPSolution result =  ils_solve_SALBP1(C, N, task_times, precedence, 10, 30,0.5, true, test_assignments);
    std::cout << "Here is the result" << std::endl;
    result.print();
    // std::cout << "Name: " << albp.name << std::endl;
    // std::cout << "Cycle time: " << albp.C << std::endl;
    // std::cout << "Number of tasks: " << albp.N << std::endl;
    //
    // std::cout << "Precedence matrix:" << std::endl;
    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < N; ++j) {
    //         std::cout << albp.prec_mat[i * N + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    //
    // std::cout << "Precedence relations:" << std::endl;
    // for (const auto& rel : albp.precedence_relations) {
    //     std::cout << rel.parent << " -> " << rel.child << std::endl;
    // } n_attempts_++;
    //
    return 0;
}
int tabu_test() {
    //  int C = 10;
    //  int N = 5;
    //  std::vector<int> task_times = {1, 2, 3, 4, 5};
    //
    //  std::vector<std::vector<int>> precedence = {
    //      {1, 2},
    //      {1, 3},
    //      {2, 4},
    //      {3, 5}
    // };
    // int C = 16;
    // int N = 17;
    // std::vector<int> task_times = {6, 3, 3, 11, 11,11,11,3,3,4,4,4,13,4,4,4,6};
    //
    // //Precedence constraints: each pair is (pred, succ), using 1-based indexing
    // std::vector<std::vector<int>> precedence = {
    //     {1, 2},
    //     {1, 3},
    //     {1, 10},
    //     {2, 4},
    //         {3,5},
    //     {4,6},
    //     {5,7},
    //     {6,8},
    //     {7,9},
    //     {8,17},
    //     {9,17},
    //     {10,11},
    //     {10,12},
    //     {11,13},
    //     {12,13},
    //     {13,14},
    //     {13,15},
    //     {14,16},
    //     {15,16},
    //     {16,17},
    // };
    std::vector<int> task_times = {
        141, 137, 51, 439, 125, 330, 255, 62, 33, 490,
        58, 91, 115, 211, 392, 158, 537, 66, 345, 563,
        211, 466, 215, 228, 568, 477, 88, 41, 482, 92,
        136, 174, 523, 125, 52, 26, 516, 533, 123, 617,
        503, 263, 528, 106, 172, 110, 39, 108, 76, 323
    };
    std::vector<std::vector<int>> precedence = {
        {1,4},{2,5},{2,8},{2,9},{2,10},{3,6},{3,7},{3,9},{3,11},{4,12},{5,13},{6,14},
        {8,16},{8,18},{8,28},{9,15},{10,17},{12,20},{13,21},{14,19},{15,22},{18,23},
        {19,24},{20,28},{21,26},{22,25},{22,27},{22,33},{24,31},{25,32},{26,29},{26,30},
        {26,33},{27,34},{29,35},{30,36},{31,39},{32,37},{33,38},{33,40},{33,41},{33,44},
        {34,42},{34,43},{35,48},{36,48},{37,45},{38,46},{39,48},{40,47},{41,49},{42,50}
    };
    int C =1000;
    int N = 50;
    // ALBP albp;
    // std::string filepath  = "/Users/letshopethisworks2/CLionProjects/SALBP_ILS/n_20_16.alb";
    // std::cout << "Loading ALBP file: " << filepath << std::endl;
    //
    // if (!albp.loadFromFile(filepath)) {
    //     std::cerr << "Error: Failed to load file '" << filepath << "'" << std::endl;
    //     return 1;
    // }
    // int N = 20;
    // int C = 1000;
    //
    // std::vector<int> task_times = {
    //     599, 464, 579, 489, 624, 430, 411, 524, 478, 585,
    //     570, 622, 452, 344, 720, 502, 355, 813, 342, 473
    // };
    //
    // std::vector<std::vector<int>> precedence = {
    //     {1, 5},
    //     {2, 7},
    //     {3, 8},
    //     {4, 6},
    //     {4, 9},
    //     {4, 10},
    //     {5, 11},
    //     {6, 12},
    //     {7, 13},
    //     {8, 13},
    //     {11, 15},
    //     {12, 14},
    //     {13, 16},
    //     {13, 17},
    //     {14, 18},
    //     {15, 19},
    //     {16, 20}
    // };

    //std::vector<int> test_assignments = {0,1,2,3,4};
    ALBP albp = ALBP::type_1(C, N, task_times, precedence);

    ALBPSolution result =  tabu_solve_salbp1(albp, 7);
     std::cout << "Here is the result" << std::endl;
    result.print();
    // std::cout << "Name: " << albp.name << std::endl;
    // std::cout << "Cycle time: " << albp.C << std::endl;
    // std::cout << "Number of tasks: " << albp.N << std::endl;
    //
    // std::cout << "Precedence matrix:" << std::endl;
    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < N; ++j) {
    //         std::cout << albp.prec_mat[i * N + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    //
    // std::cout << "Precedence relations:" << std::endl;
    // for (const auto& rel : albp.precedence_relations) {
    //     std::cout << rel.parent << " -> " << rel.child << std::endl;
    // }
    //
    return 0;
}

int mhh_test() {
    // int C = 10;
    // int N = 5;
    // std::vector<int> task_times = {1, 2, 3, 4, 5};
    //
    // // Precedence constraints: each pair is (pred, succ), using 1-based indexing
    // std::vector<std::vector<int>> precedence = {
    //     {1, 2},
    //     {1, 3},
    //     {2, 4},
    //     {3, 5}
    //};
    // int C = 16;
    // int N = 17;
    // std::vector<int> task_times = {6, 3, 3, 11, 11,11,11,3,3,4,4,4,13,4,4,4,6};

    // Precedence constraints: each pair is (pred, succ), using 1-based indexing
    // std::vector<std::vector<int>> precedence = {
    //     {1, 2},
    //     {1, 3},
    //     {1, 10},
    //     {2, 4},
    //         {3,5},
    //     {4,6},
    //     {5,7},
    //     {6,8},
    //     {7,9},
    //     {8,17},
    //     {9,17},
    //     {10,11},
    //     {10,12},
    //     {11,13},
    //     {12,13},
    //     {13,14},
    //     {13,15},
    //     {14,16},
    //     {15,16},
    //     {16,17},
    // };
    std::vector<int> task_times = {
        141, 137, 51, 439, 125, 330, 255, 62, 33, 490,
        58, 91, 115, 211, 392, 158, 537, 66, 345, 563,
        211, 466, 215, 228, 568, 477, 88, 41, 482, 92,
        136, 174, 523, 125, 52, 26, 516, 533, 123, 617,
        503, 263, 528, 106, 172, 110, 39, 108, 76, 323
    };
    std::vector<std::vector<int>> precedence = {
        {1,4},{2,5},{2,8},{2,9},{2,10},{3,6},{3,7},{3,9},{3,11},{4,12},{5,13},{6,14},
        {8,16},{8,18},{8,28},{9,15},{10,17},{12,20},{13,21},{14,19},{15,22},{18,23},
        {19,24},{20,28},{21,26},{22,25},{22,27},{22,33},{24,31},{25,32},{26,29},{26,30},
        {26,33},{27,34},{29,35},{30,36},{31,39},{32,37},{33,38},{33,40},{33,41},{33,44},
        {34,42},{34,43},{35,48},{36,48},{37,45},{38,46},{39,48},{40,47},{41,49},{42,50}
    };
    int C =1000;
    int N = 50;

    ALBP albp = ALBP::type_1(C, N, task_times, precedence);
    std::vector<float> alpha = {0.2};
    std::vector<float> beta = {0.2};
    ALBPSolution result =  mhh_solve_salbp1(albp, alpha, beta);
     std::cout << "Here is the result" << std::endl;
    result.print();

    return 0;
}
int rmhh_test() {
    // int C = 10;
    // int N = 5;
    // std::vector<int> task_times = {1, 2, 3, 4, 5};
    //
    // // Precedence constraints: each pair is (pred, succ), using 1-based indexing
    // std::vector<std::vector<int>> precedence = {
    //     {1, 2},
    //     {1, 3},
    //     {2, 4},
    //     {3, 5}
    //};
    // int C = 16;
    // int N = 17;
    // std::vector<int> task_times = {6, 3, 3, 11, 11,11,11,3,3,4,4,4,13,4,4,4,6};

    // Precedence constraints: each pair is (pred, succ), using 1-based indexing
    // std::vector<std::vector<int>> precedence = {
    //     {1, 2},
    //     {1, 3},
    //     {1, 10},
    //     {2, 4},
    //         {3,5},
    //     {4,6},
    //     {5,7},
    //     {6,8},
    //     {7,9},
    //     {8,17},
    //     {9,17},
    //     {10,11},
    //     {10,12},
    //     {11,13},
    //     {12,13},
    //     {13,14},
    //     {13,15},
    //     {14,16},
    //     {15,16},
    //     {16,17},
    // };
    // std::vector<int> task_times = {
    //     141, 137, 51, 439, 125, 330, 255, 62, 33, 490,
    //     58, 91, 115, 211, 392, 158, 537, 66, 345, 563,
    //     211, 466, 215, 228, 568, 477, 88, 41, 482, 92,
    //     136, 174, 523, 125, 52, 26, 516, 533, 123, 617,
    //     503, 263, 528, 106, 172, 110, 39, 108, 76, 323
    // };
    // std::vector<std::vector<int>> precedence = {
    //     {1,4},{2,5},{2,8},{2,9},{2,10},{3,6},{3,7},{3,9},{3,11},{4,12},{5,13},{6,14},
    //     {8,16},{8,18},{8,28},{9,15},{10,17},{12,20},{13,21},{14,19},{15,22},{18,23},
    //     {19,24},{20,28},{21,26},{22,25},{22,27},{22,33},{24,31},{25,32},{26,29},{26,30},
    //     {26,33},{27,34},{29,35},{30,36},{31,39},{32,37},{33,38},{33,40},{33,41},{33,44},
    //     {34,42},{34,43},{35,48},{36,48},{37,45},{38,46},{39,48},{40,47},{41,49},{42,50}
    // };
    // int C =1000;
    // int N = 50;
    std::vector<int> task_times = {
    576, 613, 490, 551, 612, 464, 277, 579, 299, 450,
    658, 399, 554, 492, 302, 473, 584, 775, 322, 497,
    347, 545, 701, 517, 352, 627, 570, 579, 306, 451,
    576, 649, 645, 480, 503, 605, 641, 644, 625, 754,
    600, 486, 560, 482, 560, 527, 387, 645, 580, 624,
    508, 472, 539, 352, 415, 361, 463, 560, 407, 511,
    763, 223, 395, 601, 578, 460, 403, 658, 564, 490,
    489, 650, 422, 430, 455, 618, 492, 508, 683, 422,
    564, 527, 555, 559, 578, 396, 553, 667, 584, 528,
    403, 435, 534, 550, 442, 508, 339, 532, 464, 512
};
std::vector<std::vector<int>> precedence = {
    {0,4},{0,5},{0,6},{0,7},{1,4},{1,5},{1,6},{1,7},{2,4},{2,5},{2,6},{2,7},
    {3,4},{3,5},{3,6},{3,7},{4,8},{4,9},{4,10},{4,11},{5,8},{5,9},{5,10},{5,11},
    {6,8},{6,9},{6,10},{6,11},{7,8},{7,9},{7,10},{7,11},{8,12},{8,13},{9,12},{9,13},
    {10,12},{10,13},{11,12},{11,13},{12,14},{12,15},{12,16},{12,17},{12,18},
    {13,14},{13,15},{13,16},{13,17},{13,18},{14,19},{15,19},{16,19},{17,19},{18,19},
    {19,20},{19,21},{19,22},{19,23},{20,24},{20,25},{20,26},{20,27},{20,28},
    {21,24},{21,25},{21,26},{21,27},{21,28},{22,24},{22,25},{22,26},{22,27},{22,28},
    {23,24},{23,25},{23,26},{23,27},{23,28},{24,29},{24,30},{24,31},{25,29},{25,30},
    {25,31},{26,29},{26,30},{26,31},{27,29},{27,30},{27,31},{28,29},{28,30},{29,57},
    {30,32},{30,33},{30,34},{31,32},{31,33},{31,34},{32,35},{32,36},{32,37},
    {33,35},{33,36},{33,37},{34,35},{34,36},{34,37},{35,38},{35,39},{35,40},{35,41},
    {36,38},{36,39},{36,40},{36,41},{36,42},{37,38},{37,39},{37,40},{37,41},
    {38,43},{38,44},{39,44},{40,44},{41,45},{42,43},{43,46},{43,49},{44,48},{44,51},
    {45,46},{45,47},{45,48},{46,50},{46,51},{47,49},{48,49},{48,50},{49,60},{49,61},
    {50,52},{50,53},{50,54},{51,52},{51,53},{52,58},{52,59},{53,56},{54,55},
    {55,57},{55,58},{55,59},{56,57},{56,58},{56,59},{57,60},{57,61},{58,60},{58,61},
    {58,62},{59,60},{59,61},{59,62},{60,64},{60,68},{61,63},{61,64},{61,65},
    {62,73},{62,84},{63,67},{64,66},{65,66},{66,69},{66,70},{66,80},{67,68},{67,73},
    {67,77},{68,72},{68,74},{69,73},{69,83},{69,88},{70,71},{70,75},{71,84},{71,92},
    {71,95},{72,76},{72,78},{72,79},{73,76},{73,91},{73,96},{74,79},{75,76},{75,77},
    {75,78},{79,80},{79,81},{80,82},{80,83},{80,84},{80,85},{81,84},{81,98},{82,87},
    {82,89},{82,97},{83,86},{83,96},{84,87},{84,88},{85,86},{85,88},{86,92},{86,99},
    {87,94},{87,96},{88,90},{88,91},{88,93},{89,95},{89,99},{90,97},{92,97},{92,98},
    {93,99},{95,96},{29,55}
    };
    int C = 1000;
    int N = 100;

    ALBP albp = ALBP::type_1(C, N, task_times, precedence, false, false, false, false);
    std::vector<float> alpha = {0.2};
    std::vector<float> beta = {0.2};
    ALBPSolution result =  mhh_solve_salbp1(albp, alpha, beta);
    std::cout << "Hoff result" << std::endl;
    result.print();
    //Assuming that MHH is functioning deterministically
    // std::vector<int> new_prec = {14,18};
    std::vector<int> new_prec = {30, 56};
    albp.add_precedence_relation(new_prec);
    ALBPSolution res2 = rep_mhh_salbp1(albp, result.station_assignments, {new_prec}, "left", 5000, std::nullopt, std::nullopt, 0.3);
    std::cout << "Left result" << std::endl;
    res2.print();
    ALBPSolution res3 = rep_mhh_salbp1(albp, result.station_assignments, {new_prec}, "center");
    std::cout << "Center result" << std::endl;
    res3.print();
    ALBPSolution res4 = rep_mhh_salbp1(albp, result.station_assignments, {new_prec}, "right");
    std::cout << "Right result" << std::endl;
    res4.print();

    return 0;
}

int vdls_salbp_1_test() {
    // int C = 20;
    // int N = 8;
    // std::vector<int> task_times = {11, 17, 9, 5, 8, 12, 10, 3};
    //
    // // Precedence constraints: each pair is (pred, succ), using 1-based indexing
    // std::vector<std::vector<int>> precedence = {
    //     {1, 2},
    //     {2, 3},
    //     {2, 4},
    //     {3, 5},
    //     {3, 6},
    //     {4, 6},
    //     {5, 7},
    //     {6, 8}
    // };
    int N = 20;
    int C = 1000;

    std::vector<int> task_times = {
        599, 464, 579, 489, 624, 430, 411, 524, 478, 585,
        570, 622, 452, 344, 720, 502, 355, 813, 342, 473
    };

    std::vector<std::vector<int>> precedence = {
        {1, 5},
        {2, 7},
        {3, 8},
        {4, 6},
        {4, 9},
        {4, 10},
        {5, 11},
        {6, 12},
        {7, 13},
        {8, 13},
        {11, 15},
        {12, 14},
        {13, 16},
        {13, 17},
        {14, 18},
        {15, 19},
        {16, 20}
    };
    // std::vector<int> task_times = {
    //     141, 137, 51, 439, 125, 330, 255, 62, 33, 490,
    //     58, 91, 115, 211, 392, 158, 537, 66, 345, 563,
    //     211, 466, 215, 228, 568, 477, 88, 41, 482, 92,
    //     136, 174, 523, 125, 52, 26, 516, 533, 123, 617,
    //     503, 263, 528, 106, 172, 110, 39, 108, 76, 323
    // };
    // std::vector<std::vector<int>> precedence = {
    //     {1,4},{2,5},{2,8},{2,9},{2,10},{3,6},{3,7},{3,9},{3,11},{4,12},{5,13},{6,14},
    //     {8,16},{8,18},{8,28},{9,15},{10,17},{12,20},{13,21},{14,19},{15,22},{18,23},
    //     {19,24},{20,28},{21,26},{22,25},{22,27},{22,33},{24,31},{25,32},{26,29},{26,30},
    //     {26,33},{27,34},{29,35},{30,36},{31,39},{32,37},{33,38},{33,40},{33,41},{33,44},
    //     {34,42},{34,43},{35,48},{36,48},{37,45},{38,46},{39,48},{40,47},{41,49},{42,50}
    // };
    // int C =1000;
    // int N = 50;

    //std::vector<int> test_assignments = {0,1,2,3,4};
    ALBP albp = ALBP::type_1(C, N, task_times, precedence);
    ALBPSolution result =  vdls_solve_salbp1(C, N, task_times, precedence);
    std::cout << "Here is the result" << std::endl;
    result.print();
    // std::cout << "Name: " << albp.name << std::endl;
    // std::cout << "Cycle time: " << albp.C << std::endl;
    // std::cout << "Number of tasks: " << albp.N << std::endl;
    //
    // std::cout << "Precedence matrix:" << std::endl;
    // for (int i = 0; i < N; ++i) {
    //     for (int j = 0; j < N; ++j) {
    //         std::cout << albp.prec_mat[i * N + j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
    //
    // std::cout << "Precedence relations:" << std::endl;
    // for (const auto& rel : albp.precedence_relations) {
    //     std::cout << rel.parent << " -> " << rel.child << std::endl;
    // }
    //
    return 0;
}

int priority_methods_salbp_1_test() {
    std::vector<int> task_times = {
        141, 137, 51, 439, 125, 330, 255, 62, 33, 490,
        58, 91, 115, 211, 392, 158, 537, 66, 345, 563,
        211, 466, 215, 228, 568, 477, 88, 41, 482, 92,
        136, 174, 523, 125, 52, 26, 516, 533, 123, 617,
        503, 263, 528, 106, 172, 110, 39, 108, 76, 323
    };
    std::vector<std::vector<int>> precedence = {
        {1,4},{2,5},{2,8},{2,9},{2,10},{3,6},{3,7},{3,9},{3,11},{4,12},{5,13},{6,14},
        {8,16},{8,18},{8,28},{9,15},{10,17},{12,20},{13,21},{14,19},{15,22},{18,23},
        {19,24},{20,28},{21,26},{22,25},{22,27},{22,33},{24,31},{25,32},{26,29},{26,30},
        {26,33},{27,34},{29,35},{30,36},{31,39},{32,37},{33,38},{33,40},{33,41},{33,44},
        {34,42},{34,43},{35,48},{36,48},{37,45},{38,46},{39,48},{40,47},{41,49},{42,50}
    };
    int C =1000;
    int N = 50;
    //std::vector<int> test_assignments = {0,1,2,3,4};
    ALBP albp = ALBP::type_1(C, N, task_times, precedence);
    std::vector<ALBPSolution> results =  priority_solve_salbp_1(C, N, task_times, precedence, 100, 42, 100);
    for (int i = 0; i < results.size(); ++i) {
        ALBPSolution result = results[i];
        std::cout << "Here is the result" << std::endl;
        result.print();
    }
    //testing with time limit
    results =  priority_solve_salbp_1(C, N, task_times, precedence, 1000000000, 42, 1);
    for (int i = 0; i < results.size(); ++i) {
        ALBPSolution result = results[i];
        std::cout << "Here is the result" << std::endl;
        result.print();
    }

    return 0;
}
int priority_methods_salbp_2_test() {
    int S = 4;
    int N = 8;
    std::vector<int> task_times = {11, 17, 9, 5, 8, 12, 10, 3};

    // Precedence constraints: each pair is (pred, succ), using 1-based indexing
    std::vector<std::vector<int>> precedence = {
        {1, 2},
        {2, 3},
        {2, 4},
        {3, 5},
        {3, 6},
        {4, 6},
        {5, 7},
        {6, 8}
    };
    //std::vector<int> test_assignments = {0,1,2,3,4};

    std::vector<ALBPSolution> results =  priority_solve_salbp_2(S, N, task_times, precedence, 2);
    for (int i = 0; i < results.size(); ++i) {
        ALBPSolution result = results[i];
        std::cout << "Here is the result" << std::endl;
        result.print();
    }

    return 0;
}

int topo_sort_test() {
    std::vector<int> task_times = {
        141, 137, 51, 439, 125, 330, 255, 62, 33, 490,
        58, 91, 115, 211, 392, 158, 537, 66, 345, 563,
        211, 466, 215, 228, 568, 477, 88, 41, 482, 92,
        136, 174, 523, 125, 52, 26, 516, 533, 123, 617,
        503, 263, 528, 106, 172, 110, 39, 108, 76, 323
    };
    std::vector<std::vector<int>> precedence = {
        {1,4},{2,5},{2,8},{2,9},{2,10},{3,6},{3,7},{3,9},{3,11},{4,12},{5,13},{6,14},
        {8,16},{8,18},{8,28},{9,15},{10,17},{12,20},{13,21},{14,19},{15,22},{18,23},
        {19,24},{20,28},{21,26},{22,25},{22,27},{22,33},{24,31},{25,32},{26,29},{26,30},
        {26,33},{27,34},{29,35},{30,36},{31,39},{32,37},{33,38},{33,40},{33,41},{33,44},
        {34,42},{34,43},{35,48},{36,48},{37,45},{38,46},{39,48},{40,47},{41,49},{42,50}
    };
     int C =1000;
     int N = 50;
     // int C = 10;
     // int N = 10;
    //  std::vector<int> task_times = {1, 2, 3, 4, 5, 7,4, 3,2,1};
    //
    //  // Precedence constraints: each pair is (pred, succ), using 1-based indexing
    //  std::vector<std::vector<int>> precedence = {
    //      {1, 2},
    //      {1, 3},
    //      {2, 4},
    //      {3, 5},
    //         {1,9},
    //         {8,9},
    //         {7,8},
    //      {3,6},
    //      {3,7},
    //     {4, 10},
    //     {5, 10},
    //     {6, 10},
    //     {2, 7},
    //     {4, 9},
    // };
    // int C = 10;
    // int N = 15;
    // std::vector<int> task_times = {3, 5, 2, 7, 4, 6, 3, 8, 2, 5, 4, 6, 3, 7, 2};
    //
    // std::vector<std::vector<int>> precedence = {
    //     // roots: 1, 2
    //     {1, 3}, {1, 4},
    //     {2, 4}, {2, 5},
    //     // layer 2 -> 3
    //     {3, 6}, {3, 7},
    //     {4, 7}, {4, 8},
    //     {5, 8}, {5, 9},
    //     // layer 3 -> 4
    //     {6, 10}, {6, 11},
    //     {7, 11}, {7, 12},
    //     {8, 12}, {8, 13},
    //     {9, 13},
    //     // layer 4 -> sink
    //     {10, 14}, {11, 14},
    //     {12, 15}, {13, 15},
    //     {14, 15}
    // };
    //
    // int C = 10;
    // int N = 14;
    // std::vector<int> task_times = {3, 5, 2, 7, 4, 6, 3, 8, 2, 5, 1,3, 2,3};
    //
    // std::vector<std::vector<int>> precedence = {
    //     // roots: 1, 2
    //     {1, 3}, {1, 4},
    //     {2, 4}, {2, 5},
    //     // layer 2 -> 3
    //     {3, 6}, {3, 7},
    //     {4, 7}, {4, 8},
    //     {5, 8}, {5, 9},
    //     // // layer 3 -> 4
    //     {6, 10}, {6, 11},
    //      {7, 11}, {7, 12},
    //     {8, 12}, {8, 13},
    //     {9, 13},
    //     // // layer 4 -> sink
    //      {10, 14}, {11, 14},
    //     // {12, 15}, {13, 15},
    //     // {14, 15}
    // };

    ALBP albp = ALBP::type_1(C, N, task_times, precedence, false, true);
    //std::vector<int> test_assignments = {0,1,2,3,4};
    auto start = std::chrono::high_resolution_clock::now();
    albp.calc_trans_closure();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    std::cout << "calc_trans_closure: " << elapsed.count() << " ms\n";
    auto original_mat = albp.t_close_mat;
    start = std::chrono::high_resolution_clock::now();
    albp.calc_fast_trans_closure();
    end = std::chrono::high_resolution_clock::now();
    elapsed = end - start;
    std::cout << "calc_fast_trans_closure: " << elapsed.count() << " ms\n";
    auto fast_mat = albp.t_close_mat;
    if (original_mat == fast_mat) {
        std::cout << "Results match!\n";
    } else {
        if (original_mat == fast_mat) {
            std::cout << "Results match!\n";
        } else {
            std::cout << "Results differ!\n";
            for (int i = 0; i < albp.N; ++i) {
                std::cout << "row " << i + 1<< " orig: ";
                for (int j = 0; j < albp.N; ++j)
                    std::cout << original_mat[i * N + j] << " ";

                std::cout << "\nrow " << i + 1<< " fast: ";
                for (int j = 0; j < albp.N; ++j)
                    std::cout << fast_mat[i * N + j] << " ";

                std::cout << "\n\n";
            }
        }
    }
    std::vector<int> results = get_topo_sort(albp.dir_pred, albp.dir_suc);
    for (int i = 0; i < results.size(); ++i) {
        std::cout << "Here is the result" <<    results[i] << std::endl;
    }

    return 0;
}

int tails_test() {
    std::vector<int> task_times = {
        141, 137, 51, 439, 125, 330, 255, 62, 33, 490,
        58, 91, 115, 211, 392, 158, 537, 66, 345, 563,
        211, 466, 215, 228, 568, 477, 88, 41, 482, 92,
        136, 174, 523, 125, 52, 26, 516, 533, 123, 617,
        503, 263, 528, 106, 172, 110, 39, 108, 76, 323
    };
    std::vector<std::vector<int>> precedence = {
        {1,4},{2,5},{2,8},{2,9},{2,10},{3,6},{3,7},{3,9},{3,11},{4,12},{5,13},{6,14},
        {8,16},{8,18},{8,28},{9,15},{10,17},{12,20},{13,21},{14,19},{15,22},{18,23},
        {19,24},{20,28},{21,26},{22,25},{22,27},{22,33},{24,31},{25,32},{26,29},{26,30},
        {26,33},{27,34},{29,35},{30,36},{31,39},{32,37},{33,38},{33,40},{33,41},{33,44},
        {34,42},{34,43},{35,48},{36,48},{37,45},{38,46},{39,48},{40,47},{41,49},{42,50}
    };
    int C =1000;
    int N = 50;
    // int C = 1000;
    // int N = 20;
    // std::vector<int> task_times = {
    //     132, 120, 514, 190, 209, 457, 163, 491, 503, 138,
    //     138, 247, 230, 169, 29, 120, 247, 104, 286, 154
    // };
    //
    //
    // // Precedence constraints: each pair is (pred, succ), using 1-based indexing
    // std::vector<std::vector<int>> precedence = {
    //     {1, 9}, {2, 9}, {3, 9},
    //     {4, 5}, {4, 6}, {4, 7}, {4, 8},
    //     {5, 9}, {6, 9},
    //     {7, 10}, {7, 11}, {7, 16},
    //     {9, 12}, {9, 13}, {9, 14}, {9, 15},
    //     {10, 17}, {10, 18},
    //     {11, 18},
    //     {12, 16},
    //     {13, 16}, {13, 17},
    //     {16, 18},
    //     {17, 19}, {17, 20}
    // };
    int lb_1 = calc_lb_1(task_times, 1000);
    int lb_6 = calc_lb_6(task_times, 1000);
    //std::vector<int> test_assignments = {0,1,2,3,4};
    ALBP albp = ALBP::type_1(C, N, task_times, precedence);

    ALBPSolution result =  hoff_solve_salbp1(C, N, task_times, precedence);
    result.print();

    std::vector<float> tails;
    tails = get_tails(albp, false);
    std::cout << "lb_1: " << lb_1 << " lb_6: "<< lb_6<<std::endl;
    std::cout << "Here is the result" << std::endl;
    for (int i = 0; i < tails.size(); ++i) {
        std::cout << tails[i] << std::endl;
    }
    std::vector<float> heads = get_heads(albp, false);
    std::cout << "Here is the result" << std::endl;
    for (int i = 0; i < heads.size(); ++i) {
        std::cout << heads[i] << std::endl;
    }

    return 0;
}



int vdls_salbp_2_test() {
    int S = 10;
    int N = 20;
    std::vector<int> task_times = {
        132, 120, 514, 190, 209, 457, 163, 491, 503, 138,
        138, 247, 230, 169, 29, 120, 247, 104, 286, 154
    };


    // Precedence constraints: each pair is (pred, succ), using 1-based indexing
    std::vector<std::vector<int>> precedence = {
        {1, 9}, {2, 9}, {3, 9},
        {4, 5}, {4, 6}, {4, 7}, {4, 8},
        {5, 9}, {6, 9},
        {7, 10}, {7, 11}, {7, 16},
        {9, 12}, {9, 13}, {9, 14}, {9, 15},
        {10, 17}, {10, 18},
        {11, 18},
        {12, 16},
        {13, 16}, {13, 17},
        {16, 18},
        {17, 19}, {17, 20}
    };
    //std::vector<int> test_assignments = {0,1,2,3,4};
    ALBP albp = ALBP::type_2(S, N, task_times, precedence);
    ALBPSolution result =  vdls_solve_salbp2(S, N, task_times, precedence, {}, 1000, 2000);
    std::cout << "Here is the result" << std::endl;
    result.print();

    return 0;
}

void lb_6_test() {

    std::vector<int> task_times = {8, 8, 7,6,5,5,4,3,3,};
    int C = 10;
    int lb_6 = calc_lb_6(task_times, C);
}

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_alb_file>" << std::endl;
        std::cerr << "Example: " << argv[0] << " problem.alb" << std::endl;
        std::cerr << "Performing default run to test system" << std::endl;
        //default_run();
       rmhh_test();
        //lb_6_test();
       //vdls_salbp_1_test();
        //tails_test();
        //priority_methods_salbp_1_test();
        //tabu_test();
       // priority_methods_salbp_2_test();
        return 1;
    }
    bool salbp2 =false;
    std::optional<int> time_limit = std::nullopt;
    std::optional<int> max_attempts = std::nullopt;
    std::optional<int> seed = std::nullopt;
    int priority_n_random = 100;

    std::string heuristic = "priority";
    int n_stations;
    for (int i = 2; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "--n_stations" && i + 1 < argc) {
            n_stations = std::stoi(argv[++i]);
            salbp2 = true;

        } else if (arg == "--time_limit" && i + 1 < argc) {
            time_limit = std::stoi(argv[++i]);
        } else if (arg == "--priority_n_random" && i + 1 < argc) {
            priority_n_random = std::stoi(argv[++i]);
        } else if (arg == "--max_attempts") {
            max_attempts = std::stoi(argv[++i]);
        } else if (arg == "--random_seed") {
            seed = std::stoi(argv[++i]);
        }
        else if (arg == "--heuristic" && i + 1 < argc) {
                heuristic = argv[++i];
            }
        else {
            std::cerr << "Unknown argument: " << arg << "\n";
        }
    }

    std::string filepath = argv[1];



    // Check if file exists
    if (!std::filesystem::exists(filepath)) {
        std::cerr << "Error: File '" << filepath << "' does not exist." << std::endl;
        return 1;
    }

    // Check file extension (optional)
    std::filesystem::path path(filepath);
    if (path.extension() != ".alb") {
        std::cout << "Warning: File does not have .alb extension" << std::endl;
    }

    // Create ALBP instance and loads file
    ALBP problem;

    std::cout << "Loading ALBP file: " << filepath << std::endl;

    if (!problem.loadFromFile(filepath)) {
        std::cerr << "Error: Failed to loads file '" << filepath << "'" << std::endl;
        return 1;
    }

    // Print success message and basic info
    std::cout << "Successfully loaded ALBP problem!" << std::endl;

    // Optional: parse second argument if provided
    if (salbp2) {
        try {
            problem.S = n_stations;
            std::cout<< "Stations " << problem.S << " detected, performing SALBP-2" << std::endl;
            // Print the problem details (assuming you have a print method)
            std::cout << "\n--- Problem Details ---" << std::endl;
            problem.print(false);  // print without precedence matrix
            std::cout << "\n--- Solving problem with vdls  " << "---"<< std::endl;
            ALBPSolution result = vdls_solve_salbp2(problem, {},max_attempts, time_limit);
            std::cout << "Here is the result" << std::endl;
            result.print();
        } catch (const std::exception& e) {
            std::cerr << "Error: Invalid number of stations: " << argv[2] << std::endl;
            return 1;
        }
    }

    else{
        std::cout<< "Solving SALBP-1 with " << heuristic << std::endl;
            if (heuristic == "priority"){
                assert( priority_n_random >= 0 && "Only postive integer number of random solutions expected");
                std::vector<ALBPSolution> results =  generate_priority_ranking_solutions(problem, priority_n_random, seed, time_limit);
                for (int i = 0; i < results.size(); ++i) {
                    std::cout << "Solution number " << i << " :" <<std::endl;
                    results[i].print();
                }
            }
            else if (heuristic == "MHH"){
                ALBPSolution result = mhh_solve_salbp1(problem);
                std::cout << "Here is the result" << std::endl;
                result.print();

            }  else if (heuristic == "hoffman"){
                ALBPSolution result = hoff_solve_salbp1(problem);
                std::cout << "Here is the result" << std::endl;
                result.print();

            }else if (heuristic == "VDLS") {
                ALBPSolution result =vdls_solve_salbp1(problem,{}, max_attempts, time_limit);
                std::cout << "Here is the result" << std::endl;
                result.print();
            }
            else if (heuristic == "ILS") {
                int n_iter =   max_attempts.value_or(50000);
                int tl =   time_limit.value_or(10);
                ALBPSolution result = iterated_local_search(problem, n_iter, tl, 0.5, true );
                std::cout << "Here is the result" << std::endl;
                result.print();
            }
            else if (heuristic == "TS") {
                int n_iter =   max_attempts.value_or(50000);
                int tl =   time_limit.value_or(10);
                ALBPSolution result = tabu_solve_salbp1(problem,tl );
                std::cout << "Here is the result" << std::endl;
                result.print();
            }
            else {
                std::cerr << "Heuristic was not recognized"<< std::endl;
            }
        return 0;}



}