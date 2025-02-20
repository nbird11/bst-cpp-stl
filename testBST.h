/***********************************************************************
 * Header:
 *    TEST BST
 * Summary:
 *    Unit tests for bst
 * Author
 *    Br. Helfrich
 ************************************************************************/

#pragma once

#ifdef DEBUG

#include "bst.h"
#include "unitTest.h"
#include "spy.h"

#include <cassert>
#include <memory>
#include <iostream>
#include <string>
#include <functional> // for std::less and std::greater

 /***********************************************
  * TEST BST
  * Unit tests for the BST class
  ***********************************************/
class TestBST : public UnitTest
{

public:
   void run()
   {
      reset();

      // Construct
      test_construct_default();
      test_constructCopy_empty();
      test_constructCopy_one();
      test_constructCopy_standard();
      test_constructMove_empty();
      test_constructMove_one();
      test_constructMove_standard();
      test_constructInitializer_empty();
      test_constructInitializer_standard();
      test_destruct_empty();
      test_destruct_standard();


      // Assign
      test_assign_emptyToEmpty();
      test_assign_standardToEmpty();
      test_assign_emptyToStandard();
      test_assign_oneToStandard();
      test_assign_standardToOne();
      test_assign_standardToStandard();
      test_assignMove_emptyToEmpty();
      test_assignMove_standardToEmpty();
      test_assignMove_emptyToStandard();
      test_assignMove_oneToStandard();
      test_assignMove_standardToOne();
      test_assignMove_standardToStandard();
      test_assignInitializer_oneToStandard();
      test_assignInitializer_standardToEmpty();
      test_swap_emptyToEmpty();
      test_swap_standardToEmpty();
      test_swap_emptyToStandard();
      test_swap_standardToStandard();

      // Iterator
      test_begin_empty();
      test_begin_standard();
      test_end_standard();
      test_iterator_increment_standardToParent();
      test_iterator_increment_standardToChild();
      test_iterator_increment_standardToGrandma();
      test_iterator_increment_standardToGrandchild();
      test_iterator_increment_standardToDone();
      test_iterator_increment_standardEnd();
      test_iterator_decrement_standardToParent();
      test_iterator_decrement_standardToChild();
      test_iterator_decrement_standardToGrandma();
      test_iterator_decrement_standardToGrandchild();
      test_iterator_decrement_standardToDone();
      test_iterator_decrement_standardEnd();
      test_iterator_dereference_standardRead();

      // Find
      test_find_empty();
      test_find_standardBegin();
      test_find_standardLast();
      test_find_standardMissing();

      // Insert
      test_insert_oneLeft();
      test_insert_oneRight();
      test_insert_duplicate();
      test_insert_keepUnique();
      test_insertMove_oneLeft();
      test_insertMove_oneRight();
      test_insertMove_duplicate();
      test_insertMove_keepUnique();
      test_insert_case1();
      test_insert_case2();
      test_insert_case3();
      test_insert_case4aSimple();
      test_insert_case4bSimple();
      test_insert_case4cSimple();
      test_insert_case4dSimple();
      test_insert_case4aComplex();
      test_insert_case4bComplex();
      test_insert_case4cComplex();
      test_insert_case4dComplex();

      // Remove
      test_erase_empty();
      test_erase_standardMissing();
      test_erase_noChildren();
      test_erase_oneChild();
      test_erase_twoChildren();
      test_erase_twoChildrenSpecial();
      test_clear_empty();
      test_clear_standard();

      // Status
      test_empty_empty();
      test_empty_standard();
      test_size_empty();
      test_size_standard();

      report("BST");
   }
   
   /***************************************
    * CREATE
    *     BST::BST()
    ***************************************/

   // default constructor, no allocations
   void test_construct_default()
   {  // setup
      std::allocator<custom::BST<Spy>> alloc;
      custom::BST<Spy> bst;
      bst.numElements = 99;
      bst.root = (custom::BST<Spy>::BNode *)0xBAADF00D;
      Spy::reset();
      // exercise
      alloc.construct(&bst);  // just call the constructor by itself
      // verify
      assertUnit(Spy::numDefault() == 0);    
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertEmptyFixture(bst);
   }  // teardown

   /***************************************
    * COPY CONSTRUCTOR
    ***************************************/

    // copy an empty BST
   void test_constructCopy_empty()
   {  // setup
      custom::BST<Spy> bstSrc;
      custom::BST<Spy> bstDest;
      std::allocator<custom::BST<Spy>> alloc;
      bstDest.numElements = 99;
      bstDest.root = (custom::BST<Spy>::BNode*)0xBAADF00D;
      Spy::reset();
      // exercise
      alloc.construct(&bstDest, bstSrc);  // just call the constructor by itself
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertEmptyFixture(bstSrc);
      assertEmptyFixture(bstDest);
   }  // teardown

   // copy a BST with a single node
   void test_constructCopy_one()
   {
      // setup
      //            (50b)
      custom::BST <Spy> bstSrc;
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      p50->isRed = false;
      bstSrc.root = p50;
      bstSrc.numElements = 1;
      Spy::reset();
      // exercise
      custom::BST <Spy> bstDest(bstSrc);
      // verify
      assertUnit(Spy::numCopy() == 1);      // copy [50]
      assertUnit(Spy::numAlloc() == 1);     // allocate [50]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(bstSrc.root != bstDest.root);
      //            (50b)
      assertUnit(bstSrc.numElements == 1);
      assertUnit(bstSrc.root == p50);
      assertUnit(bstSrc.root != nullptr);
      if (bstSrc.root)
      {
         assertUnit(bstSrc.root->data == Spy(50));
         assertUnit(bstSrc.root->isRed == false);
         assertUnit(bstSrc.root->pLeft == nullptr);
         assertUnit(bstSrc.root->pRight == nullptr);
         assertUnit(bstSrc.root->pParent == nullptr);
      }
      //            (50b)
      assertUnit(bstDest.numElements == 1);
      assertUnit(bstDest.root != nullptr);
      if (bstDest.root)
      {
         assertUnit(bstDest.root->data == Spy(50));
         assertUnit(bstDest.root->isRed == false);
         assertUnit(bstDest.root->pLeft == nullptr);
         assertUnit(bstDest.root->pRight == nullptr);
         assertUnit(bstDest.root->pParent == nullptr);
      }
      // teardown
      if (bstSrc.root)
         delete bstSrc.root;
      bstSrc.root = nullptr;
      bstSrc.numElements = 0;
      if (bstDest.root)
         delete bstDest.root;
      bstDest.root = nullptr;
      bstDest.numElements = 0;
   }

   // copy the standard fixture
   void test_constructCopy_standard()
   {  // setup
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstSrc;
      setupStandardFixture(bstSrc);
      Spy::reset();
      // exercise
      custom::BST <Spy> bstDest(bstSrc);
      // verify
      assertUnit(Spy::numCopy() == 7);      // copy     [20][30][40][50][60][70][80]
      assertUnit(Spy::numAlloc() == 7);     // allocate [20][30][40][50][60][70][80]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(bstSrc.root != bstDest.root);
      if (bstSrc.root && bstDest.root)
      {
         assertUnit(bstSrc.root->pLeft != bstDest.root->pLeft);
         assertUnit(bstSrc.root->pRight != bstDest.root->pRight);
      }
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(bstSrc);
      assertStandardFixture(bstDest);
      // teardown
      teardownStandardFixture(bstSrc);
      teardownStandardFixture(bstDest);
   }

   /***************************************
    * MOVE CONSTRUCTOR
    ***************************************/

