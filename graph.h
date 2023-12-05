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

struct Node{
    std::pair<double, double> cords;

    int count;
};

class Graph{
    std::map<std::pair<double, double>, std::vector<std::pair<double, double>>> adjList;
public:

    Graph(std::vector<std::vector<std::string>> abductionList);

    int getAbductionCount(std::pair<std::string, std::string> sCords);

};

#endif //COP3530MENINBLACK_GRAPH_H
