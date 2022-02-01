#define UNIT_TESTING

#include <iostream>
#include <cstdlib>
#include <chrono>

#include "catch.hpp"
#include "Header/DataStructures.hpp"
#include "Header/OpController.hpp"
#include "Header/Movement.hpp"
#include "Header/Receiver.hpp"
#include "Header/LateralController.hpp"


std::chrono::system_clock::duration timeFunction(void* func);

TEST_CASE("Test Data Structures", "[DataStructures]") {
    PWMPair signals;

    // Test default constructor
    REQUIRE(signals.left() == 0);
    REQUIRE(signals.right() == 0);

    // Tests operator()
    signals(4, 2);
    REQUIRE(signals.left() == 4);
    REQUIRE(signals.right() == 2);

    // Test bit boundaries
    signals(-1, -1);
    REQUIRE(signals.left() == 0x3FF);
    REQUIRE(signals.right() == 0x3FF);

    signals(0, -1);
    REQUIRE(signals.left() == 0);
    REQUIRE(signals.right() == 0x3FF);

    signals(-1, 0);
    REQUIRE(signals.left() == 0x3FF);
    REQUIRE(signals.right() == 0);

    signals(845, 1023);
    REQUIRE(signals.left() == 845);
    REQUIRE(signals.right() == 1023);

    
}


TEST_CASE("Test Operational Controller", "[OpController]") {
    
    // Test default constructor
    OpController state;
    REQUIRE(state.currMode() == standby);

    // Test up() function
    for (uint8_t i = 0; i < 3; i++) {
        state.up();
        REQUIRE(state.currMode() == (Mode)(i + 1));
    }
    REQUIRE(state.currMode() == automatic);

    state.up(); // loop back to beginning
    REQUIRE(state.currMode() == standby);


    // Test down() function
    state.down();
    REQUIRE(state.currMode() == automatic);


    for(int i = 2; i >= 0; i--) {
        state.down();
        REQUIRE(state.currMode() == (Mode)i);
    }
    

}

TEST_CASE("Test Remote Controller Receiver class", "[Receiver]") {

    Receiver rcSignal;

    PWMPair signal = rcSignal.read();

    REQUIRE(signal.left() == 512);
    REQUIRE(signal.left() == 512);


}

TEST_CASE("Test Duration of Functions", "[Timing]") {

    using namespace std::chrono;

    auto t1 = system_clock::now();

    system("cd ..");
    system("dir");

    auto duration = system_clock::now() - t1;


    std::cout << "It took " << duration_cast<milliseconds>(duration).count() << " milliseconds to run this function.\n";







}

TEST_CASE("Test angle to motor signal function", "[LateralController]") {

    PWMPair signals;

    angleToMotorSignal(0, signals);

    REQUIRE(signals.left() == 1023);
    REQUIRE(signals.right() == 1023);

    angleToMotorSignal(5.0, signals);

    REQUIRE(signals.left() < 1023);
    REQUIRE(signals.left() != 0);
    REQUIRE(signals.right() == 1023);

    angleToMotorSignal(-5.0, signals);

    REQUIRE(signals.right() < 1023);
    REQUIRE(signals.right() != 0);
    REQUIRE(signals.left() == 1023);

}

TEST_CASE("Test initialization speed", "[Timing]") {

    using namespace std::chrono;

    { // with initialization
        auto t1 = system_clock::now();
        double rightWSpeed = 0, leftWSpeed = 0;
        auto initTime = system_clock::now() - t1;

        rightWSpeed = MAXSPEED + ((2 * PI * 5 * TRACKWIDTH) / (360.0 * SAMPLETIME));
        leftWSpeed = MAXSPEED - ((2 * PI * 5 * TRACKWIDTH) / (360.0 * SAMPLETIME));

        auto duration = system_clock::now() - t1;
        std::cout << "Initialization time: " 
            << duration_cast<nanoseconds>(initTime).count() << " nanoseconds.\n";

        std::cout << "Calculation time: "
            << duration_cast<nanoseconds>(duration-initTime).count() << " nanoseconds.\n";


    }

    { // without initialization
        double rightWSpeed = 0, leftWSpeed = 0;
        auto t1 = system_clock::now();

        rightWSpeed = MAXSPEED + ((2 * PI * 5 * TRACKWIDTH) / (360.0 * SAMPLETIME));
        leftWSpeed = MAXSPEED - ((2 * PI * 5 * TRACKWIDTH) / (360.0 * SAMPLETIME));

        auto duration = system_clock::now() - t1;
        std::cout << "Calculation without initialization: "
            << duration_cast<nanoseconds>(duration).count() << " nanoseconds.\n";

    }







}

/*
std::chrono::system_clock::duration timeFunction(void* func) {
    auto t1 = std::chrono::system_clock::now();

    func();

    return (std::chrono::system_clock::now() - t1);
}
*/
