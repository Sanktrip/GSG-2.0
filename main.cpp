#include "liveTrades.hpp"
#include "backTesting.hpp"
#include "liveTesting.hpp"

int main(int argc, char **argv) {
    bool testing = false;
    if (testing) {
        //liveTest();
        backTest();
    } else {
        marketStream();
    }
    return 0;
}