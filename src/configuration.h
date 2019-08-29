//
// Created by Trebing, Peter on 2019-08-27.
//

#ifndef COLLISIONSIM_CONFIGURATION_H
#define COLLISIONSIM_CONFIGURATION_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>

class Configuration {

public:

    static std::string DEFAULT_CONFIGFILE;

    Configuration() {
        load();
    }

    void load(std::string filename = Configuration::DEFAULT_CONFIGFILE) {
        {
            // std::ifstream is RAII, i.e. no need to call close
            std::ifstream cFile(filename);
            if (cFile.is_open()) {
                std::string line;
                while (getline(cFile, line)) {
                    line.erase(std::remove_if(line.begin(), line.end(), isspace),
                               line.end());
                    if (line[0] == '#' || line.empty())
                        continue;
                    auto delimiterPos = line.find("=");
                    auto name = line.substr(0, delimiterPos);
                    auto value = line.substr(delimiterPos + 1);
                    keyValuesPairs.insert(make_pair(name, value));
                    std::cout << name << " " << value << '\n';
                }

            } else {
                std::cerr << "Couldn't open config file for reading.\n";
            }
        };
    };

    std::string getParameter(std::string key) {
        auto entry = keyValuesPairs.find(key);
        if (entry == keyValuesPairs.end())
            return "null";
        else
            return entry->second;
    };

    std::size_t getIntParameter(std::string key) {
        std::string found = getParameter(key);
        return std::stoi(found);
    };


private:
    std::unordered_map<std::string, std::string> keyValuesPairs;
};

std::string Configuration::DEFAULT_CONFIGFILE = "simulation_config.txt";

#endif //COLLISIONSIM_CONFIGURATION_H
