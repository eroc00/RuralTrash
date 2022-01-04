#define UNIT_TESTING

#include <iostream>
#include "catch.hpp"
#include "Header/DataStructures.hpp"
#include "Header/OpController.hpp"
#include "Header/Movement.hpp"
#include "Header/Receiver.hpp"
#include "Header/LateralController.hpp"




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

    
}


TEST_CASE("Test Operational Controller", "[OpController]") {
    
    // Test default constructor
    OpController state;
    REQUIRE(state.currMode() == off);

    // Test up() function
    for (uint8_t i = 0; i < 3; i++) {
        state.up();
        REQUIRE(state.currMode() == (Mode)(i + 1));
    }
    REQUIRE(state.currMode() == automatic);

    state.up(); // loop back to beginning
    REQUIRE(state.currMode() == off);


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


