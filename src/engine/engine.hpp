#pragma once

#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>

#include "../chess/board.hpp"

class EngineInterface {

  public:
    EngineInterface(const std::string& binPath);
    ~EngineInterface();

    bool start();
    void stop();

    void sendUci();
    void sendIsReady();

    void newGame();
    void endGame();

    void setPosition(const Chess::Board& board);
    void setPosition(const std::string& uciPos);
    void setStartPosition();

    void go(const int movetime = 1000);

    std::optional<std::string> getBestMove();
    std::optional<std::string> pollRaw();

  private:
    bool running = false;
    std::string binPath;

    int in_fd;
    int out_fd;
    pid_t pid;

    std::thread reader;
    std::mutex queueMutex;
    std::queue<std::string> messageQueue;

    void readerLoop();
};
