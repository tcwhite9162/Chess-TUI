#include "engine.hpp"

#include <cstring>
#include <mutex>
#include <optional>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

EngineInterface::EngineInterface(const std::string& binPath)
    : in_fd(-1), out_fd(-1), pid(-1), binPath(binPath) {}

EngineInterface::~EngineInterface() {
    stop();
}

bool EngineInterface::start() {
    if (running)
        return true;

    int fromEngine[2];
    int toEngine[2];

    if (pipe(fromEngine) < 0 || pipe(toEngine) < 0) {
        perror("pipe");
        return false;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return false;
    }

    if (pid == 0) {
        // child //
        // redirect stdin
        dup2(toEngine[0], STDIN_FILENO);
        close(toEngine[0]);
        close(toEngine[1]);

        // redirect stdout
        dup2(fromEngine[1], STDOUT_FILENO);
        close(fromEngine[0]);
        close(fromEngine[1]);

        // exec engine bin path
        execl(binPath.c_str(), binPath.c_str(), (char*)nullptr);

        // if that fails
        perror("exec");
        _exit(-1);
    }

    // parent //
    close(toEngine[0]);
    close(fromEngine[1]);

    in_fd = toEngine[1];
    out_fd = fromEngine[0];

    // make read non blocking
    fcntl(out_fd, F_SETFL, O_NONBLOCK);

    running = true;
    reader = std::thread(&EngineInterface::readerLoop, this);

    return true;
}

void EngineInterface::stop() {
    if (!running)
        return;

    running = false;

    write(in_fd, "quit\n", 5);

    // wait for child to end
    waitpid(pid, nullptr, 0);

    close(in_fd);
    close(out_fd);

    if (reader.joinable())
        reader.join();
}

void EngineInterface::readerLoop() {
    char buffer[256];

    while (running) {
        ssize_t n = read(out_fd, buffer, sizeof(buffer) - 1);

        if (n > 0) {
            buffer[n] = '\0';

            std::string chunk(buffer);
            size_t pos = 0;

            while (true) {
                size_t newline = chunk.find('\n', pos);

                if (newline == std::string::npos)
                    break;

                std::string line = chunk.substr(pos, newline - pos);
                {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    messageQueue.push(line);
                }

                pos = newline + 1;
            }
        }

        usleep(100);
    }
}

void EngineInterface::sendUci() {
    const char* cmd = "uci\n";
    write(in_fd, cmd, strlen(cmd));
}

void EngineInterface::sendIsReady() {
    const char* cmd = "isready\n";
    write(in_fd, cmd, strlen(cmd));
}

void EngineInterface::newGame() {
    const char* cmd = "ucinewgame\n";
    write(in_fd, cmd, strlen(cmd));
}

void EngineInterface::endGame() {
    const char* cmd = "quit\n";
    write(in_fd, cmd, strlen(cmd));
}

void EngineInterface::setPosition(const Chess::Board& board) {
    // TODO: set engine position from board pos
    const char* cmd = "position startpos\n";
    write(in_fd, cmd, strlen(cmd));
}

void EngineInterface::setPosition(const std::string& uciPos) {
    const std::string cmd = uciPos + "\n";
    write(in_fd, cmd.c_str(), cmd.size()); 
}

void EngineInterface::setStartPosition() {
    const char* cmd = "position startpos\n";
    write(in_fd, cmd, strlen(cmd));
}

void EngineInterface::go(const int movetime)  {
    const std::string cmd = "go movetime " + std::to_string(movetime) + "\n";
    write(in_fd, cmd.c_str(), cmd.size());
}

std::optional<std::string> EngineInterface::getBestMove() {
    std::lock_guard<std::mutex> lock(queueMutex);

    if (messageQueue.empty())
        return std::nullopt;

    std::string msg = messageQueue.front();
    messageQueue.pop();

    if (msg.rfind("bestmove", 0) == 0)
        return msg;

    return std::nullopt;
}

std::optional<std::string> EngineInterface::pollRaw() {
    std::lock_guard<std::mutex> lock(queueMutex);

    if (messageQueue.empty())
        return std::nullopt;

    std::string msg = messageQueue.front();
    messageQueue.pop();
    return msg;
}