    // move an empty BST
   void test_constructMove_empty()
   {  // setup
      custom::BST<Spy> bstSrc;
      Spy::reset();
      // exercise
      custom::BST<Spy> bstDest(std::move(bstSrc));
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bstSrc);
      assertEmptyFixture(bstDest);
   }  // teardown

   // move a BST with a single node
   void test_constructMove_one()
   {
      // setup
      //            (50b)
      custom::BST <Spy> bstSrc;
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      p50->isRed = false;
      bstSrc.root = p50;
      bstSrc.numElements = 1;
      Spy::reset();
      // exercise
      custom::BST <Spy> bstDest(std::move(bstSrc));
      // verify
      assertUnit(Spy::numCopy() == 0);     
      assertUnit(Spy::numAlloc() == 0);     
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(bstSrc.root != bstDest.root);
      //           
      assertEmptyFixture(bstSrc);
      assertUnit(bstSrc.numElements == 0);
      assertUnit(bstSrc.root == nullptr);
      //            (50b)
      assertUnit(bstDest.numElements == 1);
      assertUnit(bstDest.root == p50);
      assertUnit(bstDest.root != nullptr);
      if (bstDest.root)
      {
         assertUnit(bstDest.root->data == Spy(50));
         assertUnit(bstDest.root->isRed == false);
         assertUnit(bstDest.root->pLeft == nullptr);
         assertUnit(bstDest.root->pRight == nullptr);
         assertUnit(bstDest.root->pParent == nullptr);
      }
      // teardown
      if (bstDest.root)
         delete bstDest.root;
      bstDest.root = nullptr;
      bstDest.numElements = 0;
   }

   // move the standard fixture
   void test_constructMove_standard()
   {  // setup
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstSrc;
      setupStandardFixture(bstSrc);
      Spy::reset();
      // exercise
      custom::BST <Spy> bstDest(std::move(bstSrc));
      // verify
      assertUnit(Spy::numCopy() == 0); 
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bstSrc);
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(bstDest);
      // teardown
      teardownStandardFixture(bstDest);
   }

   // create a BST with an empty initializer list.
   void test_constructInitializer_empty()
   {  // setup
      std::initializer_list<Spy> ilSrc;
      Spy::reset();
      // exercise
      custom::BST<Spy> bstDest(ilSrc);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bstDest);
   }  // teardown
   
   // create a standard fixture from an initializer list
   void test_constructInitializer_standard()
   {  // setup
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      std::initializer_list<Spy> ilSrc{Spy(50), Spy(30), Spy(70), Spy(20), Spy(40), Spy(60), Spy(80)};
      Spy::reset();
      // exercise
      custom::BST <Spy> bstDest(ilSrc);
      // verify
      assertUnit(Spy::numCopy() == 7);
      assertUnit(Spy::numAlloc() == 7);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 10);
      assertStandardFixture(bstDest);
      // teardown
      teardownStandardFixture(bstDest);
   }

   /***************************************
    * DESTRUCTOR
    ***************************************/

    // destroy an already empty fixture
   void test_destruct_empty()
   {  
      {
         // setup
         custom::BST<Spy> bst;
         Spy::reset();
         // exercise
      }

      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
   }  // teardown

   // destroy the standard fixture
   void test_destruct_standard()
   {  // setup
      {
         //                (50b)
         //          +-------+-------+
         //        (30b)           (70b)
         //     +----+----+     +----+----+
         //   (20r)     (40r) (60r)     (80r)
         custom::BST <Spy> bst;
         setupStandardFixture(bst);
         Spy::reset();
         // exercise
      }
      // verify
      assertUnit(Spy::numDestructor() == 7);  // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);      // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
   }  // teardown

   /***************************************
    * EMPTY and SIZE
    ***************************************/

   // is the empty BST empty?
   void test_empty_empty()
   {  // setup
      custom::BST<Spy> bst;
      Spy::reset();
      // exercise
      bool empty = bst.empty();
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(empty == true);
      assertEmptyFixture(bst);
   }  // teardown

   // is the standard fixture empty?
   void test_empty_standard()
   {  // setup
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      Spy::reset();
      // exercise
      bool empty = bst.empty();
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(empty == false);
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // what is the size of an empty BST?
   void test_size_empty()
   {  // setup
      custom::BST<Spy> bst;
      Spy::reset();
      // exercise
      size_t size = bst.size();
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(size == 0);
      assertEmptyFixture(bst);
   }  // teardown

   // what is the size of the standard fixture?
   void test_size_standard()
   {  // setup
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      Spy::reset();
      // exercise
      size_t size = bst.size();
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(size == 7);
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   /***************************************
    * Assignment
    *    BST::operator=(const BST &)
    ***************************************/

   // assignment operator: empty = empty
   void test_assign_emptyToEmpty()
   {  // setup
      custom::BST <Spy> bstSrc;
      custom::BST <Spy> bstDest;
      Spy::reset();
      // exercise
      bstDest = bstSrc;
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bstSrc);
      assertEmptyFixture(bstDest);
   }  // teardown

   // assignment operator: empty = standard
   void test_assign_standardToEmpty()
   {  // setup
      //                (50b) = bstSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstSrc;
      setupStandardFixture(bstSrc);
      custom::BST <Spy> bstDest;
      Spy::reset();
      // exercise
      bstDest = bstSrc;
      // verify
      assertUnit(Spy::numCopy() == 7);      // copy     [20][30][40][50][60][70][80]
      assertUnit(Spy::numAlloc() == 7);     // allocate [20][30][40][50][60][70][80]
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(bstSrc.root != bstDest.root);
      if (bstSrc.root && bstDest.root)
      {
         assertUnit(bstSrc.root->pLeft != bstDest.root->pLeft);
         assertUnit(bstSrc.root->pRight != bstDest.root->pRight);
      }
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(bstSrc);
      assertStandardFixture(bstDest);
      // teardown
      teardownStandardFixture(bstSrc);
      teardownStandardFixture(bstDest);
   }

   // assignment operator: standard = empty
   void test_assign_emptyToStandard()
   {  // setup
      custom::BST <Spy> bstSrc;
      //                (50b) = bstDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstDest;
      setupStandardFixture(bstDest);
      Spy::reset();
      // exercise
      bstDest = bstSrc;
      // verify
      assertUnit(Spy::numDestructor() == 7); // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);     // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bstSrc);
      assertEmptyFixture(bstDest);
   }  // teardown

   // assignment operator : one = standard
   void test_assign_oneToStandard()
   {  // setup
      //                (50b) = bstSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstSrc;
      setupStandardFixture(bstSrc);
      //                (99) = bstDest
      custom::BST <Spy> bstDest;
      custom::BST <Spy>::BNode* p99 = new custom::BST<Spy>::BNode(Spy(99));
      p99->isRed = false;
      bstDest.root = p99;
      bstDest.numElements = 1;
      Spy::reset();
      // exercise
      bstDest = bstSrc;
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign   [50] onto [99]
      assertUnit(Spy::numCopy() == 6);        // copy     [20][30][40]    [60][70][80]
      assertUnit(Spy::numAlloc() == 6);       // allocate [20][30][40]    [60][70][80]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);   
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(bstDest.root != bstSrc.root);
      if (bstSrc.root && bstDest.root)
      {
         assertUnit(bstSrc.root->pLeft != bstDest.root->pLeft);
         assertUnit(bstSrc.root->pRight != bstDest.root->pRight);
      }
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(bstSrc);
      assertStandardFixture(bstDest);
      // teardown
      teardownStandardFixture(bstSrc);
      teardownStandardFixture(bstDest);
   }

   // assignment operator : standard = one
   void test_assign_standardToOne()
   {  // setup
      //                (99) = bstSrc
      custom::BST <Spy> bstSrc;
      custom::BST <Spy>::BNode* p99 = new custom::BST<Spy>::BNode(Spy(99));
      p99->isRed = false;
      bstSrc.root = p99;
      bstSrc.numElements = 1;
      //                (50b) = bstSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstDest;
      setupStandardFixture(bstDest);
      Spy::reset();
      // exercise
      bstDest = bstSrc;
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign   [99] onto [0]
      assertUnit(Spy::numDestructor() == 6);  // destroy  [20][30][40]    [60][70][80]
      assertUnit(Spy::numDelete() == 6);      // delete   [20][30][40]    [60][70][80]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(bstDest.root != bstSrc.root);
      //                (99) = bstSrc
      assertUnit(bstSrc.root != nullptr);
      if (bstSrc.root)
      {
         assertUnit(bstSrc.root->data == Spy(99));
         assertUnit(bstSrc.root->isRed == false);
         assertUnit(bstSrc.root->pParent == nullptr);
         assertUnit(bstSrc.root->pLeft == nullptr);
         assertUnit(bstSrc.root->pRight == nullptr);
      }
      //                (99) = bstDest
      assertUnit(bstDest.root != nullptr);
      if (bstDest.root)
      {
         assertUnit(bstDest.root->data == Spy(99));
         assertUnit(bstDest.root->isRed == false);
         assertUnit(bstDest.root->pParent == nullptr);
         assertUnit(bstDest.root->pLeft == nullptr);
         assertUnit(bstDest.root->pRight == nullptr);
      }
      // teardown
      teardownStandardFixture(bstSrc);
      teardownStandardFixture(bstDest);
   }

   // assignment operator : standard = standard
   void test_assign_standardToStandard()
   {  // setup
      //                (50b) = bstSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstSrc;
      setupStandardFixture(bstSrc);
      //                (50b) = bstDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstDest;
      setupStandardFixture(bstDest);
      Spy::reset();
      // exercise
      bstDest = bstSrc;
      // verify
      assertUnit(Spy::numAssign() == 7);      // assign [2][30][40][50][60][70][80]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);    
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(bstDest.root != bstSrc.root);
      if (bstSrc.root && bstDest.root)
      {
         assertUnit(bstSrc.root->pLeft != bstDest.root->pLeft);
         assertUnit(bstSrc.root->pRight != bstDest.root->pRight);
      }
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(bstSrc);
      assertStandardFixture(bstDest);
      // teardown
      teardownStandardFixture(bstSrc);
      teardownStandardFixture(bstDest);
   }


   /***************************************
    * Assignment-Move
    *    BST::operator=(BST &&)
    ***************************************/

    // assignment-move operator: empty = std::move(empty)
   void test_assignMove_emptyToEmpty()
   {  // setup
      custom::BST <Spy> bstSrc;
      custom::BST <Spy> bstDest;
      Spy::reset();
      // exercise
      bstDest = std::move(bstSrc);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bstSrc);
      assertEmptyFixture(bstDest);
   }  // teardown

   // assignment-move operator: empty = move(standard)
   void test_assignMove_standardToEmpty()
   {  // setup
      //                (50b) = bstSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstSrc;
      setupStandardFixture(bstSrc);
      custom::BST <Spy> bstDest;
      Spy::reset();
      // exercise
      bstDest = std::move(bstSrc);
      // verify
      assertUnit(Spy::numCopy() == 0);  
      assertUnit(Spy::numAlloc() == 0); 
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertStandardFixture(bstDest);
      assertEmptyFixture(bstSrc);
      // teardown
      teardownStandardFixture(bstDest);
   }

   // assignment-move operator: standard = move(empty)
   void test_assignMove_emptyToStandard()
   {  // setup
      custom::BST <Spy> bstSrc;
      //                (50b) = bstDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstDest;
      setupStandardFixture(bstDest);
      Spy::reset();
      // exercise
      bstDest = std::move(bstSrc);
      // verify
      assertUnit(Spy::numDestructor() == 7); // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);     // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bstSrc);
      assertEmptyFixture(bstDest);
   }  // teardown

   // assignment-move operator : one = standard
   void test_assignMove_oneToStandard()
   {  // setup
      //                (50b) = bstSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstSrc;
      setupStandardFixture(bstSrc);
      //                (99) = bstDest
      custom::BST <Spy> bstDest;
      custom::BST <Spy>::BNode* p99 = new custom::BST<Spy>::BNode(Spy(99));
      p99->isRed = false;
      bstDest.root = p99;
      bstDest.numElements = 1;
      Spy::reset();
      // exercise
      bstDest = std::move(bstSrc);
      // verify
      assertUnit(Spy::numDestructor() == 1);  // destroy [99]
      assertUnit(Spy::numDelete() == 1);      // delete  [99]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);  
      assertUnit(Spy::numAlloc() == 0); 
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //                (50b)
      //          +-------+-------+
      //        (30r)           (70r)
      //     +----+----+     +----+----+
      //   (20b)     (40b) (60b)     (80b)
      assertStandardFixture(bstDest);
      assertEmptyFixture(bstSrc);
      // teardown
      teardownStandardFixture(bstDest);
   }

   // assignment operator : standard = one
   void test_assignMove_standardToOne()
   {  // setup
      //                (99) = bstSrc
      custom::BST <Spy> bstSrc;
      custom::BST <Spy>::BNode* p99 = new custom::BST<Spy>::BNode(Spy(99));
      p99->isRed = false;
      bstSrc.root = p99;
      bstSrc.numElements = 1;
      //                (50b) = bstDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstDest;
      setupStandardFixture(bstDest);
      Spy::reset();
      // exercise
      bstDest = std::move(bstSrc);
      // verify
      assertUnit(Spy::numDestructor() == 7);  // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);      // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bstSrc);
      //                (99) = bstDest
      assertUnit(bstDest.root != nullptr);
      if (bstDest.root)
      {
         assertUnit(bstDest.root->data == Spy(99));
         assertUnit(bstDest.root->isRed == false);
         assertUnit(bstDest.root->pParent == nullptr);
         assertUnit(bstDest.root->pLeft == nullptr);
         assertUnit(bstDest.root->pRight == nullptr);
      }
      // teardown
      teardownStandardFixture(bstDest);
   }

   // assignment operator : standard = standard
   void test_assignMove_standardToStandard()
   {  // setup
      //                (50b) = bstSrc
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstSrc;
      setupStandardFixture(bstSrc);
      //                (50b) = bstDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstDest;
      setupStandardFixture(bstDest);
      Spy::reset();
      // exercise
      bstDest = std::move(bstSrc);
      // verify
      assertUnit(Spy::numDestructor() == 7);  // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);      // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(bstDest);
      assertEmptyFixture(bstSrc);
      // teardown
      teardownStandardFixture(bstDest);
   }

   // assign with an empty initializer list.
   void test_assignInitializer_standardToEmpty()
   {  // setup
      std::initializer_list<Spy> ilSrc;
      //                (50b) = bstDest
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bstDest;
      setupStandardFixture(bstDest);
      Spy::reset();
      // exercise
      bstDest = ilSrc;
      // verify
      assertUnit(Spy::numDestructor() == 7);  // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);      // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bstDest);
   }  // teardown

   // create a standard fixture from an initializer list
   void test_assignInitializer_oneToStandard()
   {  // setup
      std::initializer_list<Spy> ilSrc{ Spy(50), Spy(30), Spy(70), Spy(20), Spy(40), Spy(60), Spy(80) };
      //                (99) = bstDest
      custom::BST <Spy> bstDest;
      custom::BST <Spy>::BNode* p99 = new custom::BST<Spy>::BNode(Spy(99));
      p99->isRed = false;
      bstDest.root = p99;
      bstDest.numElements = 1;
      Spy::reset();
      // exercise
      bstDest = ilSrc;
      // verify
      assertUnit(Spy::numCopy() == 7);
      assertUnit(Spy::numAlloc() == 7);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 1);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 10);
      assertStandardFixture(bstDest);
      // teardown
      teardownStandardFixture(bstDest);
   }


   /***************************************
    * Swap
    *    BST::swap(BST &)
    ***************************************/

    // empty.swap(empty)
   void test_swap_emptyToEmpty()
   {  // setup
      custom::BST <Spy> bst1;
      custom::BST <Spy> bst2;
      Spy::reset();
      // exercise
      bst1.swap(bst2);
      // verify
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bst1);
      assertEmptyFixture(bst2);
   }  // teardown

   // swap: standard.swap(empty)
   void test_swap_standardToEmpty()
   {  // setup
      //                (50b) = bst1
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bst1;
      setupStandardFixture(bst1);
      custom::BST <Spy> bst2;
      Spy::reset();
      // exercise
      bst1.swap(bst2);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertStandardFixture(bst2);
      assertEmptyFixture(bst1);
      // teardown
      teardownStandardFixture(bst2);
   }

   // swap: empty.swap(standard)
   void test_swap_emptyToStandard()
   {  // setup
      custom::BST <Spy> bst1;
      //                (50b) = bst2
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bst2;
      setupStandardFixture(bst2);
      Spy::reset();
      // exercise
      bst1.swap(bst2);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertStandardFixture(bst1);
      assertEmptyFixture(bst2);
      // teardown
      teardownStandardFixture(bst1);
   }

   // swap : standard.swap(standard)
   void test_swap_standardToStandard()
   {  // setup
      //                (50b) = bst1
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bst1;
      setupStandardFixture(bst1);
      custom::BST <Spy> :: BNode *pRoot1 = bst1.root;
      //                (50b) = bst2
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bst2;
      setupStandardFixture(bst2);
      custom::BST <Spy> ::BNode* pRoot2 = bst2.root;
      Spy::reset();
      // exercise
      bst1.swap(bst2);
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      assertStandardFixture(bst1);
      assertStandardFixture(bst2);
      assertUnit(pRoot2 = bst1.root);
      assertUnit(pRoot1 = bst2.root);
      // teardown
      teardownStandardFixture(bst1);
      teardownStandardFixture(bst2);
   }

   /***************************************
    * CLEAR
    *    BST::clear()
    ***************************************/

   // clear an already empty fixture
   void test_clear_empty()
   {  // setup
      custom::BST<Spy> bst;
      Spy::reset();
      // exercise
      bst.clear();
      // verify
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bst);
   }  // teardown

   // clear the standard fixture
   void test_clear_standard()
   {  // setup
      //                (50b)
      //          +-------+-------+
      //        (30b)           (70b)
      //     +----+----+     +----+----+
      //   (20r)     (40r) (60r)     (80r)
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      Spy::reset();
      // exercise
      bst.clear();
      // verify
      assertUnit(Spy::numDestructor() == 7);  // destroy  [20][30][40][50][60][70][80]
      assertUnit(Spy::numDelete() == 7);      // delete   [20][30][40][50][60][70][80]
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numLessthan() == 0);
      assertEmptyFixture(bst);
   }  // teardown

   /***************************************
    * Iterator
    *     BST::begin()
    *     BST::end()
    *     BST::iterator::operator++()
    *     BST::iterator::operator*()
    ***************************************/

   // begin() from an empty BST
   void test_begin_empty()
   {  // setup
      custom::BST<Spy> bst;
      custom::BST<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = bst.begin();
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);     
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.pNode == nullptr);
      assertUnit(it == bst.end());
      assertEmptyFixture(bst);
   }  // teardown

   // begin() from the standard fixture
   void test_begin_standard()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = bst.begin();
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.pNode != nullptr);
      assertUnit(bst.root != nullptr && bst.root->pLeft != nullptr);
      if (it.pNode && bst.root && bst.root->pLeft)
         assertUnit(it.pNode == bst.root->pLeft->pLeft);
      if (it.pNode)
         assertUnit(it.pNode->data == Spy(20));
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // end() from the standard fixture.
   void test_end_standard()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      Spy::reset();
      // exercise
      it = bst.end();
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it.pNode == nullptr);
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // increment where the next node is the parent
   void test_iterator_increment_standardToParent()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //  [[20]]      40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = bst.root->pLeft->pLeft;
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //       [[30]]            70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.pNode != nullptr);
      assertUnit(bst.root != nullptr);
      if (bst.root)
         assertUnit(it.pNode == bst.root->pLeft);
      if (it.pNode)
         assertUnit(it.pNode->data == Spy(30));
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }


   // increment where the next node is the right child
   void test_iterator_increment_standardToChild()
   {  // setup
      //                 50 
      //          +-------+-------+
      //       [[30]]            70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = bst.root->pLeft; // 30
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20      [[40]]  60        80  
      assertUnit(it.pNode != nullptr);
      assertUnit(bst.root != nullptr && bst.root->pLeft != nullptr);
      if (bst.root && bst.root->pLeft)
         assertUnit(it.pNode == bst.root->pLeft->pRight);
      if (it.pNode)
         assertUnit(it.pNode->data == Spy(40));
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // increment where the next node is grandma
   void test_iterator_increment_standardToGrandma()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20      [[40]]  60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = bst.root->pLeft->pRight; // 40
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //               [[50]] 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.pNode == bst.root);
      if (it.pNode)
         assertUnit(it.pNode->data == Spy(50));
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // increment where the next node is the right grandchild
   void test_iterator_increment_standardToGrandchild()
   {  // setup
      //               [[50]] 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = bst.root; // 50
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40  [[60]]      80  
      assertUnit(it.pNode != nullptr);
      assertUnit(bst.root != nullptr && bst.root->pRight != nullptr);
      if (bst.root && bst.root->pRight)
         assertUnit(it.pNode == bst.root->pRight->pLeft);
      if (it.pNode)
         assertUnit(it.pNode->data == Spy(60));
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // increment where we are already at the last node
   void test_iterator_increment_standardToDone()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60      [[80]]  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = bst.root->pRight->pRight; // 80
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.pNode == nullptr);
      assertUnit(it == bst.end());
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // increment where we are already at the end
   void test_iterator_increment_standardEnd()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = nullptr; 
      Spy::reset();
      // exercise
      ++it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.pNode == nullptr);
      assertUnit(it == bst.end());
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // decrement where the next node is the parent
   void test_iterator_decrement_standardToParent()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20      [[40]]  60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = bst.root->pLeft->pRight;
      Spy::reset();
      // exercise
      --it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //       [[30]]            70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.pNode != nullptr);
      assertUnit(bst.root != nullptr);
      if (bst.root)
         assertUnit(it.pNode == bst.root->pLeft);
      if (it.pNode)
         assertUnit(it.pNode->data == Spy(30));
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }


   // decrement where the next node is the left child
   void test_iterator_decrement_standardToChild()
   {  // setup
      //                 50 
      //          +-------+-------+
      //       [[30]]            70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = bst.root->pLeft; // 30
      Spy::reset();
      // exercise
      --it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //  [[20]]      40    60        80  
      assertUnit(it.pNode != nullptr);
      assertUnit(bst.root != nullptr && bst.root->pLeft != nullptr);
      if (bst.root && bst.root->pLeft)
         assertUnit(it.pNode == bst.root->pLeft->pLeft);
      if (it.pNode)
         assertUnit(it.pNode->data == Spy(20));
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // decrement where the next node is grandma
   void test_iterator_decrement_standardToGrandma()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40  [[60]]      80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = bst.root->pRight->pLeft; // 60
      Spy::reset();
      // exercise
      --it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //               [[50]] 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.pNode == bst.root);
      if (it.pNode)
         assertUnit(it.pNode->data == Spy(50));
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // decrement where the next node is the right grandchild
   void test_iterator_decrement_standardToGrandchild()
   {  // setup
      //               [[50]] 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = bst.root; // 50
      Spy::reset();
      // exercise
      --it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20      [[40]]  60        80  
      assertUnit(it.pNode != nullptr);
      assertUnit(bst.root != nullptr && bst.root->pLeft != nullptr);
      if (bst.root && bst.root->pLeft)
         assertUnit(it.pNode == bst.root->pLeft->pRight);
      if (it.pNode)
         assertUnit(it.pNode->data == Spy(40));
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // decrement where we are already at the last node
   void test_iterator_decrement_standardToDone()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //  [[20]]      40    60        80    
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = bst.root->pLeft->pLeft; // 20
      Spy::reset();
      // exercise
      --it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.pNode == nullptr);
      assertUnit(it == bst.end());
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // increment where we are already at the end
   void test_iterator_decrement_standardEnd()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = nullptr;
      Spy::reset();
      // exercise
      --it;
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertUnit(it.pNode == nullptr);
      assertUnit(it == bst.end());
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // itereator dereference were we just read
   void test_iterator_dereference_standardRead()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20      [[40]]  60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      it.pNode = bst.root->pLeft->pRight;
      Spy s(99);
      Spy::reset();
      // exercise
      s = *it;
      // verify
      assertUnit(Spy::numAssign() == 1);      // assign the results to s
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(s == Spy(40));
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   /***************************************
    * Find
    *    BST::find(const T &)
    ***************************************/

   // attempt to find something in an empty BST
   void test_find_empty()
   {  // setup
      custom::BST<Spy> bst;
      custom::BST<Spy>::iterator it;
      Spy s(50);
      Spy::reset();
      // exercise
      it = bst.find(s);
      // verify
      assertUnit(Spy::numLessthan() == 0);    // does not look at any element
      assertUnit(Spy::numEquals() == 0);      // does not look at any element
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it == bst.end());
      assertEmptyFixture(bst);
   }  // teardown
   
   // attemp to find something where it is at the beginning
   void test_find_standardBegin()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      Spy s(20);
      Spy::reset();
      // exercise
      it = bst.find(s);
      // verify
      assertUnit(Spy::numEquals() == 3);      // check [50][30][20]
      assertUnit(Spy::numLessthan() == 2);    // compare [50][30]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //  [[20]]      40    60        80  
      assertUnit(it.pNode != nullptr);
      if (it.pNode)
         assertUnit(*it == Spy(20));
      assertUnit(bst.root != nullptr && bst.root->pLeft != nullptr);
      if (bst.root && bst.root->pLeft)
         assertUnit(it.pNode == bst.root->pLeft->pLeft);
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }
   
   // attempt to find something where it is the last element
   void test_find_standardLast()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      Spy s(80);
      Spy::reset();
      // exercise
      it = bst.find(s);
      // verify
      assertUnit(Spy::numEquals() == 3);      // check [50][70][80]
      assertUnit(Spy::numLessthan() == 2);    // compare [50][70]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60      [[80]]
      assertUnit(it.pNode != nullptr);
      if (it.pNode)
         assertUnit(*it == Spy(80));
      assertUnit(bst.root != nullptr && bst.root->pRight != nullptr);
      if (bst.root && bst.root->pRight)
         assertUnit(it.pNode == bst.root->pRight->pRight);
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }

   // attempt to find something where it is not there
   void test_find_standardMissing()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      custom::BST<Spy>::iterator it;
      Spy s(42);
      Spy::reset();
      // exercise
      it = bst.find(s);
      // verify
      assertUnit(Spy::numEquals() == 3);      // check [50][30][40]
      assertUnit(Spy::numLessthan() == 3);    // compare [50][30][40]
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(it == bst.end());
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }



   /***************************************
    * Insert
    *    BST::insert(const T &)
    ***************************************/

   // insert an element to the right of a single-element tree
   void test_insert_oneRight()
   {  // setup
      //            (50b)
      custom::BST <Spy> bst;
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      p50->isRed = false;
      bst.root = p50;
      bst.numElements = 1;
      Spy s(60);
      Spy::reset();
      // exercise
      auto pairBST = bst.insert(s);
      // verify
      assertUnit(Spy::numLessthan() == 1);    // compare [50]
      assertUnit(Spy::numCopy() == 1);        // copy-create [60]
      assertUnit(Spy::numAlloc() == 1);       // allocate [60]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(pairBST.first != bst.end());
      assertUnit(pairBST.second == true);
      if (pairBST.first != bst.end())
         assertUnit(*(pairBST.first) == Spy(60));
      //           (50b)
      //             +----+
      //                 (60r)
      assertUnit(bst.size() == 2);
      assertUnit(bst.numElements == 2);
      assertUnit(bst.root == p50);

      assertUnit(p50 != nullptr);
      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft == nullptr);
         assertUnit(p50->pRight != nullptr);
         assertUnit(p50->pParent == nullptr);
      }
      if (p50 && p50->pRight)
      {
         assertUnit(p50->pRight->data == Spy(60));
         assertUnit(p50->pRight->isRed == true);
         assertUnit(p50->pRight->pLeft == nullptr);
         assertUnit(p50->pRight->pRight == nullptr);
         assertUnit(p50->pRight->pParent == p50);
      }
      // teardown
      if (p50 && p50->pRight && p50->pRight != p50)
         delete p50->pRight;
      if (p50)
         delete p50;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // insert an element to the left of a single-element tree
   void test_insert_oneLeft()
   {  // setup
      //            (50b)
      custom::BST <Spy> bst;
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      p50->isRed = false;
      bst.root = p50;
      bst.numElements = 1;
      Spy s(40);
      Spy::reset();
      // exercise
      auto pairBST = bst.insert(s);
      // verify
      assertUnit(Spy::numLessthan() == 1);    // compare [50]
      assertUnit(Spy::numCopy() == 1);        // copy-create [40]
      assertUnit(Spy::numAlloc() == 1);       // allocate [40]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(pairBST.first != bst.end());
      assertUnit(pairBST.second == true);
      if (pairBST.first != bst.end())
         assertUnit(*(pairBST.first) == Spy(40));

      //           (50b)
      //        +----+
      //      (40r)
      assertUnit(bst.size() == 2);
      assertUnit(bst.numElements == 2);
      assertUnit(bst.root == p50);

      assertUnit(p50 != nullptr);
      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft != nullptr);
         assertUnit(p50->pRight == nullptr);
         assertUnit(p50->pParent == nullptr);
      }

      if (p50 && p50->pLeft)
      {
         assertUnit(p50->pLeft->data == Spy(40));
         assertUnit(p50->pLeft->isRed == true);
         assertUnit(p50->pLeft->pLeft == nullptr);
         assertUnit(p50->pLeft->pRight == nullptr);
         assertUnit(p50->pLeft->pParent == p50);
      }

      // teardown
      if (p50 && p50->pLeft && p50->pLeft != p50)
         delete p50->pLeft;
      if (p50)
         delete p50;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // insert a duplicate item in a BST
   void test_insert_duplicate()
   {  // setup
      //            (50b)
      custom::BST <Spy> bst;
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      p50->isRed = false;
      bst.root = p50;
      bst.numElements = 1;
      Spy s(50);
      Spy::reset();
      // exercise
      auto pairBST = bst.insert(s, false /* keepUnique */);
      // verify
      assertUnit(Spy::numLessthan() == 1);    // compare [50]
      assertUnit(Spy::numCopy() == 1);        // copy-create [50]
      assertUnit(Spy::numAlloc() == 1);       // allocate [50]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(pairBST.first != bst.end());
      assertUnit(pairBST.second == true);
      if (pairBST.first != bst.end())
         assertUnit(*(pairBST.first) == Spy(50));
      //           (50b)
      //             +----+
      //                 (50r)
      assertUnit(bst.size() == 2);
      assertUnit(bst.numElements == 2);
      assertUnit(bst.root == p50);

      assertUnit(p50 != nullptr);
      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft == nullptr);
         assertUnit(p50->pRight != nullptr);
         assertUnit(p50->pParent == nullptr);
      }

      if (p50 && p50->pRight)
      {
         assertUnit(p50->pRight->data == Spy(50));
         assertUnit(p50->pRight->isRed == true);
         assertUnit(p50->pRight->pLeft == nullptr);
         assertUnit(p50->pRight->pRight == nullptr);
         assertUnit(p50->pRight->pParent == p50);
      }

      // teardown
      if (p50 && p50->pRight && p50->pRight != p50)
         delete p50->pRight;
      if (p50)
         delete p50;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // insert an item when it already exists
   void test_insert_keepUnique()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      Spy s(40);
      Spy::reset();
      // exercise
      auto pairBST = bst.insert(s, true /* keepUnique */);
      // verify
      assertUnit(Spy::numLessthan() == 2);    // compare [50][30]
      assertUnit(Spy::numEquals() == 3);      // compare [50][30][40]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(pairBST.first != bst.end());
      assertUnit(pairBST.second == false);
      if (pairBST.first != bst.end())
         assertUnit(*(pairBST.first) == Spy(40));
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }


   /***************************************
    * Insert Move
    *    BST::insert(T &&)
    ***************************************/

    // insert an element to the right of a single-element tree
   void test_insertMove_oneRight()
   {  // setup
      //            (50b)
      custom::BST <Spy> bst;
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      p50->isRed = false;
      bst.root = p50;
      bst.numElements = 1;
      Spy s(60);
      Spy::reset();
      // exercise
      auto pairBST = bst.insert(std::move(s));
      // verify
      assertUnit(Spy::numLessthan() == 1);    // compare [50]
      assertUnit(Spy::numCopyMove() == 1);    // assign move [60]
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(pairBST.first != bst.end());
      assertUnit(pairBST.second == true);
      if (pairBST.first != bst.end())
         assertUnit(*(pairBST.first) == Spy(60));
      //           (50b)
      //             +----+
      //                 (60r)
      assertUnit(bst.size() == 2);
      assertUnit(bst.numElements == 2);
      assertUnit(bst.root == p50);

      assertUnit(p50 != nullptr);
      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft == nullptr);
         assertUnit(p50->pRight != nullptr);
         assertUnit(p50->pParent == nullptr);
      }
      if (p50 && p50->pRight)
      {
         assertUnit(p50->pRight->data == Spy(60));
         assertUnit(p50->pRight->isRed == true);
         assertUnit(p50->pRight->pLeft == nullptr);
         assertUnit(p50->pRight->pRight == nullptr);
         assertUnit(p50->pRight->pParent == p50);
      }
      // teardown
      if (p50 && p50->pRight && p50->pRight != p50)
         delete p50->pRight;
      if (p50)
         delete p50;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // insert an element to the left of a single-element tree
   void test_insertMove_oneLeft()
   {  // setup
      //            (50b)
      custom::BST <Spy> bst;
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      p50->isRed = false;
      bst.root = p50;
      bst.numElements = 1;
      Spy s(40);
      Spy::reset();
      // exercise
      auto pairBST = bst.insert(std::move(s));
      // verify
      assertUnit(Spy::numLessthan() == 1);    // compare [50]
      assertUnit(Spy::numCopyMove() == 1);    // assign move [40]
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);   
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(s.empty() == true);
      assertUnit(pairBST.first != bst.end());
      assertUnit(pairBST.second == true);
      if (pairBST.first != bst.end())
         assertUnit(*(pairBST.first) == Spy(40));

      //           (50b)
      //        +----+
      //      (40r)
      assertUnit(bst.size() == 2);
      assertUnit(bst.numElements == 2);
      assertUnit(bst.root == p50);

      assertUnit(p50 != nullptr);
      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft != nullptr);
         assertUnit(p50->pRight == nullptr);
         assertUnit(p50->pParent == nullptr);
      }

      if (p50 && p50->pLeft)
      {
         assertUnit(p50->pLeft->data == Spy(40));
         assertUnit(p50->pLeft->isRed == true);
         assertUnit(p50->pLeft->pLeft == nullptr);
         assertUnit(p50->pLeft->pRight == nullptr);
         assertUnit(p50->pLeft->pParent == p50);
      }

      // teardown
      if (p50 && p50->pLeft && p50->pLeft != p50)
         delete p50->pLeft;
      if (p50)
         delete p50;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // insert a duplicate item in a BST
   void test_insertMove_duplicate()
   {  // setup
      //            (50b)
      custom::BST <Spy> bst;
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      p50->isRed = false;
      bst.root = p50;
      bst.numElements = 1;
      Spy s(50);
      Spy::reset();
      // exercise
      auto pairBST = bst.insert(std::move(s), false /* keepUnique */);
      // verify
      assertUnit(Spy::numLessthan() == 1);    // compare [50]
      assertUnit(Spy::numCopyMove() == 1);    // assign move [50]
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(s.empty() == true);
      assertUnit(pairBST.first != bst.end());
      assertUnit(pairBST.second == true);
      if (pairBST.first != bst.end())
         assertUnit(*(pairBST.first) == Spy(50));
      //           (50b)
      //             +----+
      //                 (50r)
      assertUnit(bst.size() == 2);
      assertUnit(bst.numElements == 2);
      assertUnit(bst.root == p50);

      assertUnit(p50 != nullptr);
      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft == nullptr);
         assertUnit(p50->pRight != nullptr);
         assertUnit(p50->pParent == nullptr);
      }

      if (p50 && p50->pRight)
      {
         assertUnit(p50->pRight->data == Spy(50));
         assertUnit(p50->pRight->isRed == true);
         assertUnit(p50->pRight->pLeft == nullptr);
         assertUnit(p50->pRight->pRight == nullptr);
         assertUnit(p50->pRight->pParent == p50);
      }

      // teardown
      if (p50 && p50->pRight && p50->pRight != p50)
         delete p50->pRight;
      if (p50)
         delete p50;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // insert an item when it already exists
   void test_insertMove_keepUnique()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      Spy s(40);
      Spy::reset();
      // exercise
      auto pairBST = bst.insert(std::move(s), true /* keepUnique */);
      // verify
      assertUnit(Spy::numLessthan() == 2);    // compare [50][30]
      assertUnit(Spy::numEquals() == 3);      // compare [50][30][40]
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(s.empty() == false);
      assertUnit(pairBST.first != bst.end());
      assertUnit(pairBST.second == false);
      if (pairBST.first != bst.end())
         assertUnit(*(pairBST.first) == Spy(40));
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }


   /***************************************
    * Insert Balancing
    ***************************************/
   
   // Red/Black balancing - Case 1
   void test_insert_case1()
   {  // setup
      custom::BST <Spy> bst;
      Spy s(50);
      Spy::reset();
      // exercise
      bst.insert(s);
      // verify
      assertUnit(Spy::numCopy() == 1);        // copy-create [50]
      assertUnit(Spy::numAlloc() == 1);       // allocate [50]
      assertUnit(Spy::numLessthan() == 0); 
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //            (50b)
      assertUnit(bst.root != nullptr);
      assertUnit(bst.numElements == 1);

      if (bst.root)
      { 
         assertUnit(bst.root->data == Spy(50));
         assertUnit(bst.root->isRed == false);
         assertUnit(bst.root->pLeft == nullptr);
         assertUnit(bst.root->pRight == nullptr);
         assertUnit(bst.root->pParent == nullptr);
      }
      // teardown
      delete bst.root;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // Red/Black balancing - Case 2
   void test_insert_case2()
   {  // setup
      //            (50b)
      custom::BST <Spy> bst;
      custom::BST<Spy>::BNode *p50 = new custom::BST<Spy>::BNode(Spy(50));
      p50->isRed = false;
      bst.root = p50;
      bst.numElements = 1;
      Spy s(30);
      Spy::reset();
      // exercise
      bst.insert(s);
      // verify
      assertUnit(Spy::numLessthan() == 1);    // compare [50]
      assertUnit(Spy::numCopy() == 1);        // copy-create [30]
      assertUnit(Spy::numAlloc() == 1);       // allocate [30]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //              (50b)
      //            +---+
      //          (30r)
      assertUnit(bst.empty() == false);
      assertUnit(bst.size() == 2);        
      assertUnit(bst.root == p50);
      assertUnit(bst.numElements == 2);

      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft != nullptr);
         assertUnit(p50->pRight == nullptr);
         assertUnit(p50->pParent == nullptr);
      }

      if (p50 && p50->pLeft)
      {
         assertUnit(p50->pLeft->isRed == true);
         assertUnit(p50->pLeft->data == Spy(30));
         assertUnit(p50->pLeft->pLeft == nullptr);
         assertUnit(p50->pLeft->pLeft == nullptr);
         assertUnit(p50->pLeft->pParent == bst.root);
      }

      // teardown
      if (p50 && p50->pLeft && p50->pLeft != p50)
         delete p50->pLeft;
      if (p50)
         delete p50;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // Red/Black balancing - Case 3
   void test_insert_case3()
   {  // setup
      //           (50b)
      //        +----+----+
      //      (30r)     (70r)
      custom::BST<Spy>::BNode* p30 = new custom::BST<Spy>::BNode(Spy(30));
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      custom::BST<Spy>::BNode* p70 = new custom::BST<Spy>::BNode(Spy(70));

      p50->pLeft  = p30;
      p50->pRight = p70;
      p70->pParent = p30->pParent = p50;

      p50->isRed = false;
      p30->isRed = p70->isRed = true;

      custom::BST <Spy> bst;
      bst.root = p50;
      bst.numElements = 3;

      Spy s(20);
      Spy::reset();

      // exercise
      bst.insert(s);

      // verify
      assertUnit(Spy::numLessthan() == 2);    // compare [50][30]
      assertUnit(Spy::numCopy() == 1);        // copy-create [20]
      assertUnit(Spy::numAlloc() == 1);       // allocate [20]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //              (50b)
      //           +----+----+
      //         (30b)     (70b)
      //       +---+
      //     (20r)
      assertUnit(bst.empty() == false);
      assertUnit(bst.size() == 4);
      assertUnit(bst.root == p50);
      assertUnit(bst.numElements == 4);

      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft == p30);
         assertUnit(p50->pRight == p70);
         assertUnit(p50->pParent == nullptr);
      }

      if (p30)
      {
         assertUnit(p30->data == Spy(30));
         assertUnit(p30->isRed == false);
         assertUnit(p30->pLeft != nullptr);
         assertUnit(p30->pRight == nullptr);
         assertUnit(p30->pParent == p50);
      }

      if (p70)
      {
         assertUnit(p70->data == Spy(70));
         assertUnit(p70->isRed == false);
         assertUnit(p70->pLeft == nullptr);
         assertUnit(p70->pRight == nullptr);
         assertUnit(p70->pParent == p50);
      }

      if (p30 && p30->pLeft)
      {
         assertUnit(p30->pLeft->data == Spy(20));
         assertUnit(p30->pLeft->isRed == true);
         assertUnit(p30->pLeft->pLeft == nullptr);
         assertUnit(p30->pLeft->pRight == nullptr);
         assertUnit(p30->pLeft->pParent == p30);
      }
      // teardown
      if (p30->pLeft && p30->pLeft != p30)
         delete p30->pLeft;
      if (p30)
         delete p30;
      if (p70)
         delete p70;
      if (p50)
         delete p50;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // Red/Black balancing - Case 4a Simplest case
   void test_insert_case4aSimple()
   {  // setup
      //              (50b)
      //           +----+
      //         (30r)    
      custom::BST<Spy>::BNode* p30 = new custom::BST<Spy>::BNode(Spy(30));
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));

      p50->pLeft   = p30;
      p30->pParent = p50;

      p50->isRed = false;
      p30->isRed = true;

      custom::BST <Spy> bst;
      bst.root = p50;
      bst.numElements = 2;

      Spy s(10);
      Spy::reset();

      // exercise
      bst.insert(s);

      // verify
      assertUnit(Spy::numLessthan() == 2);    // compare [50][30]
      assertUnit(Spy::numCopy() == 1);        // copy-create [10]
      assertUnit(Spy::numAlloc() == 1);       // allocate [10]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //              (30b)
      //           +----+----+
      //         (10r)     (50r)
      assertUnit(bst.empty() == false);
      assertUnit(bst.size() == 3);
      assertUnit(bst.root == p30);
      assertUnit(bst.numElements == 3);

      if (p30)
      {
         assertUnit(p30->data == Spy(30));
         assertUnit(p30->isRed == false);
         assertUnit(p30->pLeft != nullptr);
         assertUnit(p30->pRight == p50);
         assertUnit(p30->pParent == nullptr);
      }

      if (p30 && p30->pLeft)
      {
         assertUnit(p30->pLeft->data == Spy(10));
         assertUnit(p30->pLeft->isRed == true);
         assertUnit(p30->pLeft->pLeft == nullptr);
         assertUnit(p30->pLeft->pRight == nullptr);
         assertUnit(p30->pLeft->pParent == p30);
      }

      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == true);
         assertUnit(p50->pLeft == nullptr);
         assertUnit(p50->pRight == nullptr);
         assertUnit(p50->pParent == p30);
      }

      // teardown
      if (p30 && p30->pLeft && p30->pLeft != p30)
         delete p30->pLeft;
      if (p50)
         delete p50;
      if (p30)
         delete p30;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // Red/Black balancing - Case 4b Simplest case
   void test_insert_case4bSimple()
   {  // setup
      //              (50b)
      //                +----+
      //                   (70r)    
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      custom::BST<Spy>::BNode* p70 = new custom::BST<Spy>::BNode(Spy(70));

      p50->pRight = p70;
      p70->pParent = p50;

      p50->isRed = false;
      p70->isRed = true;

      custom::BST <Spy> bst;
      bst.root = p50;
      bst.numElements = 2;

      Spy s(90);
      Spy::reset();

      // exercise
      bst.insert(s);

      // verify
      assertUnit(Spy::numLessthan() == 2);    // compare [50][70]
      assertUnit(Spy::numCopy() == 1);        // copy-create [90]
      assertUnit(Spy::numAlloc() == 1);       // allocate [90]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //              (70b)
      //           +----+----+
      //         (50r)     (90r)
      assertUnit(bst.empty() == false);
      assertUnit(bst.size() == 3);
      assertUnit(bst.root == p70);
      assertUnit(bst.numElements == 3);

      if (p70)
      {
         assertUnit(p70->data == Spy(70));
         assertUnit(p70->isRed == false);
         assertUnit(p70->pLeft == p50);
         assertUnit(p70->pRight != nullptr);
         assertUnit(p70->pParent == nullptr);
      }

      if (p70->pRight)
      {
         assertUnit(p70->pRight->data == Spy(90));
         assertUnit(p70->pRight->isRed == true);
         assertUnit(p70->pRight->pLeft == nullptr);
         assertUnit(p70->pRight->pRight == nullptr);
         assertUnit(p70->pRight->pParent == p70);
      }

      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == true);
         assertUnit(p50->pLeft == nullptr);
         assertUnit(p50->pRight == nullptr);
         assertUnit(p50->pParent == p70);
      }
      
      // teardown
      if (p70->pRight && p70->pRight != p70)
         delete p70->pRight;
      if (p50)
         delete p50;
      if (p70)
         delete p70;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // Red/Black balancing - Case 4c Simplest case
   void test_insert_case4cSimple()
   {  // setup
      //                   (50b)
      //           +---------+
      //         (30r)     
      custom::BST<Spy>::BNode* p30 = new custom::BST<Spy>::BNode(Spy(30));
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));

      p50->pLeft = p30;
      p30->pParent = p50;

      p30->isRed = true;
      p50->isRed = false;

      custom::BST <Spy> bst;
      bst.root = p50;
      bst.numElements = 2;

      Spy s(40);
      Spy::reset();

      // exercise
      bst.insert(s);

      // verify
      assertUnit(Spy::numLessthan() == 2);    // compare [50][30]
      assertUnit(Spy::numCopy() == 1);        // copy-create [40]
      assertUnit(Spy::numAlloc() == 1);       // allocate [40]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                   (40b)
      //           +---------+---------+
      //         (30r)               (50r)
      assertUnit(bst.empty() == false);
      assertUnit(bst.size() == 3);
      assertUnit(bst.root != nullptr);
      assertUnit(bst.numElements == 3);

      if (bst.root)
      {
         assertUnit(bst.root->data == Spy(40));
         assertUnit(bst.root->isRed == false);
         assertUnit(bst.root->pLeft == p30);
         assertUnit(bst.root->pRight == p50);
         assertUnit(bst.root->pParent == nullptr);
      }

      if (p30)
      {
         assertUnit(p30->data == Spy(30));
         assertUnit(p30->isRed == true);
         assertUnit(p30->pLeft == nullptr);
         assertUnit(p30->pRight == nullptr);
         assertUnit(p30->pParent == bst.root);
      }

      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == true);
         assertUnit(p50->pLeft == nullptr);
         assertUnit(p50->pRight == nullptr);
         assertUnit(p50->pParent == bst.root);
      }

      // teardown
      if (p50)
         delete p50;
      if (p30)
         delete p30;
      bst.numElements = 0;
      bst.root = nullptr;
   }

   // Red/Black balancing - Case 4d Simplest Case
   void test_insert_case4dSimple()
   {  // setup
      //         (50b)
      //           +---------+
      //                   (70r)     
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      custom::BST<Spy>::BNode* p70 = new custom::BST<Spy>::BNode(Spy(70));

      p50->pRight = p70;
      p70->pParent = p50;

      p70->isRed = true;
      p50->isRed = false;

      custom::BST <Spy> bst;
      bst.root = p50;
      bst.numElements = 2;

      Spy s(60);
      Spy::reset();

      // exercise
      bst.insert(s);

      // verify
      assertUnit(Spy::numLessthan() == 2);    // compare [50][70]
      assertUnit(Spy::numCopy() == 1);        // copy-create [60]
      assertUnit(Spy::numAlloc() == 1);       // allocate [60]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                   (60b)
      //           +---------+---------+
      //         (50r)               (70r)
      assertUnit(bst.empty() == false);
      assertUnit(bst.size() == 3);
      assertUnit(bst.root != nullptr);
      assertUnit(bst.numElements == 3);

      if (bst.root)
      {
         assertUnit(bst.root->data == Spy(60));
         assertUnit(bst.root->isRed == false);
         assertUnit(bst.root->pLeft == p50);
         assertUnit(bst.root->pRight == p70);
         assertUnit(bst.root->pParent == nullptr);
      }

      if (p50)
      {

         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == true);
         assertUnit(p50->pLeft == nullptr);
         assertUnit(p50->pRight == nullptr);
         assertUnit(p50->pParent == bst.root);
      }

      if (p70)
      {
         assertUnit(p70->data == Spy(70));
         assertUnit(p70->isRed == true);
         assertUnit(p70->pLeft == nullptr);
         assertUnit(p70->pRight == nullptr);
         assertUnit(p70->pParent == bst.root);
      }
      // teardown
      if (p50)
         delete p50;
      if (p70)
         delete p70;
      bst.numElements = 0;
      bst.root = nullptr;
   }

   // Red/Black balancing - Case 4a Complex
   void test_insert_case4aComplex()
   {  // setup
      //              (50b)
      //           +----+----+
      //         (30b)     (70b)
      //       +---+
      //     (20r)    
      custom::BST<Spy>::BNode* p20 = new custom::BST<Spy>::BNode(Spy(20));
      custom::BST<Spy>::BNode* p30 = new custom::BST<Spy>::BNode(Spy(30));
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      custom::BST<Spy>::BNode* p70 = new custom::BST<Spy>::BNode(Spy(70));

      p50->pLeft  = p30;
      p50->pRight = p70;
      p30->pLeft  = p20;
      p70->pParent = p30->pParent = p50;
      p20->pParent = p30;

      p20->isRed = true;
      p30->isRed = p70->isRed = p50->isRed = false;

      custom::BST <Spy> bst;
      bst.root = p50;
      bst.numElements = 4;

      Spy s(10);
      Spy::reset();

      // exercise
      bst.insert(s);

      // verify
      assertUnit(Spy::numLessthan() == 3);    // compare [50][30][20]
      assertUnit(Spy::numCopy() == 1);        // copy-create [10]
      assertUnit(Spy::numAlloc() == 1);       // allocate [10]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //              (50b)
      //           +----+----+
      //         (20b)     (70b)
      //       +---+---+
      //     (10r)   (30r)
      assertUnit(bst.empty() == false);
      assertUnit(bst.size() == 5);
      assertUnit(bst.root == p50);
      assertUnit(bst.numElements == 5);

      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft == p20);
         assertUnit(p50->pRight == p70);
         assertUnit(p50->pParent == nullptr);
      }

      if (p20)
      {
         assertUnit(p20->data == Spy(20));
         assertUnit(p20->isRed == false);
         assertUnit(p20->pLeft != nullptr);
         assertUnit(p20->pRight == p30);
         assertUnit(p20->pParent == p50);
      }

      if (p70)
      {
         assertUnit(p70->data == Spy(70));
         assertUnit(p70->isRed == false);
         assertUnit(p70->pLeft == nullptr);
         assertUnit(p70->pRight == nullptr);
         assertUnit(p70->pParent == p50);
      }

      if (p30)
      {
         assertUnit(p30->data == Spy(30));
         assertUnit(p30->isRed == true);
         assertUnit(p30->pLeft == nullptr);
         assertUnit(p30->pRight == nullptr);
         assertUnit(p30->pParent == p20);
      }

      if (p20 && p20->pLeft)
      {
         assertUnit(p20->pLeft->data == Spy(10));
         assertUnit(p20->pLeft->isRed == true);
         assertUnit(p20->pLeft->pLeft == nullptr);
         assertUnit(p20->pLeft->pRight == nullptr);
         assertUnit(p20->pLeft->pParent == p20);
      }

      // teardown
      if (p20 && p20->pLeft && p20->pLeft != p20)
        delete p20->pLeft;
      if (p30)
         delete p30;
      if (p70)
         delete p70;
      if (p20)
         delete p20;
      if (p50)
         delete p50;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // Red/Black balancing - Case 4b
   void test_insert_case4bComplex()
   {  // setup
      //              (50b)
      //           +----+----+
      //         (30b)     (70b)
      //                     +---+
      //                       (80r)    
      custom::BST<Spy>::BNode* p30 = new custom::BST<Spy>::BNode(Spy(30));
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      custom::BST<Spy>::BNode* p70 = new custom::BST<Spy>::BNode(Spy(70));
      custom::BST<Spy>::BNode* p80 = new custom::BST<Spy>::BNode(Spy(80));

      p50->pLeft = p30;
      p50->pRight = p70;
      p70->pRight = p80;
      p70->pParent = p30->pParent = p50;
      p80->pParent = p70;

      p80->isRed = true;
      p30->isRed = p70->isRed = p50->isRed = false;

      custom::BST <Spy> bst;
      bst.root = p50;
      bst.numElements = 4;

      Spy s(90);
      Spy::reset();

      // exercise
      bst.insert(s);

      // verify
      assertUnit(Spy::numLessthan() == 3);    // compare [50][70][80]
      assertUnit(Spy::numCopy() == 1);        // copy-create [90]
      assertUnit(Spy::numAlloc() == 1);       // allocate [90]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //              (50b)
      //           +----+----+
      //         (30b)     (80b)
      //                 +---+---+
      //               (70r)   (90r)
      assertUnit(bst.empty() == false);
      assertUnit(bst.size() == 5);
      assertUnit(bst.root == p50);
      assertUnit(bst.numElements == 5);

      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft == p30);
         assertUnit(p50->pRight == p80);
         assertUnit(p50->pParent == nullptr);
      }

      if (p30)
      {
         assertUnit(p30->data == Spy(30));
         assertUnit(p30->isRed == false);
         assertUnit(p30->pLeft == nullptr);
         assertUnit(p30->pRight == nullptr);
         assertUnit(p30->pParent == p50);
      }

      if (p80)
      {
         assertUnit(p80->data == Spy(80));
         assertUnit(p80->isRed == false);
         assertUnit(p80->pLeft == p70);
         assertUnit(p80->pRight != nullptr);
         assertUnit(p80->pParent == p50);
      }

      if (p70)
      {
         assertUnit(p70->data == Spy(70));
         assertUnit(p70->isRed == true);
         assertUnit(p70->pLeft == nullptr);
         assertUnit(p70->pRight == nullptr);
         assertUnit(p70->pParent == p80);
      }

      if (p80 && p80->pRight)
      {
         assertUnit(p80->pRight->data == Spy(90));
         assertUnit(p80->pRight->isRed == true);
         assertUnit(p80->pRight->pLeft == nullptr);
         assertUnit(p80->pRight->pRight == nullptr);
         assertUnit(p80->pRight->pParent == p80);
      }

      // teardown
      if (p80 && p80->pRight && p80->pRight != p80)
         delete p80->pRight;
      if (p70)
         delete p70;
      if (p30)
         delete p30;
      if (p80)
         delete p80;
      if (p50)
         delete p50;
      bst.root = nullptr;
      bst.numElements = 0;
   }

   // Red/Black balancing - Case 4c Complex case
   void test_insert_case4cComplex()
   {  // setup
      //                   (70b)
      //           +---------+---------+
      //         (20r)               (80b)  
      //     +-----+-----+
      //   (10b)       (50b)
      //            +----+----+
      //          (30r)     (60r)
      custom::BST<Spy>::BNode* p10 = new custom::BST<Spy>::BNode(Spy(10));
      custom::BST<Spy>::BNode* p20 = new custom::BST<Spy>::BNode(Spy(20));
      custom::BST<Spy>::BNode* p30 = new custom::BST<Spy>::BNode(Spy(30));
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      custom::BST<Spy>::BNode* p60 = new custom::BST<Spy>::BNode(Spy(60));
      custom::BST<Spy>::BNode* p70 = new custom::BST<Spy>::BNode(Spy(70));
      custom::BST<Spy>::BNode* p80 = new custom::BST<Spy>::BNode(Spy(80));

      p20->pLeft  = p10;
      p20->pRight = p50;
      p50->pLeft  = p30;
      p50->pRight = p60;
      p70->pLeft  = p20;
      p70->pRight = p80;
      p30->pParent = p60->pParent = p50;
      p10->pParent = p50->pParent = p20;
      p20->pParent = p80->pParent = p70;

      p20->isRed = p30->isRed = p60->isRed = true;
      p10->isRed = p50->isRed = p70->isRed = p80->isRed = false;

      custom::BST <Spy> bst;
      bst.root = p70;
      bst.numElements = 7;

      Spy s(40);
      Spy::reset();

      // exercise
      bst.insert(s);

      // verify
      assertUnit(Spy::numLessthan() == 4);    // compare [70][20][50][30]
      assertUnit(Spy::numCopy() == 1);        // copy-create [40]
      assertUnit(Spy::numAlloc() == 1);       // allocate [40]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                   (50b)
      //           +---------+---------+
      //         (20r)               (70r)  
      //     +-----+-----+       +-----+-----+
      //   (10b)       (30b)   (60b)       (80b)
      //                 +--+
      //                  (40r)
      assertUnit(bst.empty() == false);
      assertUnit(bst.size() == 8);
      assertUnit(bst.root == p50);
      assertUnit(bst.numElements == 8);

      if (p10)
      {
         assertUnit(p10->data == Spy(10));
         assertUnit(p10->isRed == false);
         assertUnit(p10->pLeft == nullptr);
         assertUnit(p10->pRight == nullptr);
         assertUnit(p10->pParent == p20);
      }

      if (p20)
      {
         assertUnit(p20->data == Spy(20));
         assertUnit(p20->isRed == true);
         assertUnit(p20->pLeft == p10);
         assertUnit(p20->pRight == p30);
         assertUnit(p20->pParent == p50);
      }

      if (p30)
      {
         assertUnit(p30->data == Spy(30));
         assertUnit(p30->isRed == false);
         assertUnit(p30->pLeft == nullptr);
         assertUnit(p30->pRight != nullptr);
         assertUnit(p30->pParent == p20);
      }

      if (p30 && p30->pRight)
      {
         assertUnit(p30->pRight->data == Spy(40));
         assertUnit(p30->pRight->isRed == true);
         assertUnit(p30->pRight->pLeft == nullptr);
         assertUnit(p30->pRight->pRight == nullptr);
         assertUnit(p30->pRight->pParent == p30);
      }

      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft == p20);
         assertUnit(p50->pRight == p70);
         assertUnit(p50->pParent == nullptr);
      }

      if (p60)
      {
         assertUnit(p60->data == Spy(60));
         assertUnit(p60->isRed == false);
         assertUnit(p60->pLeft == nullptr);
         assertUnit(p60->pRight == nullptr);
         assertUnit(p60->pParent == p70);
      }

      if (p70)
      {
         assertUnit(p70->data == Spy(70));
         assertUnit(p70->isRed == true);
         assertUnit(p70->pLeft == p60);
         assertUnit(p70->pRight == p80);
         assertUnit(p70->pParent == p50);
      }

      if (p80)
      {
         assertUnit(p80->data == Spy(80));
         assertUnit(p80->isRed == false);
         assertUnit(p80->pLeft == nullptr);
         assertUnit(p80->pRight == nullptr);
         assertUnit(p80->pParent == p70);
      }
      // teardown
      if (p30 && p30->pRight && p30->pRight != p30)
         delete p30->pRight;
      if (p10)
         delete p10;
      if (p20)
         delete p20;
      if (p30)
         delete p30;
      if (p50)
         delete p50;
      if (p60)
         delete p60;
      if (p70)
         delete p70;
      if (p80)
         delete p80;
      bst.numElements = 0;
      bst.root = nullptr;
   }

   // Red/Black balancing - Case 4d Complex Case
   void test_insert_case4dComplex()
   {  // setup
      //                   (30b)
      //           +---------+---------+
      //         (20b)               (80r)  
      //                         +-----+-----+
      //                       (50b)       (90b)
      //                    +----+----+
      //                  (40r)     (70r)
      custom::BST<Spy>::BNode* p20 = new custom::BST<Spy>::BNode(Spy(20));
      custom::BST<Spy>::BNode* p30 = new custom::BST<Spy>::BNode(Spy(30));
      custom::BST<Spy>::BNode* p40 = new custom::BST<Spy>::BNode(Spy(40));
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      custom::BST<Spy>::BNode* p70 = new custom::BST<Spy>::BNode(Spy(70));
      custom::BST<Spy>::BNode* p80 = new custom::BST<Spy>::BNode(Spy(80));
      custom::BST<Spy>::BNode* p90 = new custom::BST<Spy>::BNode(Spy(90));

      p30->pLeft  = p20;
      p30->pRight = p80;
      p50->pLeft  = p40;
      p50->pRight = p70;
      p80->pLeft  = p50;
      p80->pRight = p90;
      p40->pParent = p70->pParent = p50;
      p50->pParent = p90->pParent = p80;
      p20->pParent = p80->pParent = p30;

      p40->isRed = p70->isRed = p80->isRed = true;
      p20->isRed = p30->isRed = p50->isRed = p90->isRed = false;

      custom::BST <Spy> bst;
      bst.root = p30;
      bst.numElements = 7;

      Spy s(60);
      Spy::reset();

      // exercise
      bst.insert(s);

      // verify
      assertUnit(Spy::numLessthan() == 4);    // compare [30][80][50][70]
      assertUnit(Spy::numCopy() == 1);        // copy-create [60]
      assertUnit(Spy::numAlloc() == 1);       // allocate [60]
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                   (50b)
      //           +---------+---------+
      //         (30r)               (80r)  
      //     +-----+-----+       +-----+-----+
      //   (20b)       (40b)   (70b)       (90b)
      //                      +--+
      //                    (60r)
      assertUnit(bst.empty() == false);
      assertUnit(bst.size() == 8);
      assertUnit(bst.root == p50);
      assertUnit(bst.numElements == 8);

      if (p20)
      {
         assertUnit(p20->data == Spy(20));
         assertUnit(p20->isRed == false);
         assertUnit(p20->pLeft == nullptr);
         assertUnit(p20->pRight == nullptr);
         assertUnit(p20->pParent == p30);
      }

      if (p30)
      {
         assertUnit(p30->data == Spy(30));
         assertUnit(p30->isRed == true);
         assertUnit(p30->pLeft == p20);
         assertUnit(p30->pRight == p40);
         assertUnit(p30->pParent == p50);
      }

      if (p40)
      {
         assertUnit(p40->data == Spy(40));
         assertUnit(p40->isRed == false);
         assertUnit(p40->pLeft == nullptr);
         assertUnit(p40->pRight == nullptr);
         assertUnit(p40->pParent == p30);
      }

      if (p50)
      {
         assertUnit(p50->data == Spy(50));
         assertUnit(p50->isRed == false);
         assertUnit(p50->pLeft == p30);
         assertUnit(p50->pRight == p80);
         assertUnit(p50->pParent == nullptr);
      }

      if (p70 && p70->pLeft)
      {
         assertUnit(p70->pLeft->data == Spy(60));
         assertUnit(p70->pLeft->isRed == true);
         assertUnit(p70->pLeft->pLeft == nullptr);
         assertUnit(p70->pLeft->pRight == nullptr);
         assertUnit(p70->pLeft->pParent == p70);
      }

      if (p70)
      {
         assertUnit(p70->data == Spy(70));
         assertUnit(p70->isRed == false);
         assertUnit(p70->pLeft != nullptr);
         assertUnit(p70->pRight == nullptr);
         assertUnit(p70->pParent == p80);
      }

      if (p80)
      {
         assertUnit(p80->data == Spy(80));
         assertUnit(p80->isRed == true);
         assertUnit(p80->pLeft == p70);
         assertUnit(p80->pRight == p90);
         assertUnit(p80->pParent == p50);
      }

      if (p90)
      {
         assertUnit(p90->data == Spy(90));
         assertUnit(p90->isRed == false);
         assertUnit(p90->pLeft == nullptr);
         assertUnit(p90->pRight == nullptr);
         assertUnit(p90->pParent == p80);
      }

      // teardown
      if (p70 && p70->pLeft && p70->pLeft != p70)
         delete p70->pLeft;
      if (p20)
         delete p20;
      if (p30)
         delete p30;
      if (p40)
         delete p40;
      if (p50)
         delete p50;
      if (p70)
         delete p70;
      if (p80)
         delete p80;
      if (p90)
         delete p90;
      bst.numElements = 0;
      bst.root = nullptr;
   }

   /***************************************
    * Erase
    *    BST::erase(it)
    ***************************************/

   // erase from an empty BST
   void test_erase_empty()
   {  // setup
      custom::BST <Spy> bst;
      auto it = bst.end();
      Spy::reset();
      // exercise
      auto itReturn = bst.erase(it);
      // verify
      assertUnit(Spy::numLessthan() == 0);   
      assertUnit(Spy::numCopy() == 0);       
      assertUnit(Spy::numAlloc() == 0);      
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(itReturn == bst.end());
      assertEmptyFixture(bst);
   }  // teardown

   // erase with an empty iterator
   void test_erase_standardMissing()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      auto it = bst.end();
      Spy::reset();
      // exercise
      auto itReturn = bst.erase(it);
      // verify
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numDestructor() == 0);
      assertUnit(Spy::numDelete() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      assertUnit(itReturn == bst.end());
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }  // teardown

   // remove a leaf node from the standard fixture
   void test_erase_noChildren()
   {  // setup
      //                 50 
      //          +-------+-------+
      //         30              70  
      //     +----+----+     +----+----+
      //    20        40  [[60]]      80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      auto it = custom::BST <Spy> :: iterator(bst.root->pRight->pLeft);
      Spy::reset();
      // exercise
      auto itReturn = bst.erase(it);
      // verify
      assertUnit(Spy::numDestructor() == 1);  // destroy [60]
      assertUnit(Spy::numDelete() == 1);      // delete [60]
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);
      //                 50 
      //          +-------+-------+
      //         30            [[70]]
      //     +----+----+          +----+
      //    20        40              80  
      assertUnit(itReturn == custom::BST <Spy> ::iterator(bst.root->pRight));
      assertUnit(bst.root->pRight->pLeft == nullptr);
      assertUnit(bst.numElements == 6);
      bst.root->pRight->pLeft = new custom::BST<Spy>::BNode(Spy(60));
      bst.root->pRight->pLeft->pParent = bst.root->pRight;
      bst.numElements = 7;
      assertStandardFixture(bst);
      // teardown
      teardownStandardFixture(bst);
   }  // teardown
   
   void test_erase_oneChild()
   {  // setup
      //                 50 
      //          +-------+-------+
      //       [[10]]            60
      //          +----+     
      //              30          
      //            +--+--+
      //           20    40
      custom :: BST <int> bst;
      auto p10 = new custom::BST<int>::BNode(10);
      auto p20 = new custom::BST<int>::BNode(20);
      auto p30 = new custom::BST<int>::BNode(30);
      auto p40 = new custom::BST<int>::BNode(40);
      auto p60 = new custom::BST<int>::BNode(60);
      auto p50 = new custom::BST<int>::BNode(50);
      bst.root = p10->pParent = p60->pParent = p50;
      p50->pLeft = p30->pParent = p10;
      p50->pRight = p60;
      p10->pRight = p20->pParent = p40->pParent = p30;
      p30->pLeft = p20;
      p30->pRight = p40;
      bst.numElements = 6;
      auto it = custom::BST <int> :: iterator(p10);
      // exercise
      auto itReturn = bst.erase(it);
      // verify
      //                 50
      //          +-------+-------+
      //         30              60
      //     +----+----+
      //  [[20]]      40
      assertUnit(itReturn == custom::BST <int> ::iterator(p20));
      assertUnit(bst.numElements == 5);
      assertUnit(bst.root == p50);
      assertUnit(p50->pLeft == p30);
      assertUnit(p50->pRight == p60);
      assertUnit(p30->pLeft == p20);
      assertUnit(p30->pRight = p40);
      assertUnit(p30->pParent == p50);
      assertUnit(p20->pParent == p30);
      assertUnit(p40->pParent == p30);
      assertUnit(p60->pParent == p50);
      assertUnit(p20->data == 20);
      assertUnit(p30->data == 30);
      assertUnit(p40->data == 40);
      assertUnit(p50->data == 50);
      assertUnit(p60->data == 60);
      // teardown
      delete p20;
      delete p30;
      delete p40;
      delete p50;
      delete p60;
      bst.numElements = 0;
      bst.root = nullptr;
   }

   void test_erase_twoChildren()
   {  // setup
      //                 70 
      //          +-------+-------+
      //       [[20]]            80
      //     +----+----+     
      //    10        50          
      //            +--+--+
      //           30    60
      //            +-+
      //              40 
      custom::BST <int> bst;
      auto p10 = new custom::BST<int>::BNode(10);
      auto p20 = new custom::BST<int>::BNode(20);
      auto p30 = new custom::BST<int>::BNode(30);
      auto p40 = new custom::BST<int>::BNode(40);
      auto p50 = new custom::BST<int>::BNode(50);
      auto p60 = new custom::BST<int>::BNode(60);
      auto p70 = new custom::BST<int>::BNode(70);
      auto p80 = new custom::BST<int>::BNode(80);
      bst.root = p20->pParent = p80->pParent = p70;
      p10->pParent = p50->pParent = p70->pLeft = p20;
      p70->pRight = p80;
      p20->pLeft = p10;
      p20->pRight = p30->pParent = p60->pParent = p50;
      p50->pLeft = p40->pParent = p30;
      p50->pRight = p60;
      p30->pRight = p40;
      bst.numElements = 8;
      auto it = custom::BST <int> ::iterator(p20);
      // exercise
      auto itReturn = bst.erase(it);
      // verify
      //                 70
      //          +-------+-------+
      //       [[30]]            80
      //     +----+----+     
      //    10        50    
      //            +--+--+
      //           40    60
      assertUnit(itReturn == custom::BST <int> ::iterator(p30));
      assertUnit(bst.root == p70);
      assertUnit(bst.numElements == 7);
      assertUnit(p70->pLeft == p30);
      assertUnit(p70->pRight == p80);
      assertUnit(p30->pLeft == p10);
      assertUnit(p30->pRight = p50);
      assertUnit(p50->pLeft == p40);
      assertUnit(p50->pRight = p60);
      assertUnit(p30->pParent == p70);
      assertUnit(p80->pParent == p70);
      assertUnit(p10->pParent == p30);
      assertUnit(p50->pParent == p30);
      assertUnit(p40->pParent == p50);
      assertUnit(p60->pParent == p50);
      assertUnit(p10->data == 10);
      assertUnit(p30->data == 30);
      assertUnit(p40->data == 40);
      assertUnit(p50->data == 50);
      assertUnit(p60->data == 60);
      assertUnit(p70->data == 70);
      assertUnit(p80->data == 80);
      // teardown
      delete p10;
      delete p30;
      delete p40;
      delete p50;
      delete p60;
      delete p70;
      delete p80;
      bst.numElements = 0;
      bst.root = nullptr;
   }

   void test_erase_twoChildrenSpecial()
   {  // setup
      //                 50 
      //          +-------+-------+
      //       [[30]]            70  
      //     +----+----+     +----+----+
      //    20        40    60        80  
      custom::BST <Spy> bst;
      setupStandardFixture(bst);
      auto it = custom::BST <Spy> ::iterator(bst.root->pLeft);
      Spy::reset();
      // exercise
      auto itReturn = bst.erase(it);
      // verify
      //                 50  
      //          +-------+-------+
      //       [[40]]            70  
      //     +----+          +----+----+
      //    20              60        80  
      assertUnit(Spy::numDestructor() == 1);  // destroy [30]
      assertUnit(Spy::numDelete() == 1);      // delete [30]
      assertUnit(Spy::numLessthan() == 0);
      assertUnit(Spy::numCopy() == 0);
      assertUnit(Spy::numAlloc() == 0);
      assertUnit(Spy::numEquals() == 0);
      assertUnit(Spy::numAssign() == 0);
      assertUnit(Spy::numDefault() == 0);
      assertUnit(Spy::numNondefault() == 0);
      assertUnit(Spy::numCopyMove() == 0);
      assertUnit(Spy::numAssignMove() == 0);

      assertUnit(bst.numElements == 6);
      assertUnit(bst.root != nullptr);
      if (bst.root)
      {
         assertUnit(itReturn.pNode == bst.root->pLeft);
         assertUnit(bst.root->data == Spy(50));
         assertUnit(bst.root->pParent == nullptr);
         assertUnit(bst.root->pLeft != nullptr);
         if (bst.root->pLeft)
         {
            assertUnit(bst.root->pLeft->data == Spy(40));
            assertUnit(bst.root->pLeft->pParent == bst.root);
            assertUnit(bst.root->pLeft->pLeft != nullptr);
            if (bst.root->pLeft->pLeft)
            {
               assertUnit(bst.root->pLeft->pLeft->data == Spy(20));
               assertUnit(bst.root->pLeft->pLeft->pParent == bst.root->pLeft);
               assertUnit(bst.root->pLeft->pLeft->pLeft == nullptr);
               assertUnit(bst.root->pLeft->pLeft->pRight == nullptr);
            }
            assertUnit(bst.root->pLeft->pRight == nullptr);
         }
         assertUnit(bst.root->pRight != nullptr);
         if (bst.root->pRight)
         {
            assertUnit(bst.root->pRight->data == Spy(70));
            assertUnit(bst.root->pRight->pParent == bst.root);
            assertUnit(bst.root->pRight->pLeft != nullptr);
            if (bst.root->pRight->pLeft)
            {
               assertUnit(bst.root->pRight->pLeft->data == Spy(60));
               assertUnit(bst.root->pRight->pLeft->pParent == bst.root->pRight);
               assertUnit(bst.root->pRight->pLeft->pLeft == nullptr);
               assertUnit(bst.root->pRight->pLeft->pRight == nullptr);
            }
            assertUnit(bst.root->pRight->pRight != nullptr);
            if (bst.root->pRight->pRight)
            {
               assertUnit(bst.root->pRight->pRight->data == Spy(80));
               assertUnit(bst.root->pRight->pRight->pParent == bst.root->pRight);
               assertUnit(bst.root->pRight->pRight->pLeft == nullptr);
               assertUnit(bst.root->pRight->pRight->pRight == nullptr);
            }
         }
      }
      // teardown
      teardownStandardFixture(bst);
   }


   /**************************************************************
    * SETUP STANDARD FIXTURE
    *                (50b)
    *          +-------+-------+
    *        (30b)           (70b)
    *     +----+----+     +----+----+
    *   (20r)     (40r) (60r)     (80r)
    *************************************************************/
   void setupStandardFixture(custom::BST <Spy>& bst)
   {
      // make sure that bst is clean
      assertUnit(bst.numElements == 0);
      assertUnit(bst.root == nullptr);

      // allocate
      custom::BST<Spy>::BNode* p20 = new custom::BST<Spy>::BNode(Spy(20));
      custom::BST<Spy>::BNode* p30 = new custom::BST<Spy>::BNode(Spy(30));
      custom::BST<Spy>::BNode* p40 = new custom::BST<Spy>::BNode(Spy(40));
      custom::BST<Spy>::BNode* p50 = new custom::BST<Spy>::BNode(Spy(50));
      custom::BST<Spy>::BNode* p60 = new custom::BST<Spy>::BNode(Spy(60));
      custom::BST<Spy>::BNode* p70 = new custom::BST<Spy>::BNode(Spy(70));
      custom::BST<Spy>::BNode* p80 = new custom::BST<Spy>::BNode(Spy(80));

      // hook up the pointers down
      p30->pLeft  = p20;
      p30->pRight = p40;
      p50->pLeft  = p30;
      p50->pRight = p70;
      p70->pLeft  = p60;
      p70->pRight = p80;

      // hook up the pointers up
      p20->pParent = p40->pParent = p30;
      p30->pParent = p70->pParent = p50;
      p60->pParent = p80->pParent = p70;

      // color everything
      p50->isRed = p30->isRed = p70->isRed = false;
      p20->isRed = p40->isRed = p60->isRed = p80->isRed = true;

      // now assign everything to the bst
      bst.root = p50;
      bst.numElements = 7;
   }

   /**************************************************************
    * VERIFY EMPTY FIXTURE
    *************************************************************/
   void assertEmptyFixtureParameters(const custom::BST <Spy>& bst, int line, const char* function)
   {
      assertUnit(bst.root == nullptr);
      assertUnit(bst.numElements == 0);
   }

   /**************************************************************
    * VERIFY STANDARD FIXTURE
    *                (50b)
    *          +-------+-------+
    *        (30b)           (70b)
    *     +----+----+     +----+----+
    *   (20r)     (40r) (60r)     (80r)
    *************************************************************/
   void assertStandardFixtureParameters(const custom::BST <Spy>& bst, int line, const char* function)
   {
      // verify the member variables
      assertIndirect(bst.numElements == 7);
      assertIndirect(bst.root != nullptr);

      // verify the pointers down
      assertIndirect(bst.root != nullptr);
      if (bst.root)
      {
         assertIndirect(bst.root->data == Spy(50));
         assertIndirect(bst.root->isRed == false);
         assertIndirect(bst.root->pParent == nullptr);
         assertIndirect(bst.root->pLeft != nullptr);
         if (bst.root->pLeft)
         {
            assertIndirect(bst.root->pLeft->data == Spy(30));
            assertIndirect(bst.root->pLeft->isRed == false);
            assertIndirect(bst.root->pLeft->pParent == bst.root);
            assertIndirect(bst.root->pLeft->pLeft != nullptr);
            if (bst.root->pLeft->pLeft)
            {
               assertIndirect(bst.root->pLeft->pLeft->data == Spy(20));
               assertIndirect(bst.root->pLeft->pLeft->isRed == true);
               assertIndirect(bst.root->pLeft->pLeft->pParent == bst.root->pLeft);
               assertIndirect(bst.root->pLeft->pLeft->pLeft == nullptr);
               assertIndirect(bst.root->pLeft->pLeft->pRight == nullptr);
            }
            assertIndirect(bst.root->pLeft->pRight != nullptr);
            if (bst.root->pLeft->pRight)
            {
               assertIndirect(bst.root->pLeft->pRight->data == Spy(40));
               assertIndirect(bst.root->pLeft->pRight->isRed == true);
               assertIndirect(bst.root->pLeft->pRight->pParent == bst.root->pLeft);
               assertIndirect(bst.root->pLeft->pRight->pLeft == nullptr);
               assertIndirect(bst.root->pLeft->pRight->pRight == nullptr);
            }
         }
         assertIndirect(bst.root->pRight != nullptr);
         if (bst.root->pRight)
         {
            assertIndirect(bst.root->pRight->data == Spy(70));
            assertIndirect(bst.root->pRight->isRed == false);
            assertIndirect(bst.root->pRight->pParent == bst.root);
            assertIndirect(bst.root->pRight->pLeft != nullptr);
            if (bst.root->pRight->pLeft)
            {
               assertIndirect(bst.root->pRight->pLeft->data == Spy(60));
               assertIndirect(bst.root->pRight->pLeft->isRed == true);
               assertIndirect(bst.root->pRight->pLeft->pParent == bst.root->pRight);
               assertIndirect(bst.root->pRight->pLeft->pLeft == nullptr);
               assertIndirect(bst.root->pRight->pLeft->pRight == nullptr);
            }
            assertIndirect(bst.root->pRight->pRight != nullptr);
            if (bst.root->pRight->pRight)
            {
               assertIndirect(bst.root->pRight->pRight->data == Spy(80));
               assertIndirect(bst.root->pRight->pRight->isRed == true);
               assertIndirect(bst.root->pRight->pRight->pParent == bst.root->pRight);
               assertIndirect(bst.root->pRight->pRight->pLeft == nullptr);
               assertIndirect(bst.root->pRight->pRight->pRight == nullptr);
            }
         }
      }
   }


   /**************************************************************
    * TEARDOWN STANDARD FIXTURE
    *                 ( )
    *          +-------+-------+
    *         ( )             ( )
    *     +----+----+     +----+----+
    *    ( )       ( )   ( )       ( )
    *************************************************************/
   void teardownStandardFixture(custom::BST <Spy>& bst)
   {
      if (bst.root)
      {
         if (bst.root->pLeft && bst.root->pLeft != bst.root)
         {
            if (bst.root->pLeft->pLeft && bst.root->pLeft->pLeft != bst.root->pLeft && bst.root->pLeft->pLeft != bst.root)
               delete bst.root->pLeft->pLeft;
            if (bst.root->pLeft->pRight && bst.root->pLeft->pRight != bst.root->pLeft && bst.root->pLeft->pRight != bst.root)
               delete bst.root->pLeft->pRight;
            delete bst.root->pLeft;
         }
         if (bst.root->pRight && bst.root->pRight != bst.root)
         {
            if (bst.root->pRight->pLeft && bst.root->pRight->pLeft != bst.root->pRight && bst.root->pRight->pLeft != bst.root)
               delete bst.root->pRight->pLeft;
            if (bst.root->pRight->pRight && bst.root->pRight->pRight != bst.root->pRight && bst.root->pRight->pRight != bst.root)
               delete bst.root->pRight->pRight;

            delete bst.root->pRight;
         }
         delete bst.root;
      }
      bst.root = nullptr;
      bst.numElements = 0;
   }

  
};

#endif // DEBUG
