#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/List.h"
#include "../src/Functional.h"

bool is_odd(int val)
{
    return val % 2 == 1;
}

bool is_twice(int val1, int val2)
{
    return val1 == val2 * 2;
}

TEST_CASE("test List.h", "[List]")
{
    SECTION("test List() function")
    {
        Zyx::List<int> ilist;
        REQUIRE(ilist.empty());
    }

    SECTION("test List(size_type n) function")
    {
        Zyx::List<int> ilist(3);
        REQUIRE(ilist.size() == 3);

        for (auto itr = ilist.begin(); itr != ilist.end(); ++itr)
        {
            REQUIRE(*itr == 0);
        }
    }

    SECTION("test List(size_type n, const T& val) function")
    {
        Zyx::List<int> ilist(3, 10);
        REQUIRE(ilist.size() == 3);

        for (auto itr = ilist.begin(); itr != ilist.end(); ++itr)
        {
            REQUIRE(*itr == 10);
        }
    }

    SECTION("test List(InputIterator first, InputIterator last) function")
    {
    	int arr[] = { 3, 5, 7 };
    	Zyx::List<int> ilist(arr, arr + 3);
        REQUIRE(ilist.size() == 3);

        auto itr = ilist.begin();
        REQUIRE(*itr == 3);

        ++itr;
        REQUIRE(*itr == 5); 

        ++itr;
        REQUIRE(*itr == 7);         
    }

    SECTION("test List(const List& other) function")
    {
    	int arr[] = { 3, 5, 7 };
        Zyx::List<int> ilist1(arr, arr + 3);

        Zyx::List<int> ilist2(ilist1);
        REQUIRE(ilist2.size() == 3);

        auto itr = ilist2.begin();
        REQUIRE(*itr == 3);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 7);
    }

    SECTION("test operator=(const List& other) function")
    {
    	int arr[] = { 3, 5, 7 };
        Zyx::List<int> ilist1(arr, arr + 3);
        Zyx::List<int> ilist2(5);
        
        ilist2 = ilist1;
        REQUIRE(ilist2.size() == 3);

        auto itr = ilist2.begin();
        REQUIRE(*itr == 3);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 7);
    }

    SECTION("test assign(size_type n, const T& val) function")
    {
    	int arr[] = { 3, 5, 7 };
        Zyx::List<int> ilist(arr, arr + 3);

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
        Zyx::List<int> ilist(arr, arr + 3);

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

    SECTION("test push_front(const T& val) function")
    {
        Zyx::List<int> ilist;
        ilist.push_front(3);
        REQUIRE(ilist.size() == 1);
        REQUIRE(ilist.front() == 3);
    }

    SECTION("test push_back(const T& val) function")
    {
        Zyx::List<int> ilist;
        ilist.push_back(3);
        REQUIRE(ilist.size() == 1);
        REQUIRE(ilist.back() == 3);
    }    

    SECTION("test pop_front() function")
    {
        Zyx::List<int> ilist;
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

    SECTION("test pop_back() function")
    {
        Zyx::List<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);
        ilist.pop_back();

        REQUIRE(ilist.size() == 2);

        auto itr = ilist.begin();
        REQUIRE(*itr == 7);
        
        ++itr;
        REQUIRE(*itr == 5);        
    }

    SECTION("test iterator insert(iterator pos, const T& val) function")
    {
        Zyx::List<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);

        auto itr = ilist.begin();
        ++itr;
        itr = ilist.insert(itr, 6);

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

    SECTION("test insert(iterator pos, size_type n, const T& val) function")
    {
        Zyx::List<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);

        auto itr = ilist.begin();
        ++itr;        
        ilist.insert(itr, 2, 6);

        REQUIRE(ilist.size() == 5);

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

    SECTION("test insert(iterator pos, InputIterator first, InputIterator last) function")
    {
        int arr[] = { 3, 5, 7 };
        Zyx::List<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);

        auto itr = ilist.begin();
        ++itr;        
        ilist.insert(itr, arr, arr + 3);

        REQUIRE(ilist.size() == 6);

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

    SECTION("test erase(iterator pos) function")
    {
        Zyx::List<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);

        auto itr = ilist.begin();
        ++itr;
        itr = ilist.erase(itr);

        REQUIRE(ilist.size() == 2);        
        REQUIRE(*itr == 3);

        itr = ilist.begin();
        REQUIRE(*itr == 7);

        ++itr;
        REQUIRE(*itr == 3);
    }

    SECTION("test erase(iterator first, iterator last) function")
    {
        Zyx::List<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);
        ilist.push_front(9);

        auto beg = ilist.begin();
        auto end = Zyx::prev(ilist.end());
        ++beg;
        auto itr = ilist.erase(beg, end);

        REQUIRE(ilist.size() == 2);    
        REQUIRE(*itr == 3);    

        itr = ilist.begin();
        REQUIRE(*itr == 9);

        ++itr;
        REQUIRE(*itr == 3);
    }

    SECTION("test resize(size_type n) function")
    {
        Zyx::List<int> ilist;
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
        Zyx::List<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);

        ilist.clear();
        REQUIRE(ilist.empty());
    }

    SECTION("test swap(List& other) function")
    {
    	int arr[] = { 3, 5, 7 };
        Zyx::List<int> ilist1(arr, arr + 3);
        Zyx::List<int> ilist2(5);

        ilist2.swap(ilist1);
        REQUIRE(ilist2.size() == 3);

        auto itr = ilist2.begin();
        REQUIRE(*itr == 3);

        ++itr;
        REQUIRE(*itr == 5);

        ++itr;
        REQUIRE(*itr == 7);
    }

    SECTION("test remove(const T& value) function")
    {
        Zyx::List<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);
        ilist.push_front(5);

        ilist.remove(5);
        REQUIRE(ilist.size() == 2);

        auto itr = ilist.begin();
        REQUIRE(*itr == 7);

        ++itr;
        REQUIRE(*itr == 3);
    }

    SECTION("test remove_if(UnaryPredicate pred) function")
    {
        Zyx::List<int> ilist;
        ilist.push_front(3);
        ilist.push_front(5);
        ilist.push_front(7);
        ilist.push_front(6);

        ilist.remove_if(is_odd);
        REQUIRE(ilist.size() == 1);

        auto itr = ilist.begin();
        REQUIRE(*itr == 6);
    }

    SECTION("test unique() function")
    {
        Zyx::List<int> ilist;
        ilist.push_front(3);
        ilist.push_front(3);        
        ilist.push_front(3);
        ilist.push_front(7);
        ilist.push_front(7);

        ilist.unique();
        REQUIRE(ilist.size() == 2);

        auto itr = ilist.begin();
        REQUIRE(*itr == 7);

        ++itr;
        REQUIRE(*itr == 3);
    }

    SECTION("test unique(BinaryPredicate pred) function")
    {
        Zyx::List<int> ilist;
        ilist.push_front(3);
        ilist.push_front(6);        
        ilist.push_front(12);
        ilist.push_front(7);
        ilist.push_front(14);

        ilist.unique(is_twice);
        REQUIRE(ilist.size() == 3);

        auto itr = ilist.begin();
        REQUIRE(*itr == 14);

        ++itr;
        REQUIRE(*itr == 12);

        ++itr;
        REQUIRE(*itr == 3);
    }

    // SECTION("test splice_after(const_iterator pos, List& other) function")
    // {
    // 	int arr1[] = { 1, 2, 3, 4, 5 };
    //     int arr2[] = { 10, 11, 12 };

    //     Zyx::List<int> ilist1(arr1, arr1 + 5);
    //     Zyx::List<int> ilist2(arr2, arr2 + 3);

    //     ilist2.splice_after(ilist2.begin(), ilist1);

    //     REQUIRE(ilist1.empty());
    //     REQUIRE(ilist2.size() == 8);

    //     auto itr = ilist2.begin();
    //     REQUIRE(*itr == 10);

    //     ++itr;
    //     REQUIRE(*itr == 1);

    //     ++itr;
    //     REQUIRE(*itr == 2);

    //     ++itr;
    //     REQUIRE(*itr == 3);

    //     ++itr;
    //     REQUIRE(*itr == 4);

    //     ++itr;
    //     REQUIRE(*itr == 5);

    //     ++itr;
    //     REQUIRE(*itr == 11);

    //     ++itr;
    //     REQUIRE(*itr == 12);
    // }

    // SECTION("test splice_after(const_iterator pos, List& other, const_iterator it) function")
    // {
    // 	int arr1[] = { 1, 2, 3, 4, 5 };
    //     int arr2[] = { 10, 11, 12 };

    //     Zyx::List<int> ilist1(arr1, arr1 + 5);
    //     Zyx::List<int> ilist2(arr2, arr2 + 3);

    //     ilist2.splice_after(ilist2.begin(), ilist1, Zyx::next(ilist1.begin(), 1));

    //     REQUIRE(ilist1.size() == 2);
    //     REQUIRE(ilist2.size() == 6);

    //     auto itr = ilist1.begin();
    //     REQUIRE(*itr == 1);

    //     ++itr;
    //     REQUIRE(*itr == 2);

    //     itr = ilist2.begin();
    //     REQUIRE(*itr == 10);

    //     ++itr;
    //     REQUIRE(*itr == 3);

    //     ++itr;
    //     REQUIRE(*itr == 4);

    //     ++itr;
    //     REQUIRE(*itr == 5);

    //     ++itr;
    //     REQUIRE(*itr == 11);

    //     ++itr;
    //     REQUIRE(*itr == 12);
    // }

    // SECTION("test splice_after(const_iterator pos, List<T, Alloc>& other, const_iterator first, const_iterator last) function")
    // {
    // 	int arr1[] = { 1, 2, 3, 4, 5 };
    //     int arr2[] = { 10, 11, 12 };

    //     Zyx::List<int> ilist1(arr1, arr1 + 5);
    //     Zyx::List<int> ilist2(arr2, arr2 + 3);

    //     ilist2.splice_after(ilist2.begin(), ilist1, Zyx::next(ilist1.begin(), 1), Zyx::next(ilist1.begin(), 3));

    //     REQUIRE(ilist1.size() == 4);
    //     REQUIRE(ilist2.size() == 4);

    //     auto itr = ilist1.begin();
    //     REQUIRE(*itr == 1);

    //     ++itr;
    //     REQUIRE(*itr == 2);

    //     ++itr;
    //     REQUIRE(*itr == 4);

    //     ++itr;
    //     REQUIRE(*itr == 5);

    //     itr = ilist2.begin();
    //     REQUIRE(*itr == 10);

    //     ++itr;
    //     REQUIRE(*itr == 3);

    //     ++itr;
    //     REQUIRE(*itr == 11);

    //     ++itr;
    //     REQUIRE(*itr == 12);
    // }

    // SECTION("test reverse() function")
    // {
    //     Zyx::List<int> ilist;
    //     ilist.push_front(3);
    //     ilist.push_front(5);
    //     ilist.push_front(7);

    //     ilist.reverse();
    //     REQUIRE(ilist.size() == 3);

    //     auto itr = ilist.begin();
    //     REQUIRE(*itr == 3);

    //     ++itr;
    //     REQUIRE(*itr == 5);

    //     ++itr;
    //     REQUIRE(*itr == 7);
    // }

    // SECTION("test merge() function")
    // {
    // 	int arr1[] = { 1, 2, 8, 14, 15 };
    //     int arr2[] = { 6, 11, 12 };

    //     Zyx::List<int> ilist1(arr1, arr1 + 5);
    //     Zyx::List<int> ilist2(arr2, arr2 + 3);

    //     ilist1.merge(ilist2);

    //     REQUIRE(ilist1.size() == 8);
    //     REQUIRE(ilist2.size() == 0);

    //     auto itr = ilist1.begin();
    //     REQUIRE(*itr == 1);

    //     ++itr;
    //     REQUIRE(*itr == 2);

    //     ++itr;
    //     REQUIRE(*itr == 6);

    //     ++itr;
    //     REQUIRE(*itr == 8);

    //     ++itr;
    //     REQUIRE(*itr == 11);

    //     ++itr;
    //     REQUIRE(*itr == 12);

    //     ++itr;
    //     REQUIRE(*itr == 14);

    //     ++itr;
    //     REQUIRE(*itr == 15);
    // }

    // SECTION("test merge(Compare comp) function")
    // {
    // 	int arr1[] = { 13, 12, 8, 5, 4 };
    //     int arr2[] = { 11, 6, 2 };

    //     Zyx::List<int> ilist1(arr1, arr1 + 5);
    //     Zyx::List<int> ilist2(arr2, arr2 + 3);

    //     ilist1.merge(ilist2, Zyx::greater<int>());

    //     REQUIRE(ilist1.size() == 8);
    //     REQUIRE(ilist2.size() == 0);

    //     auto itr = ilist1.begin();
    //     REQUIRE(*itr == 13);

    //     ++itr;
    //     REQUIRE(*itr == 12);

    //     ++itr;
    //     REQUIRE(*itr == 11);

    //     ++itr;
    //     REQUIRE(*itr == 8);

    //     ++itr;
    //     REQUIRE(*itr == 6);

    //     ++itr;
    //     REQUIRE(*itr == 5);

    //     ++itr;
    //     REQUIRE(*itr == 4);

    //     ++itr;
    //     REQUIRE(*itr == 2);
    // }

    // SECTION("test sort() function")
    // {
    // 	int arr[] = { 11, 2, 8, 4, 5 };
    //     Zyx::List<int> ilist(arr, arr + 5);

    //     ilist.sort();
    //     REQUIRE(ilist.size() == 5);

    //     auto itr = ilist.begin();
    //     REQUIRE(*itr == 2);

    //     ++itr;
    //     REQUIRE(*itr == 4);

    //     ++itr;
    //     REQUIRE(*itr == 5);

    //     ++itr;
    //     REQUIRE(*itr == 8);

    //     ++itr;
    //     REQUIRE(*itr == 11);
    // }

    // SECTION("test sort(Compare comp) function")
    // {
    // 	int arr[] = { 11, 2, 8, 4, 5 };
    //     Zyx::List<int> ilist(arr, arr + 5);

    //     ilist.sort(Zyx::greater<int>());
    //     REQUIRE(ilist.size() == 5);

    //     auto itr = ilist.begin();
    //     REQUIRE(*itr == 11);

    //     ++itr;
    //     REQUIRE(*itr == 8);

    //     ++itr;
    //     REQUIRE(*itr == 5);

    //     ++itr;
    //     REQUIRE(*itr == 4);

    //     ++itr;
    //     REQUIRE(*itr == 2);
    // }

    // SECTION("test operator==(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs) function")
    // {
    //     int arr[] = { 3, 6, 9 };
    //     Zyx::List<int> ilist1(arr, arr + 3);
    //     Zyx::List<int> ilist2(arr, arr + 3);
    //     REQUIRE(ilist1 == ilist2);
    // }

    // SECTION("test operator!=(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs) function")
    // {
    //     int arr[] = { 3, 6, 9 };
    //     Zyx::List<int> ilist1(arr, arr + 3);
    //     Zyx::List<int> ilist2(arr, arr + 2);
    //     REQUIRE(ilist1 != ilist2);
    // }

    // SECTION("test operator<(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs) function")
    // {
    //     int arr[] = { 3, 6, 9 };
    //     Zyx::List<int> ilist1(arr, arr + 2);
    //     Zyx::List<int> ilist2(arr, arr + 3);
    //     REQUIRE(ilist1 < ilist2);
    // }

    // SECTION("test operator>(const List<T, Alloc>& lhs, const List<T, Alloc>& rhs) function")
    // {
    // 	int arr1[] = { 9, 2 };
    //     int arr2[] = { 3, 6, 9 };
    //     Zyx::List<int> ilist1(arr1, arr1 + 2);
    //     Zyx::List<int> ilist2(arr2, arr2 + 3);
    //     REQUIRE(ilist1 > ilist2);
    // }
}