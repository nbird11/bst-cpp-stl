/***********************************************************************
 * Header:
 *    BST
 * Summary:
 *    Our custom implementation of a BST for set and for map
 *      __      __     _______        __
 *     /  |    /  |   |  _____|   _  / /
 *     `| |    `| |   | |____    (_)/ /
 *      | |     | |   '_.____''.   / / _
 *     _| |_   _| |_  | \____) |  / / (_)
 *    |_____| |_____|  \______.' /_/
 *
 *    This will contain the class definition of:
 *        BST                 : A class that represents a binary search tree
 *        BST::iterator       : An iterator through BST
 * Author
 *    Nathan Bird, Brock Hoskins
 ************************************************************************/

#pragma once

#ifdef DEBUG
#define debug(x) x
#else // !DEBUG
#define debug(x)
#endif // !DEBUG

#include <cassert>
#include <utility>
#include <memory>     // for std::allocator
#include <functional> // for std::less
#include <utility>    // for std::pair

class TestBST; // forward declaration for unit tests
class TestSet;
class TestMap;

namespace custom
{

   template <typename TT>
   class set;
   template <typename KK, typename VV>
   class map;

/*****************************************************************
 * BINARY SEARCH TREE
 * Create a Binary Search Tree
 *****************************************************************/
   template <typename T>
   class BST
   {
      friend class ::TestBST; // give unit tests access to private members
      friend class ::TestSet;
      friend class ::TestMap;

      template <class TT>
      friend class custom::set;

      template <class KK, class VV>
      friend class custom::map;
   public:
      //
      // Construct
      //

      BST();
      BST(const BST& rhs);
      BST(BST&& rhs);
      BST(const std::initializer_list<T>& il);
      ~BST();

      //
      // Assign
      //

      BST& operator =(const BST& rhs);
      BST& operator =(BST&& rhs);
      BST& operator =(const std::initializer_list<T>& il);
      void swap(BST& rhs);

      //
      // Iterator
      //

      class iterator;
      iterator begin() const noexcept;
      iterator end()   const noexcept { return iterator(nullptr); }

      //
      // Access
      //

      iterator find(const T& t);

      // 
      // Insert
      //

      std::pair<iterator, bool> insert(const T& t, bool keepUnique = false);
      std::pair<iterator, bool> insert(T&& t, bool keepUnique = false);

      //
      // Remove
      // 

      iterator erase(iterator& it);
      void     clear() noexcept;

      // 
      // Status
      //

      bool   empty() const noexcept { return size() == 0; }
      size_t size()  const noexcept { return numElements; }

   private:

      class  BNode;
      BNode* root;              // root node of the binary search tree
      size_t numElements;       // number of elements currently in the tree


      // Recursive clear helper function.
      void clear(BST<T>::BNode*& pNode) noexcept;
   };


