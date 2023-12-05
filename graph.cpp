//
// Created by alex ruah on 12/3/23.
//

#include "graph.h"

using namespace std;

Graph::Graph(std::vector<std::vector<std::string>> abductionList) {
    // This method goes through every alien and bigfoot incident and creates a graph where each incident points to
    // other incidents in the same area

    for (int i = 0; i < abductionList.size(); i++) {

        // Creates a pair of coordinates for each incident
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

            // If the coordinates are in the same area make them point to each other
            if (cords.first-0.5 <= nodeCords.first && nodeCords.first <= cords.first+0.5 &&
            cords.second-0.5 <= nodeCords.second && nodeCords.second <= cords.second+0.5) {
                adjList[node.first].push_back(cords);
                neighbors.push_back(nodeCords);
            }

            // Since the map is ordered by latitude, if the latitude is much smaller than the current incident's
            // latitude it won't be in the area of any following incident

            if (nodeCords.first > cords.first+1) break;
        }

        pair<pair<double, double>, vector<pair<double, double>>> toAdd = {cords, neighbors};
        adjList.insert(toAdd);
    }
}


int Graph::getAbductionCount(std::pair<string, string> sCords) {
    // This method goes through every incident and if it finds an incident in the area of the user
    // It then only has to check the incidents it's adjacent to, to find the rest of the incidents

    pair<double, double> cords = {stod(sCords.first), stod(sCords.second)};


    int res;


    for (auto node : adjList) {
        auto nodeCords = node.first;

        if (nodeCords.first >= cords.first - 0.25 && nodeCords.first <= cords.first + 0.25 &&
                nodeCords.second >= cords.second - 0.25 && nodeCords.second <= cords.second + 0.25) {

            // If there is an incident in the area of the user check it's adjacent incidents

            res++;

            for (auto abduction : node.second) {
                if (abduction.first >= cords.first - 0.25 && abduction.first <= cords.first + 0.25 &&
                    abduction.second >= cords.second - 0.25 && abduction.second <= cords.second + 0.25) {

                    res++;
                }
            }

            break;
        }

    }

    return res;
}

