//
// Created by alex ruah on 12/3/23.
//
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

        string bigfootDataFile = "/Users/alexruah/Downloads/bfro-report-locations.csv";
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

            data.push_back(row);
        }

//        std::cout << data[1].size() << std::endl;
//        MyKDTree* KD = new MyKDTree(data);
//
//        srand(time(NULL));
//        //ifstream config("boards/config.cfg");
//        RenderWindow window(VideoMode(1000, 667), "Men In Black");
//        sf::Texture t;
//        t.loadFromFile("/Users/alexruah/PycharmProjects/COP3530/COP3530MenInBlack/background.jpg");
//        sf::Sprite s(t);
//        window.setVerticalSyncEnabled(true);
//        window.clear(Color::Black);
//        Font input_font;
//        if (!input_font.loadFromFile("/Users/alexruah/PycharmProjects/COP3530/COP3530MenInBlack/DIGITALDREAMFAT.ttf"))
//            return EXIT_FAILURE;
//        Font output_font;
//        if (!output_font.loadFromFile("/Users/alexruah/PycharmProjects/COP3530/COP3530MenInBlack/arial.ttf"))
//            return EXIT_FAILURE;
//        sf::String playerInput1;
//        sf::String playerInput2;
//        sf::String output;
//        sf::Text playerText1("Latitude", input_font, 24);
//        sf::Text playerText2("Longitude", input_font, 24);
//        sf::Text output_text("", output_font, 35);
//        playerText1.setPosition(357, 281);
//        playerText2.setPosition(357, 340);
//        output_text.setPosition(300, 550);
//        playerText1.setColor(sf::Color::Green);
//        playerText2.setColor(sf::Color::Green);
//        output_text.setColor(sf::Color::White);
//        bool top_or_bottom = true;
//        int num_sightings = 0;
//        while (window.isOpen()) {
//            sf::Event event;
//
//            while (window.pollEvent(event)) {
//
//                switch (event.type) {
//                    case sf::Event::Closed:
//                        window.close();
//                        cout << "CLOSED" << endl;
//                        break;
//                    case Event::MouseButtonPressed:
//                        //window.close();
//                        if (Mouse::getPosition(window).x > 357 && Mouse::getPosition(window).x < 457){
//                            if (Mouse::getPosition(window).y < 305 && Mouse::getPosition(window).y > 281) {
//                                top_or_bottom = true;
//                            } else if (Mouse::getPosition(window).y < 375 && Mouse::getPosition(window).y > 347){
//                                top_or_bottom = false;
//                            }
//                        }
//
//                        //  cout << "CLOSED" << endl;
//
//                        break;
//                    case sf::Event::TextEntered:
//                        if((event.text.unicode > 47 && event.text.unicode < 58) || (event.text.unicode > 44 && event.text.unicode < 47)) {
//                            if (top_or_bottom) {
//                                playerInput1 += event.text.unicode;
//                                playerText1.setString(playerInput1);
//                            } else {
//                                playerInput2 += event.text.unicode;
//                                playerText2.setString(playerInput2);
//                            }
//                        } else if (event.text.unicode == 10){
//                            num_sightings = KD->RangeSearch(playerInput1, playerInput2);
//                            output = "THERE ARE " + std::to_string(num_sightings);
//                            output_text.setString(output);
//                        }
//                }
//            }
//            window.draw(s);
//            window.draw(playerText1);
//            window.draw(playerText2);
//            window.draw(output_text);
//            window.display();
//            window.clear(Color::Black);
//        }
//        cout << KD->RangeSearch("29.6520", "-82.3250") << endl;
        //Graph* myGraph = new Graph(data);
    }

    curl_global_cleanup();
    return 0;
};