#include <atomic>
#include <charconv>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

// g++ question2.cpp -g -std=c++17 -Wall -Wextra -pthread

std::atomic<uint32_t> threadTurn{0};
std::size_t currIndex{0};

void printChars(const std::string& str, std::size_t charCount, uint32_t myTurn, uint32_t threadCount) {
    while (true) {
        while(threadTurn.load(std::memory_order_acquire) != myTurn) {}
    
        std::cout << "Thread" << myTurn + 1 << ": ";
    
        auto index = currIndex;
        for (std::size_t i = 0; i < charCount; ++i) {
            std::cout << str[index++];
            index = (index == str.length()) ? 0 : index; 
        }
        std::cout << std::endl;
    
        currIndex = index;

        uint32_t nextTurn = myTurn + 1;
        nextTurn = (nextTurn == threadCount) ? 0 : nextTurn;
        threadTurn.store(nextTurn, std::memory_order_release);
    }
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "Command line args cannot be less than 3!" << std::endl;
        exit(1);
    }
    
    std::string str{argv[1]};
    std::size_t charCount{};
    if (auto [ptr, ec] = std::from_chars(argv[2], argv[2] + strlen(argv[2]), charCount); ec != std::errc()) {
        std::cerr << "Unable to parse char count. Error: " << std::make_error_code(ec).message() << std::endl;
        exit(1);
    }

    if (charCount == 0) {
        std::cerr << "char count should be greater than 0" << std::endl;
        exit(1);
    }

    uint32_t threadCount{};
    if (auto [ptr, ec] = std::from_chars(argv[3], argv[3] + strlen(argv[3]), threadCount); ec != std::errc()) {
        std::cerr << "Unable to parse thread count. Error: " << std::make_error_code(ec).message() << std::endl;
        exit(1);
    }

    if (threadCount == 0) {
        std::cerr << "thread count should be greater than 0" << std::endl;
        exit(1);
    }

    std::vector<std::thread> threads;
    for (std::size_t i = 0; i < threadCount; ++i) {
        threads.emplace_back([myTurn=i, charCount=charCount, threadCount=threadCount, &str](){ printChars(str, charCount, myTurn, threadCount); });
    }

    using namespace std::chrono_literals;
    std::this_thread::sleep_for(10000ms);

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}