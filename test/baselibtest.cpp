#include <catch2/catch_test_macros.hpp>
#include <catch2/generators/catch_generators.hpp>
#include <catch2/generators/catch_generators_random.hpp>
#include <catch2/generators/catch_generators_adapters.hpp>

#include "mylib.hpp"

TEST_CASE( "SimpleIO init", "[simpleIO]" ) {
    mylib::SimpleIO mytest;
    SECTION("Verify initialization"){
        REQUIRE( mytest.getI() == 0 );
    }
    SECTION("Setting/Getting"){
        auto i = GENERATE(take(100,random(-100,100)));
        mytest.setI(i);
        REQUIRE(  mytest.getI() == i );
    }
}