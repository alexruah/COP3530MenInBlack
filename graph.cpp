//
// Created by alex ruah on 12/3/23.
//

#include "graph.h"

using namespace std;

Graph::Graph(std::vector<std::vector<std::string>> abductionList) {
    for (int i = 0; i < abductionList.size(); i++) {
        pair<double, double> cords;
        cords.first = stod(abductionList[i][0]);
        cords.second = stod(abductionList[i][1]);

        vector<pair<double, double>> neighbors;

        for (auto node : adjList) {
            auto nodeCords = node.first;

            if (nodeCords == cords) {
                adjList[node.first].push_back(cords);
                continue;
            }

            if (cords.first-0.5 <= nodeCords.first && nodeCords.first <= cords.first+0.5 &&
            cords.second-0.5 <= nodeCords.second && nodeCords.second <= cords.second+0.5) {
                adjList[node.first].push_back(cords);
                neighbors.emplace_back(nodeCords);
            }

            if (nodeCords.first > cords.first+1) break;
        }

        pair<pair<double, double>, vector<pair<double, double>>> toAdd = {cords, neighbors};
        adjList.insert(toAdd);
    }
}


std::vector<std::pair<double, double>> Graph::getAbductionCount(std::pair<double, double> cords) {
    std::vector<std::pair<double, double>> abductionsInArea;


    for (auto node : adjList) {
        auto nodeCords = node.first;

        if (nodeCords.first >= cords.first - 0.25 && nodeCords.first <= cords.first + 0.25 &&
                nodeCords.second >= cords.second - 0.5 && nodeCords.second <= cords.second + 0.5) {

            for (auto abduction : node.second) {
                if (abduction.first >= cords.first - 0.25 && abduction.first <= cords.first + 0.25 &&
                    abduction.second >= cords.second - 0.25 && abduction.second <= cords.second + 0.25) {

                    abductionsInArea.push_back(abduction);
                }
            }

            break;
        }

    }

    return abductionsInArea;
}

