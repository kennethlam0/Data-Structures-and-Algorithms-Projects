// Project UID db1f506d06d84ab787baf250c265e24e

#include "BinarySearchTree.h"
#include "unit_test_framework.h"
#include <sstream>

using namespace std;


TEST(test_sorting_invariant){
    //test from spec
    BinarySearchTree<int> a;
    a.insert(5);
    a.insert(1);
    a.insert(3);
    a.insert(6);
    a.insert(10);
    a.insert(7);
    ASSERT_TRUE(a.check_sorting_invariant());
    BinarySearchTree<int> b;
    b.insert(10);
    b.insert(20);
    b.insert(30);
    b.insert(40);
    b.insert(50);
    // change first datum to 2
    BinarySearchTree<int>::Iterator it;
    it = b.find(20);
    *it = 2;
    ASSERT_FALSE(b.check_sorting_invariant());
}


TEST(test_string_sorting_invariant){
    //test from spec
    BinarySearchTree<string> a;
    a.insert("jagger");
    a.insert("computer");
    a.insert("water");
    a.insert("a");
    a.insert("b");
    a.insert("c");
    ASSERT_TRUE(a.check_sorting_invariant());
    BinarySearchTree<double> b;
    b.insert(10.2);
    b.insert(20.1);
    b.insert(20.2);
    b.insert(20.02);
    b.insert(2.2);
    ASSERT_TRUE(b.check_sorting_invariant());
}



TEST(general_fcn_test1){
    BinarySearchTree<int> b;
    ASSERT_EQUAL(b.size(), 0u);
    ASSERT_EQUAL(b.height(), 0u);
    b.insert(3);
    b.insert(1);
    b.insert(2);
    b.insert(4);
    //im unsure if the following two lines are correct
    //is 4u and 3u correct? Upon testing, I think so
    ASSERT_EQUAL(b.size(), 4u);
    ASSERT_EQUAL(b.height(), 3u);
    //create new bst to check if equal
    BinarySearchTree<int> j;
    j=b;
    ASSERT_EQUAL(*b.begin(), *j.begin());
    ostringstream oss_inorder_j;
    j.traverse_inorder(oss_inorder_j);
    ostringstream oss_inorder_b;
    j.traverse_inorder(oss_inorder_b);
    ASSERT_EQUAL(oss_inorder_b.str(), oss_inorder_j.str());
}

TEST(test_empty)
{
    BinarySearchTree<int> t;
    ASSERT_TRUE(t.empty());
    t.insert(1);
    ASSERT_FALSE(t.empty());
}

TEST(test_height)
{
    BinarySearchTree<int> empty;
    BinarySearchTree<int> t;
    BinarySearchTree<int> t2;
    ASSERT_TRUE(empty.height()==0);
    t.insert(10);
    t.insert(1);
    t.insert(5);
    t.insert(8);
    t.insert(7);
    t.insert(6);
    t.insert(2);
    t.insert(3);
    t.insert(4);
    t.insert(9);
    ASSERT_TRUE(t.height()==6);
    t2.insert(7);
    t2.insert(8);
    t2.insert(1);
    t2.insert(10);
    ASSERT_TRUE(t2.height()==3);
}

TEST(test_copy)
{
    BinarySearchTree<int> t;
    t.insert(50);
    t.insert(60);
    t.insert(70);
    t.insert(80);
    t.insert(35);
    t.insert(25);
    t.insert(15);
    BinarySearchTree<int> copy(t);
    BinarySearchTree<int>::Iterator it;
    it = copy.begin();
    ASSERT_EQUAL(15,*it);
}


TEST(test_size)
{
    BinarySearchTree<int> empty;
    BinarySearchTree<int> t;
    ASSERT_TRUE(empty.size()==0);
    t.insert(1);
    ASSERT_TRUE(t.size()==1);
    t.insert(2);
    t.insert(3);
    t.insert(4);
    ASSERT_TRUE(t.size()==4);
}

TEST(test_traverse_preorder)
{
    BinarySearchTree<int> t;
    t.insert(50);
    t.insert(30);
    t.insert(35);
    t.insert(10);
    t.insert(20);
    t.insert(25);
    ostringstream oss_preorder;
    t.traverse_preorder(oss_preorder);
    cout << "preorder" << endl;
    cout << oss_preorder.str() << endl << endl;
    ASSERT_TRUE(oss_preorder.str() == "50 30 10 20 25 35 ");
}

TEST(test_traverse_inorder)
{
    BinarySearchTree<int> t;
    t.insert(50);
    t.insert(30);
    t.insert(35);
    t.insert(10);
    t.insert(20);
    t.insert(25);
    ostringstream oss_inorder;
    t.traverse_inorder(oss_inorder);
    cout << "inorder" << endl;
    cout << oss_inorder.str() << endl << endl;
    ASSERT_TRUE(oss_inorder.str() == "10 20 25 30 35 50 ");
}


