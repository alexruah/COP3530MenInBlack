//
// Created by alex ruah on 12/3/23.
//
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <sstream>
#include <chrono>

#include "graph.h"

using namespace std;

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    size_t totalSize = size * nmemb;
    data->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int main(){
    CURL* curl;
    CURLcode res;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        // Specify the URL to download
        const char *abductionDataUrl = "https://raw.githubusercontent.com/rfordatascience/tidytuesday/master/data/2019/2019-06-25/ufo_sightings.csv";

        std::string abductionData;




        // Set the URL to download
        curl_easy_setopt(curl, CURLOPT_URL, abductionDataUrl);

        // Set the callback function to write data to the file
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &abductionData);

        // Perform the request
        res = curl_easy_perform(curl);

        // Cleanup
        curl_easy_cleanup(curl);

        // Close the output file

        if (res != CURLE_OK) {
            std::cerr << "Failed to download the file: " << curl_easy_strerror(res) << std::endl;
            return 1;
        }

        std::istringstream csvStream(abductionData);

        std::vector<std::vector<std::string>> data;

        std::string line;
        int buffer = -1;

        while (std::getline(csvStream, line)) {

            if (buffer == -1) {
                buffer = 0;
                continue;
            }

            if (buffer == 20000) break;

            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;

            int index = 0;

            while (std::getline(ss, cell, ',')) {
                if (index >= 9) {
                    row.push_back(cell);
                }

                index++;
            }

            if (row[0] == "NA" or row[1] == "NA") continue;

            data.push_back(row);

            buffer++;

        }

        Graph* myGraph = new Graph(data);

        int f = -1;

        bool flag = true;

        cout << "Monsters vs Aliens!!!!!!" << "\n\n";

        while (flag) {
            cout << "Welcome to Monsters vs Aliens by Men In Black" << "\n";
            cout << "Here you could check how at risk you are of getting abducted by aliens" << "\n";

            cout << "Make a selection:" << "\n";
            cout << "1. Find out how at risk a certain coordinate is of being abducted by aliens" << "\n";
            cout << "2. Exit" << "\n";

            cout << "Make a selection: ";

            string selection;

            cin >> selection;

            int num_selection = stoi(selection);

            if (num_selection == 1) {

                string latStr;
                string longstr;

                cout << "Input latitude: ";
                cin >> latStr;
                cout << "Input longitude: ";
                cin >> longstr;

                double latitude = stod(latStr);
                double longitude = stod(longstr);

                pair<double, double> userCords = {latitude, longitude};

                auto graphStart = chrono::high_resolution_clock::now();

                vector<pair<double, double>> abductionsInAreaGraph = myGraph->getAbductionCount(userCords);

                auto graphEnd = chrono::high_resolution_clock::now();
                auto graphDuration = duration_cast<chrono::microseconds>(graphEnd - graphStart);

                cout << "Abductions in your area: ";
                cout << abductionsInAreaGraph.size();
                cout << "\n" << "Graph took " << graphDuration.count() << " microseconds" << "\n";
                cout << "\n" << "\n";

            } else if (num_selection == 2) break;

        }

        cout << "Thank you for visiting" << "\n";
        cout << "Stay safe from any aliens!!";

    }

    curl_global_cleanup();
    return 0;
};
