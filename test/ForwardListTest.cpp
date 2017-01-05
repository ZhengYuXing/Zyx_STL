#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/ForwardList.h"

TEST_CASE("test ForwardList.h", "[ForwardList]")
{
    SECTION("test ForwardList() function")
    {
        Zyx::ForwardList<int> ilist;
        REQUIRE(ilist.size() == 0);
    }

    SECTION("test ForwardList(size_type count) function")
    {
        Zyx::ForwardList<int> ilist(3);
        REQUIRE(ilist.size() == 3);

        auto itr = ilist.begin();
        REQUIRE(*itr == 0);

        ++itr;
        REQUIRE(*itr == 0); 

        ++itr;
        REQUIRE(*itr == 0); 
    }

    SECTION("test ForwardList(size_type count, const T& value) function")
    {
        Zyx::ForwardList<int> ilist(3, 10);
        REQUIRE(ilist.size() == 3);

        auto itr = ilist.begin();
        REQUIRE(*itr == 10);

        ++itr;
        REQUIRE(*itr == 10); 

        ++itr;
        REQUIRE(*itr == 10); 
    }

    SECTION("test ForwardList(InputIterator first, InputIterator last) function")
    {
    	int arr[] = { 3, 5, 7 };
    	Zyx::ForwardList<int> ilist(arr, arr + 3);
        REQUIRE(ilist.size() == 3);

        auto itr = ilist.begin();
        REQUIRE(*itr == 3);

        ++itr;
        REQUIRE(*itr == 5); 

        ++itr;
        REQUIRE(*itr == 7);         
    }

    SECTION("test ForwardList(const ForwardList& other) function")
    {
    	int arr[] = { 3, 5, 7 };
        Zyx::ForwardList<int> ilist1(arr, arr + 3);

        Zyx::ForwardList<int> ilist2(ilist1);
        REQUIRE(ilist2.size() == 3);

        auto itr = ilist2.begin();
        REQUIRE(*itr == 3);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 7);
    }

    SECTION("test operator=(const ForwardList& other) function")
    {
    	int arr[] = { 3, 5, 7 };
        Zyx::ForwardList<int> ilist1(arr, arr + 3);
        Zyx::ForwardList<int> ilist2(5);
        
        ilist2 = ilist1;
        REQUIRE(ilist2.size() == 3);

        auto itr = ilist2.begin();
        REQUIRE(*itr == 3);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 7);
    }

    SECTION("test assign(size_type count, const T& value) function")
    {
    	int arr[] = { 3, 5, 7 };
        Zyx::ForwardList<int> ilist(arr, arr + 3);

        ilist.assign(4, 10);
        REQUIRE(ilist.size() == 4);

        auto itr = ilist.begin();
        REQUIRE(*itr == 10);

        ++itr;
        REQUIRE(*itr == 10);

        ++itr;
        REQUIRE(*itr == 10);

        ++itr;
        REQUIRE(*itr == 10);

        ilist.assign(2, 10);
        REQUIRE(ilist.size() == 2);

        itr = ilist.begin();
        REQUIRE(*itr == 10);

        ++itr;
        REQUIRE(*itr == 10);
    }

    SECTION("test assign(InputIterator first, InputIterator last) function")
    {
    	int arr[] = { 3, 5, 7, 9 };
        Zyx::ForwardList<int> ilist(arr, arr + 3);

        ilist.assign(arr, arr + 4);
        REQUIRE(ilist.size() == 4);

        auto itr = ilist.begin();
        REQUIRE(*itr == 3);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 7);

        ++itr;
        REQUIRE(*itr == 9);

        ilist.assign(arr + 1, arr + 3);
        REQUIRE(ilist.size() == 2);

        itr = ilist.begin();
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 7);        
    }

    SECTION("test push_front(const T& value) function")
    {
        Zyx::ForwardList<int> ilist;
        ilist.push_front(3);
        REQUIRE(ilist.size() == 1);
        REQUIRE(ilist.front() == 3);
    }

    SECTION("test pop_front() function")
    {
        Zyx::ForwardList<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);
        ilist.pop_front();

        REQUIRE(ilist.size() == 2);

        auto itr = ilist.begin();
        REQUIRE(*itr == 5);
        
        ++itr;
        REQUIRE(*itr == 3);        
    }

    SECTION("test insert_after(const_iterator pos, const T& value) function")
    {
        Zyx::ForwardList<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);

        auto itr = ilist.begin();
        itr = ilist.insert_after(itr, 6);

        REQUIRE(ilist.size() == 4);        
        REQUIRE(*itr == 6);

        itr = ilist.begin();
        REQUIRE(*itr == 7);

        ++itr;
        REQUIRE(*itr == 6);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 3);
    }

    SECTION("test insert_after(const_iterator pos, size_type count, const T& value) function")
    {
        Zyx::ForwardList<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);

        auto itr = ilist.begin();        
        itr = ilist.insert_after(itr, 2, 6);

        REQUIRE(ilist.size() == 5);
        REQUIRE(*itr == 6);

        itr = ilist.begin();
        REQUIRE(*itr == 7);

        ++itr;
        REQUIRE(*itr == 6);

        ++itr;
        REQUIRE(*itr == 6);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 3);
    }

    SECTION("test insert_after(const_iterator pos, InputIterator first, InputIterator last) function")
    {
    	int arr[] = { 3, 5, 7 };
        Zyx::ForwardList<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);

        auto itr = ilist.begin();        
        itr = ilist.insert_after(itr, arr, arr + 3);

        REQUIRE(ilist.size() == 6);
        REQUIRE(*itr == 7);

        itr = ilist.begin();
        REQUIRE(*itr == 7);

        ++itr;
        REQUIRE(*itr == 3);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 7);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 3);
    }

    SECTION("test erase_after(const_iterator pos) function")
    {
        Zyx::ForwardList<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);

        auto itr = ilist.begin();
        itr = ilist.erase_after(itr);

        REQUIRE(ilist.size() == 2);        
        REQUIRE(*itr == 3);

        itr = ilist.begin();
        REQUIRE(*itr == 7);

        ++itr;
        REQUIRE(*itr == 3);
    }

    SECTION("test erase_after(const_iterator first, const_iterator last) function")
    {
        Zyx::ForwardList<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);
        ilist.push_front(9);

        auto beg = ilist.begin();
        auto end = next(beg, 3);
        auto itr = ilist.erase_after(beg, end);

        REQUIRE(ilist.size() == 2);        
        REQUIRE(*itr == 3);

        itr = ilist.begin();
        REQUIRE(*itr == 9);

        ++itr;
        REQUIRE(*itr == 3);
    }

    SECTION("test resize(size_type count) function")
    {
        Zyx::ForwardList<int> ilist;
        ilist.push_front(5);
        ilist.push_front(7);
        ilist.push_front(9);

        ilist.resize(5);
        REQUIRE(ilist.size() == 5);

        auto itr = ilist.begin();
        REQUIRE(*itr == 9);

        ++itr;
        REQUIRE(*itr == 7);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 0);

        ++itr;
        REQUIRE(*itr == 0);

        ilist.resize(2);
        REQUIRE(ilist.size() == 2);

        itr = ilist.begin();
        REQUIRE(*itr == 9);

        ++itr;
        REQUIRE(*itr == 7);
    }

    SECTION("test clear() function")
    {
        Zyx::ForwardList<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);

        ilist.clear();
        REQUIRE(ilist.size() == 0);
    }

    SECTION("test swap(ForwardList& other) function")
    {
    	int arr[] = { 3, 5, 7 };
        Zyx::ForwardList<int> ilist1(arr, arr + 3);
        Zyx::ForwardList<int> ilist2(5);

        ilist2.swap(ilist1);
        REQUIRE(ilist2.size() == 3);

        auto itr = ilist2.begin();
        REQUIRE(*itr == 3);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 7);
    }
}