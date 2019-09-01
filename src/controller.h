//
// Created by Trebing, Peter on 2019-08-27.
//
#ifndef CONTROLLER_H
#define CONTROLLER_H

/*
 * Structur to hold the actual user actions
 */
struct KeyState {
    bool plus;
    bool minus;
    bool heat;
    bool cool;
};

/**
 * Class to handle the user input
 */
class Controller {

public:
    Controller() : keys{false, false, false} {}

    void HandleInput(bool &running, KeyState &keys);

    KeyState getKeys() { return keys; }

private:
    KeyState keys;
};

#endif