   /*****************************************************************
    * BINARY NODE
    * A single node in a binary tree. Note that the node does not know
    * anything about the properties of the tree so no validation can be done.
    *****************************************************************/
   template <typename T>
   class BST<T>::BNode
   {
   public:
      // 
      // Construct
      //
      BNode() : data(T()), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true)
      {}
      BNode(const T& t) : data(t), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true)
      {}
      BNode(T&& t) : data(std::move(t)), pLeft(nullptr), pRight(nullptr), pParent(nullptr), isRed(true)
      {}

      //
      // Insert
      //
      void addLeft (BNode* pNode);
      void addRight(BNode* pNode);
      void addLeft (const T& t);
      void addRight(const T& t);
      void addLeft (T&& t);
      void addRight(T&& t);

      // 
      // Status
      //
      bool isRightChild(BNode* pNode) const { return pParent == pNode && pNode->pRight == this; }
      bool isLeftChild (BNode* pNode) const { return pParent == pNode && pNode->pLeft  == this; }

      // balance the tree
      void balance();

   #ifdef DEBUG
      //
      // Verify
      //
      std::pair<T, T> verifyBTree() const;
      int findDepth() const;
      bool verifyRedBlack(int depth) const;
      int computeSize() const;
   #endif // DEBUG

      //
      // Data
      //
      T data;                  // Actual data stored in the BNode
      BNode* pLeft;            // Left child - smaller
      BNode* pRight;           // Right child - larger
      BNode* pParent;          // Parent
      bool isRed;              // Red-black balancing stuff
   };

   /**********************************************************
    * BINARY SEARCH TREE ITERATOR
    * Forward and reverse iterator through a BST
    *********************************************************/
   template <typename T>
   class BST<T>::iterator
   {
      friend class ::TestBST; // give unit tests access to the privates
      friend class ::TestSet;
      friend class ::TestMap;

      template <class KK, class VV>
      friend class custom::map;
   public:
      // constructors and assignment
      iterator(BNode* p = nullptr)
      {}
      iterator(const iterator& rhs)
      {}
      iterator& operator =(const iterator& rhs)
      {
         return *this;
      }

      // compare
      bool operator ==(const iterator& rhs) const
      {
         return true;
      }
      bool operator !=(const iterator& rhs) const
      {
         return true;
      }

      // de-reference. Cannot change because it will invalidate the BST
      const T& operator *() const
      {
         return *(new T);
      }

      // increment and decrement
      iterator& operator ++();
      iterator  operator ++(int postfix)
      {
         return *this;
      }
      iterator& operator --();
      iterator  operator --(int postfix)
      {
         return *this;;
      }

      // must give friend status to remove so it can call getNode() from it
      friend BST<T>::iterator BST<T>::erase(iterator& it);

   private:

       // the node
      BNode* pNode;
   };


   /*********************************************
    *********************************************
    *********************************************
    ******************** BST ********************
    *********************************************
    *********************************************
    *********************************************/


    /*********************************************
     * BST :: DEFAULT CONSTRUCTOR
     ********************************************/
   template <typename T>
   BST<T>::BST() : numElements(0), root(nullptr) {}

   /*********************************************
    * BST :: COPY CONSTRUCTOR
    * Copy one tree to another
    ********************************************/
   template <typename T>
   BST<T>::BST (const BST<T>& rhs)
   {
      numElements = 99;
      root = new BNode;
   }

   /*********************************************
    * BST :: MOVE CONSTRUCTOR
    * Move one tree to another
    ********************************************/
   template <typename T>
   BST<T>::BST(BST<T>&& rhs)
   {
      numElements = 99;
      root = new BNode;
   }

   /*********************************************
    * BST :: INITIALIZER LIST CONSTRUCTOR
    * Create a BST from an initializer list
    ********************************************/
   template <typename T>
   BST<T>::BST(const std::initializer_list<T>& il)
   {
      numElements = 99;
      root = new BNode;
   }

   /*********************************************
    * BST :: DESTRUCTOR
    ********************************************/
   template <typename T>
   BST<T>:: ~BST()
   {

   }


   /*********************************************
    * BST :: ASSIGNMENT OPERATOR
    * Copy one tree to another
    ********************************************/
   template <typename T>
   BST<T>& BST<T>:: operator = (const BST<T>& rhs)
   {
      return *this;
   }

   /*********************************************
    * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
    * Copy nodes onto a BTree
    ********************************************/
   template <typename T>
   BST<T>& BST<T>:: operator = (const std::initializer_list<T>& il)
   {
      return *this;
   }

   /*********************************************
    * BST :: ASSIGN-MOVE OPERATOR
    * Move one tree to another
    ********************************************/
   template <typename T>
   BST<T>& BST<T>:: operator = (BST<T>&& rhs)
   {
      return *this;
   }

   /*********************************************
    * BST :: SWAP
    * Swap two trees
    ********************************************/
   template <typename T>
   void BST<T>::swap (BST<T>& rhs)
   {

   }

   /*****************************************************
    * BST :: INSERT
    * Insert a node at a given location in the tree
    ****************************************************/
   template <typename T>
   std::pair<typename BST<T>::iterator, bool> BST<T>::insert(const T& t, bool keepUnique)
   {
      std::pair<iterator, bool> pairReturn(end(), false);
      return pairReturn;
   }

   template <typename T>
   std::pair<typename BST<T>::iterator, bool> BST<T>::insert(T&& t, bool keepUnique)
   {
      std::pair<iterator, bool> pairReturn(end(), false);
      return pairReturn;
   }

   /*************************************************
    * BST :: ERASE
    * Remove a given node as specified by the iterator
    ************************************************/
   template <typename T>
   typename BST<T>::iterator BST<T>::erase(iterator& it)
   {
      return end();
   }

   /*****************************************************
    * BST :: CLEAR
    * Removes all the BNodes from a tree
    ****************************************************/
   template <typename T>
   void BST<T>::clear() noexcept
   {
      clear(root);
      numElements = 0;
   }

   /*****************************************************
   * BST :: CLEAR_RECURSIVE
   * Removes all the BNodes from a tree
   ****************************************************/
   template <typename T>
   void BST<T>::clear(BST<T>::BNode*& pNode) noexcept
   {
      if (!pNode)
         return;

      clear(pNode->pLeft);
      clear(pNode->pRight);

      delete pNode;
      pNode = nullptr;
   }

   /*****************************************************
    * BST :: BEGIN
    * Return the first node (left-most) in a binary search tree
    ****************************************************/
   template <typename T>
   typename BST<T>::iterator custom::BST<T>::begin() const noexcept
   {
      return end();
   }


   /****************************************************
    * BST :: FIND
    * Return the node corresponding to a given value
    ****************************************************/
   template <typename T>
   typename BST<T>::iterator BST<T>::find(const T& t)
   {
      return end();
   }

   /******************************************************
    ******************************************************
    ******************************************************
    *********************** B NODE ***********************
    ******************************************************
    ******************************************************
    ******************************************************/


   /******************************************************
    * BINARY NODE :: ADD LEFT
    * Add a node to the left of the current node
    ******************************************************/
   template <typename T>
   void BST<T>::BNode::addLeft (BNode* pNode)
   {
      if (pNode)
         pNode->pParent = this;
      pNode->pLeft = pLeft;

   }

   /******************************************************
    * BINARY NODE :: ADD RIGHT
    * Add a node to the right of the current node
    ******************************************************/
   template <typename T>
   void BST<T>::BNode::addRight (BNode* pNode)
   {
      if (pNode)
         pNode->pParent = this;
      pNode->pRight = pRight;
   }

   /******************************************************
    * BINARY NODE :: ADD LEFT
    * Add a node to the left of the current node
    ******************************************************/
   template <typename T>
   void BST<T>::BNode::addLeft (const T& t)
   {
      addLeft(new BNode(t));
   }

   /******************************************************
    * BINARY NODE :: ADD LEFT
    * Add a node to the left of the current node
    ******************************************************/
   template <typename T>
   void BST<T>::BNode::addLeft(T&& t)
   {
      addLeft(new BNode(std::move(t)));
   }

   /******************************************************
    * BINARY NODE :: ADD RIGHT
    * Add a node to the right of the current node
    ******************************************************/
   template <typename T>
   void BST<T>::BNode::addRight(const T& t)
   {
      addRight(new BNode(t));
   }

   /******************************************************
    * BINARY NODE :: ADD RIGHT
    * Add a node to the right of the current node
    ******************************************************/
   template <typename T>
   void BST<T>::BNode::addRight(T&& t)
   {
      addRight(new BNode(std::move(t)));
   }