TEST(test_min)
{
    BinarySearchTree<int> t;
    BinarySearchTree<int>::Iterator it;
    t.insert(10);
    it = t.min_element();
    ASSERT_TRUE(*it==10);
    t.insert(9);
    t.insert(8);
    t.insert(7);
    t.insert(6);
    it = t.min_element();
    ASSERT_TRUE(*it==6);
    t.insert(11);
    t.insert(4);
    t.insert(3);
    t.insert(2);
    t.insert(1);
    it = t.min_element();
    ASSERT_TRUE(*it==1);
}

TEST(test_iterator)
{
    BinarySearchTree<int> j;
    BinarySearchTree<int>::Iterator it;
    j.insert(4);
    j.insert(5);
    j.insert(3);
    j.insert(6);
    j.insert(2);
    it = j.min_greater_than(5);
    ASSERT_TRUE(*it==6);
    j.insert(8);
    j.insert(3020);
    j.insert(42);
    j.insert(389329);
    ASSERT_TRUE(j.check_sorting_invariant());
    it = j.min_greater_than(8);
    ASSERT_TRUE(*it==42);
    ++it;
    ASSERT_TRUE(*it==3020);
    it = j.max_element();
    ASSERT_TRUE(*it==389329);
    it = j.min_element();
    ASSERT_TRUE(*it==2);
}

TEST(test_max)
{
    BinarySearchTree<int> t;
    BinarySearchTree<int>::Iterator it; 
    t.insert(2);
    it = t.max_element();
    ASSERT_TRUE(*it==2);
    t.insert(3);
    t.insert(4);
    it = t.max_element();
    ASSERT_TRUE(*it==4);
    t.insert(100);
    t.insert(1);
    t.insert(0);
    t.insert(6);
    t.insert(7);
    t.insert(8);
    t.insert(9);
    t.insert(10);
    it = t.max_element();
    ASSERT_TRUE(*it==100);
}

TEST(test_Iterator_insert)
{
    BinarySearchTree<int> t;
    ASSERT_EQUAL(10,*(t.insert(10)));
}

TEST(test_find)
{
    BinarySearchTree<int> tree; 
    tree.insert(10); 
    tree.insert(20); 
    tree.insert(30);
    tree.insert(40); 
    tree.insert(35);
    tree.insert(25);
    BinarySearchTree<int>::Iterator it;
    it = tree.find(100) ;
    ASSERT_TRUE(it==tree.end()) ; 
}

TEST(test_min_greater_than)
{
  BinarySearchTree<int> tree;

  tree.insert(5);
  tree.insert(6);
  tree.insert(7);
  tree.insert(8);

  ASSERT_TRUE(tree.size() == 4);
  ASSERT_TRUE(tree.height() == 4);

  tree.insert(2);
  tree.insert(3);

  ASSERT_TRUE(tree.check_sorting_invariant());
  ASSERT_TRUE(*tree.max_element() == 8);
  ASSERT_TRUE(*tree.min_element() == 2);
  ASSERT_TRUE(*tree.min_greater_than(5) == 6);
}

TEST(bst_public_test) {
    //credit to public test
  BinarySearchTree<int> tree;

  tree.insert(5);

  ASSERT_TRUE(tree.size() == 1);
  ASSERT_TRUE(tree.height() == 1);

  ASSERT_TRUE(tree.find(5) != tree.end());

  tree.insert(7);
  tree.insert(3);

  ASSERT_TRUE(tree.check_sorting_invariant());
  ASSERT_TRUE(*tree.max_element() == 7);
  ASSERT_TRUE(*tree.min_element() == 3);
  ASSERT_TRUE(*tree.min_greater_than(5) == 7);

  cout << "cout << tree.to_string()" << endl;
  cout << tree.to_string() << endl << endl;

  cout << "cout << tree" << endl << "(uses iterators)" << endl;
  cout << tree << endl << endl;

  ostringstream oss_preorder;
  tree.traverse_preorder(oss_preorder);
  cout << "preorder" << endl;
  cout << oss_preorder.str() << endl << endl;
  ASSERT_TRUE(oss_preorder.str() == "5 3 7 ");

  ostringstream oss_inorder;
  tree.traverse_inorder(oss_inorder);
  cout << "inorder" << endl;
  cout << oss_inorder.str() << endl << endl;
  ASSERT_TRUE(oss_inorder.str() == "3 5 7 ");
}

