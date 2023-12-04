//
// Created by alex ruah on 12/3/23.
//
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <sstream>

#include "graph.h"

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
        const char *url = "https://raw.githubusercontent.com/rfordatascience/tidytuesday/master/data/2019/2019-06-25/ufo_sightings.csv";

        std::string csvData;

        std::vector<char> ufo_sightings;
        std::vector<char> bigfoot_sightings;



        // Set the URL to download
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Set the callback function to write data to the file
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &csvData);

        // Perform the request
        res = curl_easy_perform(curl);

        // Cleanup
        curl_easy_cleanup(curl);

        // Close the output file

        if (res != CURLE_OK) {
            std::cerr << "Failed to download the file: " << curl_easy_strerror(res) << std::endl;
            return 1;
        }

        std::istringstream csvStream(csvData);

        std::vector<std::vector<std::string>> data;

        std::string line;
        int buffer = -1;

        while (std::getline(csvStream, line)) {

            if (buffer == -1) {
                buffer = 0;
                continue;
            }

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

        }

        Graph* myGraph = new Graph(data);

    }

    curl_global_cleanup();
    return 0;
};