#ifdef DEBUG
/****************************************************
 * BINARY NODE :: FIND DEPTH
 * Find the depth of the black nodes. This is useful for
 * verifying that a given red-black tree is valid
 ****************************************************/
   template <typename T>
   int BST<T>::BNode::findDepth() const
   {
      // if there are no children, the depth is ourselves
      if (pRight == nullptr && pLeft == nullptr)
         return (isRed ? 0 : 1);

      // if there is a right child, go that way
      if (pRight != nullptr)
         return (isRed ? 0 : 1) + pRight->findDepth();
      else
         return (isRed ? 0 : 1) + pLeft->findDepth();
   }

   /****************************************************
    * BINARY NODE :: VERIFY RED BLACK
    * Do all four red-black rules work here?
    ***************************************************/
   template <typename T>
   bool BST<T>::BNode::verifyRedBlack(int depth) const
   {
      bool fReturn = true;
      depth -= (isRed == false) ? 1 : 0;

      // Rule a) Every node is either red or black
      assert(isRed == true || isRed == false); // this feels silly

      // Rule b) The root is black
      if (pParent == nullptr)
         if (isRed == true)
            fReturn = false;

      // Rule c) Red nodes have black children
      if (isRed == true)
      {
         if (pLeft != nullptr)
            if (pLeft->isRed == true)
               fReturn = false;

         if (pRight != nullptr)
            if (pRight->isRed == true)
               fReturn = false;
      }

      // Rule d) Every path from a leaf to the root has the same # of black nodes
      if (pLeft == nullptr && pRight && nullptr)
         if (depth != 0)
            fReturn = false;
      if (pLeft != nullptr)
         if (!pLeft->verifyRedBlack(depth))
            fReturn = false;
      if (pRight != nullptr)
         if (!pRight->verifyRedBlack(depth))
            fReturn = false;

      return fReturn;
   }


   /******************************************************
    * VERIFY B TREE
    * Verify that the tree is correctly formed
    ******************************************************/
   template <typename T>
   std::pair<T, T> BST<T>::BNode::verifyBTree() const
   {
      // largest and smallest values
      std::pair <T, T> extremes;
      extremes.first = data;
      extremes.second = data;

      // check parent
      if (pParent)
         assert(pParent->pLeft == this || pParent->pRight == this);

      // check left, the smaller sub-tree
      if (pLeft)
      {
         assert(!(data < pLeft->data));
         assert(pLeft->pParent == this);
         pLeft->verifyBTree();
         std::pair <T, T> p = pLeft->verifyBTree();
         assert(!(data < p.second));
         extremes.first = p.first;

      }

      // check right
      if (pRight)
      {
         assert(!(pRight->data < data));
         assert(pRight->pParent == this);
         pRight->verifyBTree();

         std::pair <T, T> p = pRight->verifyBTree();
         assert(!(p.first < data));
         extremes.second = p.second;
      }

      // return answer
      return extremes;
   }

   /*********************************************
    * COMPUTE SIZE
    * Verify that the BST is as large as we think it is
    ********************************************/
   template <typename T>
   int BST<T>::BNode::computeSize() const
   {
      return 1 +
         (pLeft == nullptr ? 0 : pLeft->computeSize()) +
         (pRight == nullptr ? 0 : pRight->computeSize());
   }
