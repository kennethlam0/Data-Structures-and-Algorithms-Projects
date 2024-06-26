// Project UID c1f28c309e55405daf00c565d57ff9ad


#include "List.h"
#include "unit_test_framework.h"


using namespace std;


// Add your test cases here


TEST(test_list_default_ctor) {
  List<int> list;
  ASSERT_EQUAL(list.size(), 0);
}

TEST(test_list_copy_ctor) {
  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  List<int> list_copy(list);
  ASSERT_EQUAL(list_copy.front(), 5);
  ASSERT_EQUAL(list_copy.back(),15)
  list_copy.pop_back();
  ASSERT_EQUAL(list_copy.front(),5);
  ASSERT_EQUAL(list_copy.back(),10);
  list_copy.pop_back();
  ASSERT_EQUAL(list_copy.front(),5);
  ASSERT_EQUAL(list_copy.back(),5);
}

TEST(test_list_empty) {
  List<int> list;
  ASSERT_TRUE(list.empty());
  list.push_front(1);
  list.push_front(2);
  list.push_front(3);
  ASSERT_FALSE(list.empty());
  list.pop_front();
  ASSERT_FALSE(list.empty());
  list.pop_front();
  ASSERT_FALSE(list.empty());
  list.pop_front();
  ASSERT_TRUE(list.empty());
}

TEST(test_list_size) {
  List<int> list;
  ASSERT_EQUAL(list.size(), 0);
  list.push_front(1);
  ASSERT_EQUAL(list.size(), 1);
  list.push_front(5);
  ASSERT_EQUAL(list.size(),2);
  list.push_front(7);
  ASSERT_EQUAL(list.size(),3)
  list.clear();
  ASSERT_EQUAL(list.size(), 0);
}

TEST(test_list_push_front) {
  List<int> list;
  list.push_front(5);
  ASSERT_EQUAL(list.front(), 5);
  ASSERT_EQUAL(list.back(), 5);
  ASSERT_EQUAL(list.size(),1);
  list.push_front(10);
  ASSERT_EQUAL(list.size(),2);
  list.push_front(15);
  ASSERT_EQUAL(list.front(), 15);
  ASSERT_EQUAL(list.back(), 5);
  ASSERT_EQUAL(list.size(),3);
}

TEST(test_list_push_back) {
  List<int> list;
  list.push_back(3);
  ASSERT_EQUAL(list.size(),1);
  ASSERT_EQUAL(list.front(), 3);
  ASSERT_EQUAL(list.back(), 3);
  list.push_back(6);
  ASSERT_EQUAL(list.size(),2);
  ASSERT_EQUAL(list.front(), 3);
  ASSERT_EQUAL(list.back(), 6);
  list.push_back(9);
  ASSERT_EQUAL(list.size(),3);
  ASSERT_EQUAL(list.front(), 3);
  ASSERT_EQUAL(list.back(), 9);
  list.push_back(12);
  ASSERT_EQUAL(list.size(),4);
  ASSERT_EQUAL(list.front(),3);
  ASSERT_EQUAL(list.back(),12);
}


TEST(test_list_pop_front) {
  List<int> list;
  list.push_back(10);
  list.push_back(20);
  list.pop_front();
  ASSERT_EQUAL(list.front(),20);
  ASSERT_EQUAL(list.back(),20);
  ASSERT_EQUAL(list.size(),1);
  list.push_front(30);
  list.push_front(50);
  list.push_front(40);
  ASSERT_EQUAL(list.size(),4);
  list.pop_front();
  ASSERT_EQUAL(list.front(),50);
  ASSERT_EQUAL(list.back(),20);
  ASSERT_EQUAL(list.size(),3);
  list.pop_front();
  list.pop_front();
  ASSERT_EQUAL(list.front(),20)
  ASSERT_EQUAL(list.back(),20);
  list.pop_front();
  ASSERT_TRUE(list.empty());
}

TEST(test_list_pop_back) {
  List<int> list;
  list.push_front(4);
  list.push_front(2);
  list.push_back(6);
  list.push_back(8);
  ASSERT_EQUAL(list.size(),4);
  ASSERT_EQUAL(list.front(),2);
  ASSERT_EQUAL(list.back(),8);
  list.pop_back();
  ASSERT_EQUAL(list.size(),3);
  ASSERT_EQUAL(list.back(), 6);
  list.pop_back();
  ASSERT_EQUAL(list.size(),2);
  ASSERT_EQUAL(list.back(),4);
}

TEST(test_list_clear) {
  List<int> list;
  ASSERT_TRUE(list.empty());
  list.clear();
  ASSERT_TRUE(list.empty());
  list.push_back(1);
  ASSERT_FALSE(list.empty());
  list.clear();
  ASSERT_TRUE(list.empty());
  List<int> list2;
  list2.push_back(2);
  list2.push_back(4);
  list.push_front(3);
  list.push_front(10);
  ASSERT_FALSE(list.empty());
  list.clear();
  ASSERT_TRUE(list.empty());

}

TEST(iterator1) {
  List<int>::Iterator i;
  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  i= list.begin();

  ASSERT_EQUAL(*i , 5);
}

TEST(iterator3) {
  List<int>::Iterator i;
  List<int>::Iterator i1;

  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  List<int> list2;
  list2.push_back(5);
  list2.push_back(10);
  list2.push_back(15);
  list2.push_back(120);

  i= list.end();
  i1 = list2.end();
  ASSERT_EQUAL(i , i1);
}

TEST(iterator_increment) {
  List<int>::Iterator i;
  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  i= list.begin();
  ++i;
  ++i;
  ASSERT_EQUAL(*i , 15);
}