TEST(test_will_this_work) {
    BinarySearchTree<int> tree;
    ASSERT_TRUE(tree.check_sorting_invariant());
    tree.insert(14);
    tree.insert(39);
    tree.insert(85);
    tree.insert(11);
    tree.insert(15);
    tree.insert(1);  
    tree.insert(75);    
    ASSERT_TRUE(tree.check_sorting_invariant());   
    *(tree.begin()) =  2918;
    ASSERT_FALSE(tree.check_sorting_invariant()); 
    *(tree.begin()) =  -83;
    ASSERT_TRUE(tree.check_sorting_invariant());   
}

//tests for edge casses

TEST(edgeCaseChecker){
    BinarySearchTree<int> j;
    j.insert(5);
    j.insert(6);
    j.insert(4);
    j.insert(3);
    j.insert(7);
    ASSERT_TRUE(j.check_sorting_invariant());
    ASSERT_TRUE(j.height() == 3u);
    ASSERT_TRUE(j.find(8) == j.end());
    ASSERT_TRUE(j.find(2939) == j.end());
    j.insert(-1);
    j.insert(-6);
    j.insert(-5);
    ASSERT_TRUE(j.check_sorting_invariant());
    ASSERT_TRUE(j.min_element() == j.find(-6));
    ASSERT_TRUE(*j.min_element() == -6);
    ASSERT_FALSE(j.max_element() == j.find(-6));
    ASSERT_TRUE(j.max_element() == j.find(7));
    ASSERT_TRUE(*j.max_element() == 7);
    ASSERT_TRUE(j.min_greater_than(-5) == j.find(-1));
    ASSERT_TRUE(*j.min_greater_than(-5) == (-1));
    ASSERT_TRUE(j.min_greater_than(7) == j.end());
    BinarySearchTree<int> empty;
    ASSERT_TRUE(empty.height() == 0u);
    ASSERT_TRUE(empty.min_element() == empty.end());
    ASSERT_TRUE(empty.max_element() == empty.end());
    ASSERT_TRUE(empty.min_greater_than(5) == empty.end());
    ASSERT_TRUE(empty.min_greater_than(0) == empty.end());
    BinarySearchTree<int> bad;
    bad.insert(1);
    bad.insert(0);
    *bad.begin() = 2;
    ASSERT_FALSE(bad.check_sorting_invariant());
    *bad.begin() = 1;
    ASSERT_TRUE(bad.check_sorting_invariant());
}

TEST(nonIntEdgeCases){
    BinarySearchTree<string> j;
    j.insert("c");
    j.insert("b");
    j.insert("a");
    j.insert("d");
    j.insert("e");
    ASSERT_TRUE(j.check_sorting_invariant());
    ASSERT_TRUE(j.height() == 3u);
    ASSERT_TRUE(j.find("jagger") == j.end());
    ASSERT_TRUE(j.find("x") == j.end());
    j.insert("f");
    j.insert("g");
    j.insert("h");
    ASSERT_TRUE(j.check_sorting_invariant());
    ASSERT_TRUE(j.min_element() == j.find("a"));
    ASSERT_TRUE(*j.min_element() == "a");
    ASSERT_FALSE(j.max_element() == j.find("a"));
    ASSERT_TRUE(j.max_element() == j.find("h"));
    ASSERT_TRUE(*j.max_element() == "h");
    ASSERT_TRUE(j.min_greater_than("b") == j.find("c"));
    ASSERT_TRUE(*j.min_greater_than("b") == "c");
    ASSERT_TRUE(j.min_greater_than("h") == j.end());
    BinarySearchTree<string> empty;
    ASSERT_TRUE(empty.height() == 0u);
    ASSERT_TRUE(empty.min_element() == empty.end());
    ASSERT_TRUE(empty.max_element() == empty.end());
    ASSERT_TRUE(empty.min_greater_than("d") == empty.end());
    ASSERT_TRUE(empty.min_greater_than("o") == empty.end());
    BinarySearchTree<string> bad;
    bad.insert("c");
    bad.insert("b");
    *bad.begin() = "d";
    ASSERT_FALSE(bad.check_sorting_invariant());
    *bad.begin() = "a";
    ASSERT_TRUE(bad.check_sorting_invariant());
}


/*Here's a list of potential edge cases that I checked for:
 - Testing "height" when there are 5+ nodes in the tree, but a height of only 3
 - Testing "find" for an element that doesn't exist within the tree
 - Testing "min_element" on a tree with negative values, as well as an empty tree (same for max)
 - Checking the "sorting_invariant" test on multiple different trees that are invalid (see https://eecs280staff.github.io/p5-ml/#sorting-invariant for a few examples)
 - Testing "min_greater_than" with a value greater than any in the tree*/


TEST_MAIN()
