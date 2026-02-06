#include "src/engine/engine.hpp"

#include <chrono>
#include <iostream>
#include <thread>
#include <unistd.h>

int main() {
    EngineInterface engine("../Chess/build/main");

    if (!engine.start()) {
        std::cerr << "Failed to start engine\n";
        return 1;
    }

    engine.sendUci();
    std::cout << "sent: 'uci'\n";

    bool uciOK = false;
    while (!uciOK) {
        auto msg = engine.pollRaw();
        if (msg.has_value()) {
            std::cout << "ENGINE: " << msg.value() << std::endl;
            if (msg.value() == "uciok")
                uciOK = true;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    std::cout << std::endl;

    engine.sendIsReady();
    std::cout << "sent: 'isready'\n";

    engine.setStartPosition();
    std::cout << "sent: 'position startpos'\n" << std::endl;
    bool pos = false;
    while (!pos) {
        auto msg = engine.pollRaw();
        if (msg.has_value()) {
            std::cout << "ENGINE: " << msg.value() << std::endl;
            pos = true;
        }
    }

    engine.go();
    std::cout << "sent: 'go'\n";

    bool bestMove = false;
    while (!bestMove) {
        auto msg = engine.getBestMove();
        if (msg.has_value()) {
            std::cout << "ENGINE: " << msg.value() << std::endl;
            bestMove = true;
        }
    }

    return 0;
}
