//
// Created by alex ruah on 12/3/23.
//
#include <iostream>
#include <fstream>
#include <curl/curl.h>
#include <string>
#include <vector>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "my_kd_tree.h"

#include "graph.h"
using namespace sf;
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

        string bigfootDataFile = "/Users/aidanparsons/Downloads/bfro-report-locations.csv";
        std::ifstream bigfootData(bigfootDataFile);

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


        std::string line;


        std::vector<std::vector<std::string>> alien_coordinates;
        std::vector<std::vector<std::string>> bigfoot_coordinates;
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

            alien_coordinates.push_back(row);

            buffer++;

        }

        buffer = -1;


        while (std::getline(bigfootData, line)) {

            if (buffer == -1) {
                buffer = 0;
                continue;
            }

            std::vector<std::string> row;
            std::stringstream ss(line);
            std::string cell;

            int index = 0;

            while (std::getline(ss, cell, ',')) {

                if (index == 4) row.push_back(cell);
                else if (index == 5) {
                    row.push_back(cell.substr(0, cell.size()-2));
                }

                index ++;
            }

            try {
                double t = stod(row[0]);
                double t2 = stod(row[1]);

                if (t == 2004 || t2 == 2004) continue;
            }
            catch(const invalid_argument& e) {
                continue;
            }
            catch(const out_of_range& e) {
                continue;
            }

            bigfoot_coordinates.push_back(row);
        }

        cout << "test" << endl;
        //cout << bigfoot_coordinates[1][0] << " " << bigfoot_coordinates[1][1] << endl;
        MyKDTree* alien_kd = new MyKDTree(alien_coordinates);
        MyKDTree* bigfoot_kd = new MyKDTree(bigfoot_coordinates);
      //  Graph* alien_graph = new Graph(alien_coordinates);
     //   Graph* bigfoot_graph = new Graph(bigfoot_coordinates);

        srand(time(NULL));
        ifstream config("boards/config.cfg");
        RenderWindow window(VideoMode(1000, 667), "Men In Black");
        sf::Texture t;
        t.loadFromFile("/Users/aidanparsons/CLionProjects/COP3530MenInBlack/background.jpg");
        sf::Sprite s(t);
        window.setVerticalSyncEnabled(true);
        window.clear(Color::Black);
        Font input_font;
        if (!input_font.loadFromFile("/Users/aidanparsons/CLionProjects/COP3530MenInBlack/DIGITALDREAMFAT.ttf"))
            return EXIT_FAILURE;
        Font output_font;
        if (!output_font.loadFromFile("/Users/aidanparsons/CLionProjects/COP3530MenInBlack/arial.ttf"))
            return EXIT_FAILURE;
        sf::String playerInput1;
        sf::String playerInput2;
        sf::String output;
        sf::Text playerText1("Latitude", input_font, 24);
        sf::Text playerText2("Longitude", input_font, 24);
        sf::Text output_text("", output_font, 35);
        playerText1.setPosition(357, 281);
        playerText2.setPosition(357, 340);
        output_text.setPosition(250, 500);
        playerText1.setColor(sf::Color::Green);
        playerText2.setColor(sf::Color::Green);
        output_text.setColor(sf::Color::White);
        bool top_or_bottom = true;
        int num_alien_sightings = 0;
        int num_bigfoot_sightings = 0;
        while (window.isOpen()) {
            sf::Event event;

            while (window.pollEvent(event)) {

                switch (event.type) {
                    case sf::Event::Closed:
                        window.close();
                        cout << "CLOSED" << endl;
                        break;
                    case Event::MouseButtonPressed:
                        //window.close();
                        if (Mouse::getPosition(window).x > 357 && Mouse::getPosition(window).x < 457){
                            if (Mouse::getPosition(window).y < 305 && Mouse::getPosition(window).y > 281) {
                                top_or_bottom = true;
                            } else if (Mouse::getPosition(window).y < 375 && Mouse::getPosition(window).y > 347){
                                top_or_bottom = false;
                            }
                        }

                      //  cout << "CLOSED" << endl;

                        break;
                    case sf::Event::TextEntered:
                        if((event.text.unicode > 47 && event.text.unicode < 58) || (event.text.unicode > 44 && event.text.unicode < 47)) {
                            if (top_or_bottom) {
                                playerInput1 += event.text.unicode;
                                playerText1.setString(playerInput1);
                            } else {
                                playerInput2 += event.text.unicode;
                                playerText2.setString(playerInput2);
                            }
                        } else if (event.text.unicode == 10){
                            if (playerInput1 != "" && playerInput2 != "") {
                                num_alien_sightings = alien_kd->RangeSearch(playerInput1, playerInput2);
                                num_bigfoot_sightings = bigfoot_kd->RangeSearch(playerInput1, playerInput2);
                               // num_alien_sightings = alien_graph->getAbductionCount(playerInput1, playerInput2);
                               // num_bigfoot_sightings = bigfoot_graph->getAbductionCount(playerInput1, playerInput2);
                                output = "THERE ARE " + std::to_string(num_alien_sightings) +
                                         " ALIEN SIGHTINGS\n AND " + std::to_string(num_bigfoot_sightings) + " BIGFOOT SIGHTINGS\n IN YOUR AREA";
                            } else {
                                output = "";
                            }
                            output_text.setString(output);
                            playerInput1 = "";
                            playerText1.setString("LATITUDE");
                            playerInput2 = "";
                            playerText2.setString("LONGITUDE");
                        } else if (event.text.unicode == 8){
                            if (top_or_bottom && !playerInput1.isEmpty()) {
                                playerInput1.erase(playerInput1.getSize() - 1);
                                if (playerInput1.isEmpty()){
                                    playerText1.setString("LATITUDE");
                                } else {
                                    playerText1.setString(playerInput1);
                                }
                            } else if (!playerInput2.isEmpty()) {
                                playerInput2.erase(playerInput2.getSize() - 1);
                                if (playerInput2.isEmpty()){
                                    playerText2.setString("LONGITUDE");
                                } else {
                                    playerText2.setString(playerInput2);
                                }
                            }
                        }
                }
            }
            window.draw(s);
            window.draw(playerText1);
            window.draw(playerText2);
            window.draw(output_text);
            window.display();
            window.clear(Color::Black);
        }
    //    cout << alien_kd->RangeSearch("29.6520", "-82.3250") << endl;
    }

    curl_global_cleanup();
    return 0;
};