//
// Created by Trebing, Peter on 2019-08-27.
//
#ifndef COLLISIONSIM_CONFIGURATION_H
#define COLLISIONSIM_CONFIGURATION_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <unordered_map>

/**
 * Configuration can load and maintain configuration parameters
 */
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

    /**
     * Loads key value pairs from specified file and stores data in private members
     * @param filename  name of file
     */
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

    // Getter and Setters for all configuration attributes
    std::size_t getFPS() { return fps; }

    void setFPS(std::size_t fps) { this->fps = fps; }

    std::size_t getWindowWidth() { return window_width; }

    void setWindowWidth(std::size_t width) { window_width = width; }

    std::size_t getWindowHeight() { return window_height; }

    void setWindowHeight(std::size_t height) { window_height = height; }

    std::size_t getPhysicIntervalMs() { return physic_interval_ms; }

    void setPhysicIntervalMs(std::size_t interval) { physic_interval_ms = interval; }

    std::size_t getParticleCount() { return particle_count; }

    void setParticleCount(std::size_t count) { particle_count = count; }

    std::size_t getParticleRenderLimit() { return particle_render_limit; }

    void setParticleRenderLimit(std::size_t limit) { particle_render_limit = limit; }

    std::size_t getCollisionLimit() { return collision_limit; }

    void setCollisionLimit(std::size_t limit) { collision_limit = limit; }

    double getParticleVelocityRange() { return particle_velocity_range; }

    void setParticleVelocityRange(double range) { particle_velocity_range = range; }

    double getDamping() { return damping; }

    void setDamping(double damping) { this->damping = damping; }

    double getGravityFactor() { return gravity_factor; }

    void setGravityFactor(double factor) { gravity_factor = factor; }

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
