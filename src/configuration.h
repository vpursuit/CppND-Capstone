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
        fps = getIntParameter("fps");
        window_width = getIntParameter("window_width");
        window_height = getIntParameter("window_height");
        physic_interval_ms = getIntParameter("physic_interval_ms");
        particle_count = getIntParameter("particle_count");
        particle_render_limit = getIntParameter("particle_render_limit");
        particle_velocity_range = getFloatParameter("particle_velocity_range");
        damping = getFloatParameter("damping");
        gravity_factor = getFloatParameter("gravity_factor");
        collision_limit = getIntParameter("collision_limit");
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

    std::size_t getFPS() { return fps; }

    std::size_t getWindowWidth() { return window_width; }

    std::size_t getWindowHeight() { return window_height; }

    std::size_t getPhysicIntervalMs() { return physic_interval_ms; }

    std::size_t getParticleCount() { return particle_count; }

    std::size_t getParticleRenderLimit() { return particle_render_limit; }

    std::size_t getCollisionLimit() { return collision_limit; }

    double getParticleVelocityRange() { return particle_velocity_range; }

    double getDamping() { return damping; }

    double getGravityFactor() { return gravity_factor; }

private:

    std::unordered_map<std::string, std::string> keyValuesPairs;

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

    double getFloatParameter(std::string key) {
        std::string found = getParameter(key);
        return std::stod(found);
    };

    std::size_t fps;
    std::size_t window_width;
    std::size_t window_height;
    std::size_t physic_interval_ms;
    std::size_t particle_count;
    std::size_t particle_render_limit;
    std::size_t collision_limit;
    double particle_velocity_range;
    double damping;
    double gravity_factor;

};

#endif //COLLISIONSIM_CONFIGURATION_H
