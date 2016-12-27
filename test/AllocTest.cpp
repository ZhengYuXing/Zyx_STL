#include <iostream>
#include "../src/Alloc.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("test Alloc.h", "[Alloc]")
{
    Zyx::simple_alloc<int, Zyx::alloc> simpleAlloc;
    int* p = simpleAlloc.allocate(3);
    p[1] = 11;
    REQUIRE(p[1] == 11);
    simpleAlloc.deallocate(p, 3);
}