TEST(iterator_decrement) {
  List<int>::Iterator i;
  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  i = list.begin();
  ++i;
  ++i;
  --i;

  ASSERT_EQUAL(*i , 10);
}


TEST(iterator_equal) {
  List<int>::Iterator it1;
  List<int>::Iterator it2;


  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);


  it1 = list.begin();
  it2 = list.begin();
 
  ASSERT_TRUE(*it1 == *it2);
}

TEST(iterator_equal1) {
  List<int>::Iterator it1;
  List<int>::Iterator it2;

  List<int> list;
  List<int> list1;

  list.push_back(5);
  list.push_back(10);
  list.push_back(15);

  list1.push_back(3);
  list1.push_back(10);
  list1.push_back(15);

  it1 = list.begin();
  it2 = list1.begin();
 
  ASSERT_FALSE(*it1 == *it2);
  ++it1;
  ++it2;
  ASSERT_TRUE(*it1 == *it2);
}


TEST(iterator_notequal) {
  List<int>::Iterator it1;
  List<int>::Iterator it2;

  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);


  it1 = list.begin();
  ++it1;

  it2 = list.begin();
 
  ASSERT_TRUE(*it1 != *it2);
}


TEST(iterator_notequal1) {
  List<int>::Iterator it1;
  List<int>::Iterator it2;

  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);


  it1 = list.begin();
  it2 = list.begin();
 
  ASSERT_FALSE(*it1 != *it2);
}

TEST(iterator_notequal2) {
  List<int>::Iterator it1;
  List<int>::Iterator it2;

  List<int> list;
  List<int> list1;

  list.push_back(5);
  list.push_back(10);
  list.push_back(15);

  list1.push_back(3);
  list1.push_back(10);
  list1.push_back(15);

  it1 = list.begin();
  it2 = list1.begin();
  ASSERT_TRUE(it1 != it2);
  ASSERT_TRUE(*it1 != *it2);
  ++it1;
  ++it2;
  ASSERT_FALSE(*it1 != *it2);
}


TEST(iterator_assignment) {
  List<int>::Iterator it1;
  List<int>::Iterator it2;

  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);

  it1 = list.begin();
  it2 = it1;
 
  ASSERT_EQUAL(5, *it2);
  ASSERT_TRUE(*it1 == *it2);
}

TEST(iterator_assignment1) {
  List<int>::Iterator it1;
  List<int>::Iterator it2;

  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);


  it1 = list.end();
  it2 = it1;
  it2 = list.begin();
  ASSERT_TRUE(5 == *it2);
}
//assign list
TEST(iterator_assignment2) {
  List<int> list1;
  List<int> list;
  List<int>::Iterator it1;

  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);

  list1 = list;
  ASSERT_EQUAL(list1.size(), 4);
  it1 = list1.begin();
  ASSERT_EQUAL(*it1, 5);
}

//erase first
TEST(iterator_erase) {
  List<int>::Iterator it1;
  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);
  it1 = list.begin();
  list.erase(it1);
  it1 = list.begin();
  ASSERT_EQUAL(*it1, 10);
}

//erase last
TEST(iterator_erase1) {
  List<int>::Iterator it1;
  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);
  it1 = list.begin();
  ++it1;
  ++it1;
  ++it1;
  list.erase(it1);
  int ri = list.size();
  ASSERT_EQUAL(ri, 3);
}

//erase middle  everything 
TEST(iterator_erase2) {
  List<int>::Iterator it1;
  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);
  it1 = list.begin();
  ++it1;
  ++it1;
  list.erase(it1);
  int ri = list.size();
  ASSERT_EQUAL(ri, 3);
  it1 = list.begin();
  list.erase(it1);
  it1 = list.begin();
  ASSERT_EQUAL(*it1, 10);
  list.erase(it1);
  it1 = list.begin();
  list.erase(it1);

  ASSERT_EQUAL(list.size(),0);
}


//insert in the beginning
TEST(iterator_insert) {
  List<int>::Iterator it1;
  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);
  it1 = list.begin();
  list.insert(it1,1);
  it1 = list.begin();
  ASSERT_EQUAL(*it1, 1);
}

//insert at the end
TEST(iterator_insert4) {
  List<int>::Iterator it1;
  List<int>::Iterator it2;

  List<int> list;
  list.push_back(5);
  list.push_back(20);
  it1 = list.end();
  list.insert(it1,1);

  it2 = list.begin();
  ++it2;
  ++it2;
  ASSERT_EQUAL(1, *it2);
}
//insert in the middle 
TEST(iterator_insert1) {
  List<int>::Iterator it1;
  List<int>::Iterator it2;

  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);
  it1 = list.begin();
  ++it1;
  ++it1;
  ++it1;
  list.insert(it1 , 1);


  it2= list.begin();
  ++it2;
  ++it2;
  ++it2;
  ASSERT_EQUAL(*it2, 1);
}
//insert in the middle
TEST(iterator_insert2) {
  List<int>::Iterator it1;
  List<int>::Iterator it2;


  List<int> list;
  list.push_back(5);
  list.push_back(10);
  list.push_back(15);
  list.push_back(20);
  it1 = list.begin();
  ++it1;
  list.insert(it1 , 0);


  it2= list.begin();
  ++it2;
  ASSERT_EQUAL(*it2, 0);
}
TEST(iterator_insert3) {
  List<int>::Iterator it1;
  List<int>::Iterator it2;


  List<int> list;
  it1 = list.begin();
  list.insert(it1 , 0);
  ASSERT_EQUAL(list.size(), 1);

  it2= list.begin();
  ASSERT_EQUAL(*it2, 0);
}
TEST_MAIN()