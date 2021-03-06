#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/Vector.h"

TEST_CASE("test Vector.h", "[Vector]")
{
    SECTION("test Vector() function")
    {
        Zyx::Vector<int> ivec;
        REQUIRE(ivec.size() == 0);
    }

    SECTION("test Vector(size_type n) function")
    {
        Zyx::Vector<int> ivec(3);
        REQUIRE(ivec.size() == 3);
        REQUIRE(ivec[0] == 0);
        REQUIRE(ivec[1] == 0);
        REQUIRE(ivec[2] == 0);
    }    

    SECTION("test Vector(size_type n, const T& val) function")
    {
        Zyx::Vector<double> ivec(3, 10);
        REQUIRE(ivec.size() == 3);
        REQUIRE(ivec[0] == 10);
        REQUIRE(ivec[1] == 10);
        REQUIRE(ivec[2] == 10);
    }

    SECTION("test Vector(InputIterator first, InputIterator last) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);
        REQUIRE(ivec.size() == 3);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 6);
        REQUIRE(ivec[2] == 9);
    }

    SECTION("test Vector(const Vector& x) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec1(arr, arr + 3);
        Zyx::Vector<int> ivec2(ivec1);        
        REQUIRE(ivec2.size() == 3);
        REQUIRE(ivec2[0] == 3);
        REQUIRE(ivec2[1] == 6);
        REQUIRE(ivec2[2] == 9);
    }

    SECTION("test operator=(const Vector& x) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec1(arr, arr + 3);
        Zyx::Vector<int> ivec2;
        ivec2 = ivec1;        
        REQUIRE(ivec2.size() == 3);
        REQUIRE(ivec2[0] == 3);
        REQUIRE(ivec2[1] == 6);
        REQUIRE(ivec2[2] == 9);
    }

    SECTION("test assign(size_type n, const T& val) function")
    {
        Zyx::Vector<int> ivec;
        ivec.assign(3, 10);
        REQUIRE(ivec.size() == 3);
        REQUIRE(ivec[0] == 10);
        REQUIRE(ivec[1] == 10);
        REQUIRE(ivec[2] == 10);
    }

    SECTION("test assign(InputIterator first, InputIterator last) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec;
        ivec.assign(arr, arr + 3);
        REQUIRE(ivec.size() == 3);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 6);
        REQUIRE(ivec[2] == 9);
    }

    SECTION("test push_back(const T& val) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);
        ivec.push_back(12);
        REQUIRE(ivec.size() == 4);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 6);
        REQUIRE(ivec[2] == 9);
        REQUIRE(ivec[3] == 12);
    }

    SECTION("test pop_back() function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);
        ivec.pop_back();
        REQUIRE(ivec.size() == 2);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 6);
    }

    SECTION("test insert(iterator pos, const T& val) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);
        REQUIRE(*ivec.insert(ivec.begin() + 1, 12) == 12);
        REQUIRE(ivec.size() == 4);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 12);
        REQUIRE(ivec[2] == 6);
        REQUIRE(ivec[3] == 9);
    }

    SECTION("test insert(iterator pos, size_type n, const T& val) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);
        ivec.insert(ivec.begin() + 1, 2, 12);
        REQUIRE(ivec.size() == 5);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 12);
        REQUIRE(ivec[2] == 12);
        REQUIRE(ivec[3] == 6);
        REQUIRE(ivec[4] == 9);
    }

    SECTION("test insert(iterator pos, InputIterator first, InputIterator last) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);
        ivec.insert(ivec.begin() + 1, arr, arr + 2);
        REQUIRE(ivec.size() == 5);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 3);
        REQUIRE(ivec[2] == 6);
        REQUIRE(ivec[3] == 6);
        REQUIRE(ivec[4] == 9);
    }

    SECTION("test erase(iterator pos) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);
        REQUIRE(*ivec.erase(ivec.begin() + 1) == 9);
        REQUIRE(ivec.size() == 2);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 9);
    }

    SECTION("test erase(iterator first, iterator last) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);
        REQUIRE(*ivec.erase(ivec.begin(), ivec.begin() + 2) == 9);
        REQUIRE(ivec.size() == 1);
        REQUIRE(ivec[0] == 9);
    }

    SECTION("test resize(size_type n, const T& val) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);

        ivec.resize(5, 10);
        REQUIRE(ivec.size() == 5);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 6);
        REQUIRE(ivec[2] == 9);
        REQUIRE(ivec[3] == 10);
        REQUIRE(ivec[4] == 10);

        ivec.resize(1, 10);
        REQUIRE(ivec.size() == 1);
        REQUIRE(ivec[0] == 3);
    }

    SECTION("test resize(size_type n) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);

        ivec.resize(5);
        REQUIRE(ivec.size() == 5);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 6);
        REQUIRE(ivec[2] == 9);
        REQUIRE(ivec[3] == 0);
        REQUIRE(ivec[4] == 0);

        ivec.resize(1);
        REQUIRE(ivec.size() == 1);
        REQUIRE(ivec[0] == 3);
    }

    SECTION("test reserve(size_type n) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);

        ivec.reserve(2);
        REQUIRE(ivec.size() == 3);
        REQUIRE(ivec.capacity() == 3);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 6);
        REQUIRE(ivec[2] == 9);

        ivec.reserve(5);
        REQUIRE(ivec.size() == 3);
        REQUIRE(ivec.capacity() == 5);
        REQUIRE(ivec[0] == 3);
        REQUIRE(ivec[1] == 6);
        REQUIRE(ivec[2] == 9);
    }

    SECTION("test clear() function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec(arr, arr + 3);
        ivec.clear();
        REQUIRE(ivec.size() == 0);	
    }

    SECTION("test swap(Vector& x) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec1(arr, arr + 3);
        Zyx::Vector<int> ivec2(arr, arr + 2);
        ivec1.swap(ivec2);

        REQUIRE(ivec1.size() == 2);
        REQUIRE(ivec1[0] == 3);
        REQUIRE(ivec1[1] == 6);

        REQUIRE(ivec2.size() == 3);
        REQUIRE(ivec2[0] == 3);
        REQUIRE(ivec2[1] == 6);
        REQUIRE(ivec2[2] == 9);
    }

    SECTION("test operator==(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec1(arr, arr + 3);
        Zyx::Vector<int> ivec2(arr, arr + 3);
        REQUIRE(ivec1 == ivec2);
    }

    SECTION("test operator!=(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec1(arr, arr + 3);
        Zyx::Vector<int> ivec2(arr, arr + 2);
        REQUIRE(ivec1 != ivec2);
    }

    SECTION("test operator<(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) function")
    {
        int arr[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec1(arr, arr + 2);
        Zyx::Vector<int> ivec2(arr, arr + 3);
        REQUIRE(ivec1 < ivec2);
    }

    SECTION("test operator>(const Vector<T, Alloc>& lhs, const Vector<T, Alloc>& rhs) function")
    {
    	int arr1[] = { 9, 2 };
        int arr2[] = { 3, 6, 9 };
        Zyx::Vector<int> ivec1(arr1, arr1 + 2);
        Zyx::Vector<int> ivec2(arr2, arr2 + 3);
        REQUIRE(ivec1 > ivec2);
    }    
}