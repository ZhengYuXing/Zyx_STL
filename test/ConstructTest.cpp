#include "../src/Alloc.h"
#include "../src/Construct.h"

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("test Construct.h", "[Construct]")
{
    SECTION("test construct function")
    {
        int* p = Zyx::simple_alloc<int, Zyx::alloc>::allocate();
        Zyx::construct(p, 10);
        REQUIRE(*p == 10);
        Zyx::destroy(p);
        Zyx::simple_alloc<int, Zyx::alloc>::deallocate(p);
    }

    SECTION("test uninitialized_copy function")
    {
        int* p = Zyx::simple_alloc<int, Zyx::alloc>::allocate(3);
        int arr[] = { 1, 2, 3 };
        Zyx::uninitialized_copy(arr, arr + 3, p);
        REQUIRE(p[0] == 1);
        REQUIRE(p[1] == 2);
        REQUIRE(p[2] == 3);
        Zyx::destroy(p, p + 3);
        Zyx::simple_alloc<int, Zyx::alloc>::deallocate(p, 3);
    }

    SECTION("test uninitialized_fill function")
    {
        int* p = Zyx::simple_alloc<int, Zyx::alloc>::allocate(3);
        Zyx::uninitialized_fill(p, p + 3, 5);
        REQUIRE(p[0] == 5);
        REQUIRE(p[1] == 5);
        REQUIRE(p[2] == 5);
        Zyx::destroy(p, p + 3);
        Zyx::simple_alloc<int, Zyx::alloc>::deallocate(p, 3);
    }

    SECTION("test uninitialized_fill_n function")
    {
        int* p = Zyx::simple_alloc<int, Zyx::alloc>::allocate(3);
        Zyx::uninitialized_fill_n(p, 3, 5);
        REQUIRE(p[0] == 5);
        REQUIRE(p[1] == 5);
        REQUIRE(p[2] == 5);
        Zyx::destroy(p, p + 3);
        Zyx::simple_alloc<int, Zyx::alloc>::deallocate(p, 3);
    }
}