#endif // DEBUG

/******************************************************
 * BINARY NODE :: BALANCE
 * Balance the tree from a given location
 ******************************************************/
   template <typename T>
   void BST<T>::BNode::balance()
   {
      // Case 1: if we are the root, then color ourselves black and call it a day.


      // Case 2: if the parent is black, then there is nothing left to do

      // Case 3: if the aunt is red, then just recolor

      // Case 4: if the aunt is black or non-existant, then we need to rotate

      // Case 4a: We are mom's left and mom is granny's left
      // case 4b: We are mom's right and mom is granny's right
      // Case 4c: We are mom's right and mom is granny's left
      // case 4d: we are mom's left and mom is granny's right
   }

   /*************************************************
    *************************************************
    *************************************************
    ****************** ITERATOR *********************
    *************************************************
    *************************************************
    *************************************************/

   /**************************************************
    * BST ITERATOR :: INCREMENT PREFIX
    * advance by one
    *************************************************/
   template <typename T>
   typename BST<T>::iterator& BST<T>::iterator::operator ++()
   {
      return *this;
   }

   /**************************************************
    * BST ITERATOR :: DECREMENT PREFIX
    * advance by one
    *************************************************/
   template <typename T>
   typename BST<T>::iterator& BST<T>::iterator::operator --()
   {
      return *this;

   }


} // namespace custom


