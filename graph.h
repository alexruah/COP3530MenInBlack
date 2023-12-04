//
// Created by alex ruah on 12/3/23.
//

#ifndef COP3530MENINBLACK_GRAPH_H
#define COP3530MENINBLACK_GRAPH_H

#include <unordered_map>
#include <map>
#include <vector>
#include <set>
#include <iostream>

class Graph{
    std::map<std::pair<double, double>, std::vector<std::pair<double, double>>> adjList;
public:

    Graph(std::vector<std::vector<std::string>> abductionList);

    std::vector<std::pair<double, double>> getAbductionCount(std::pair<double, double> cords);

};

#endif //COP3530MENINBLACK_GRAPH_H
