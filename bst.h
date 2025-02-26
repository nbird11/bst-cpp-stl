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
      // Copy
      //
      static BNode* copy(const BNode* pSrc);

      //
      // Assign
      //
      static void assign(BNode*& pDest, const BNode* pSrc);

      //
      // Insert
      //
      void addLeft(BNode* pNode);
      void addLeft(const T& t);
      void addLeft(T&& t);
      void addRight(BNode* pNode);
      void addRight(const T& t);
      void addRight(T&& t);

      //
      // Remove
      //
      static void clear(BST<T>::BNode*& pNode) noexcept;

      // 
      // Status
      //
      bool isRightChild(BNode* pNode) const { return pParent == pNode && pNode->pRight == this; }
      bool isLeftChild (BNode* pNode) const { return pParent == pNode && pNode->pLeft == this; }

      // balance the tree
      void balance(BNode*& pRoot);

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
      iterator(BNode* p = nullptr) : pNode(p)
      {}
      iterator(const iterator& rhs) : pNode(rhs.pNode)
      {}
      iterator& operator =(const iterator& rhs)
      {
         pNode = rhs.pNode;
         return *this;
      }

      // compare
      bool operator ==(const iterator& rhs) const
      {
         return pNode == rhs.pNode;
      }
      bool operator !=(const iterator& rhs) const
      {
         return pNode != rhs.pNode;
      }

      // de-reference. Cannot change because it will invalidate the BST
      const T& operator *() const
      {
         return pNode->data;
      }

      // increment and decrement
      iterator& operator ++();
      iterator  operator ++(int postfix)
      {
         iterator temp(*this);
         ++this;
         return temp;
      }
      iterator& operator --();
      iterator  operator --(int postfix)
      {
         iterator temp(*this);
         --this;
         return temp;
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
   BST<T>::BST(const BST<T>& rhs) : BST()
   {
      *this = rhs;
   }

   /*********************************************
    * BST :: MOVE CONSTRUCTOR
    * Move one tree to another
    ********************************************/
   template <typename T>
   BST<T>::BST(BST<T>&& rhs) : BST()
   {
      *this = std::move(rhs);
   }

   /*********************************************
    * BST :: INITIALIZER LIST CONSTRUCTOR
    * Create a BST from an initializer list
    ********************************************/
   template <typename T>
   BST<T>::BST(const std::initializer_list<T>& il) : BST()
   {
      *this = il;
   }

   /*********************************************
    * BST :: DESTRUCTOR
    ********************************************/
   template <typename T>
   BST<T>::~BST()
   {
      clear();
   }


   /*********************************************
    * BST :: ASSIGNMENT OPERATOR
    * Copy one tree to another
    ********************************************/
   template <typename T>
   BST<T>& BST<T>::operator =(const BST<T>& rhs)
   {
      BNode::assign(root, rhs.root);
      numElements = rhs.numElements;
      return *this;
   }

   /*********************************************
    * BST :: ASSIGN-MOVE OPERATOR
    * Move one tree to another
    ********************************************/
   template <typename T>
   BST<T>& BST<T>::operator =(BST<T>&& rhs)
   {
      clear();
      swap(rhs);
      return *this;
   }

   /*********************************************
    * BST :: ASSIGNMENT OPERATOR with INITIALIZATION LIST
    * Copy nodes onto a BTree
    ********************************************/
   template <typename T>
   BST<T>& BST<T>::operator =(const std::initializer_list<T>& il)
   {
      clear();
      for (const T& t : il)
         insert(t);
      return *this;
   }

   /*********************************************
    * BST :: SWAP
    * Swap two trees
    ********************************************/
   template <typename T>
   void BST<T>::swap(BST<T>& rhs)
   {
      std::swap(root, rhs.root);
      std::swap(numElements, rhs.numElements);
   }

   /*****************************************************
    * BST :: INSERT
    * Insert a node at its correct (sorted) location in the tree
    ****************************************************/
   template <typename T>
   std::pair<typename BST<T>::iterator, bool> BST<T>::insert(const T& t, bool keepUnique)
   {
      // If no root, insert as root.
      if (!root)
      {
         root = new BNode(t);
         root->balance(root);
         numElements++;
         return { iterator(root), true };
      }

      // Go down the tree until you reach a leaf.
      BNode* current = root;
      while (true)
      {
         if (keepUnique && t == current->data)
            return { iterator(current), false };  // Don't insert duplicates if keepUnique.

         if (t < current->data)  // Left subtree
         {
            if (!current->pLeft)
            {
               BNode* newNode = new BNode(t);
               current->addLeft(newNode);
               newNode->balance(root);
               numElements++;
               return { iterator(newNode), true };
            }
            current = current->pLeft;
         }
         else  // Right subtree
         {
            if (!current->pRight)
            {
               BNode* newNode = new BNode(t);
               current->addRight(newNode);
               newNode->balance(root);
               numElements++;
               return { iterator(newNode), true };
            }
            current = current->pRight;
         }
      }
   }  // insert()

   template <typename T>
   std::pair<typename BST<T>::iterator, bool> BST<T>::insert(T&& t, bool keepUnique)
   {
      // If no root, insert as root.
      if (!root)
      {
         root = new BNode(std::move(t));
         root->balance(root);
         numElements++;
         return { iterator(root), true };
      }

      // Go down the tree until you reach a leaf.
      BNode* current = root;
      while (true)
      {
         if (keepUnique && t == current->data)
            return { iterator(current), false };  // Don't insert duplicates if keepUnique.

         if (t < current->data)  // Left subtree
         {
            if (!current->pLeft)
            {
               BNode* newNode = new BNode(std::move(t));
               current->addLeft(newNode);
               newNode->balance(root);
               numElements++;
               return { iterator(newNode), true };
            }
            current = current->pLeft;
         }
         else  // Right subtree
         {
            if (!current->pRight)
            {
               BNode* newNode = new BNode(std::move(t));
               current->addRight(newNode);
               newNode->balance(root);
               numElements++;
               return { iterator(newNode), true };
            }
            current = current->pRight;
         }
      }
   }  // insert() move

   /*************************************************
    * BST :: ERASE
    * Remove a given node as specified by the iterator
    ************************************************/
   template <typename T>
   typename BST<T>::iterator BST<T>::erase(iterator& it)
   {
      // If the iterator is at the end, do nothing
      if (it == end())
         return end();

      iterator itReturn = it;  // copy assignment operator
      ++itReturn;  // always return the next node

      BNode* pDelete = it.pNode;

      // Case 1: No Children
      if (!pDelete->pLeft && !pDelete->pRight)
      {
         // Make parent forget about us
         if (pDelete->pParent && pDelete->isLeftChild(pDelete->pParent))
            pDelete->pParent->pLeft = nullptr;
         else if (pDelete->pParent)
            // Must be right child if has parent and is not left child
            pDelete->pParent->pRight = nullptr;

         delete pDelete;
         numElements--;
         return itReturn;
      }

      // Case 2: One Child - Replace node with child
      //   Only left child
      if (!pDelete->pRight && pDelete->pLeft)
      {
         // Hook up child to parent
         pDelete->pLeft->pParent = pDelete->pParent;
         // Hook up parent to child: left
         if (pDelete->pParent && pDelete->isLeftChild(pDelete->pParent))
            pDelete->pParent->pLeft = pDelete->pLeft;
         // Hook up parent to child: right
         else if (pDelete->pParent)
            pDelete->pParent->pRight = pDelete->pLeft;

         delete pDelete;
         numElements--;
         return itReturn;
      }
      //   Only right child
      else if (pDelete->pRight && !pDelete->pLeft)
      {
         // Hook up child to parent
         pDelete->pRight->pParent = pDelete->pParent;
         // Hook up parent to child: left
         if (pDelete->pParent && pDelete->isLeftChild(pDelete->pParent))
            pDelete->pParent->pLeft = pDelete->pRight;
         // hook up parent to child: right
         else if (pDelete->pParent)
            pDelete->pParent->pRight = pDelete->pRight;

         delete pDelete;
         numElements--;
         return itReturn;
      }

      // Case 3: Two Children - Replace node with in-order successor
      if (pDelete->pLeft && pDelete->pRight)
      {
         // Find in-order successor
         BNode* pNext = itReturn.pNode;  // itReturn already points to next node in sequence.

         // Part A: Copy the pointers from pDelete to pNext
         pNext->pLeft = pDelete->pLeft;
         pNext->pLeft->pParent = pNext;

         // Special case: if pNext is not pDelete's direct right child
         if (pNext != pDelete->pRight)
         {
            // Hook up pNext's right child to pNext's parent if it exists
            if (pNext->pRight)
            {
               pNext->pRight->pParent = pNext->pParent;
               pNext->pParent->pLeft = pNext->pRight;  // pNext must be a left child
            }
            else
               pNext->pParent->pLeft = nullptr;

            // Hook up pDelete's right child to pNext
            pNext->pRight = pDelete->pRight;
            pNext->pRight->pParent = pNext;
         }

         // Hook up pNext to pDelete's parent
         pNext->pParent = pDelete->pParent;
         if (pDelete->pParent && pDelete->isLeftChild(pDelete->pParent))
            pDelete->pParent->pLeft = pNext;
         else if (pDelete->pParent)
            pDelete->pParent->pRight = pNext;
         else  // pDelete was the root
            root = pNext;

         delete pDelete;
         numElements--;
         return itReturn;
      }

      assert(false && "Unreachable");

      return itReturn;
   }

   /*****************************************************
    * BST :: CLEAR
    * Removes all the BNodes from a tree
    ****************************************************/
   template <typename T>
   void BST<T>::clear() noexcept
   {
      BNode::clear(root);
      numElements = 0;
   }

   /*****************************************************
    * BST :: BEGIN
    * Return the first node (left-most) in a binary search tree
    ****************************************************/
   template <typename T>
   typename BST<T>::iterator custom::BST<T>::begin() const noexcept
   {
      if (empty())
         return end();

      BST<T>::BNode* p = root;

      while (p->pLeft)
         p = p->pLeft;

      return iterator(p);
   }


   /****************************************************
    * BST :: FIND
    * Return the node corresponding to a given value
    ****************************************************/
   template <typename T>
   typename BST<T>::iterator BST<T>::find(const T& t)
   {
      BNode* p = root;

      if (!p)
         return end();

      while (p)
      {
         if (t == p->data)
            return iterator(p);
         else if (t < p->data)
            p = p->pLeft;
         else
            p = p->pRight;
      }

      return end();
   }

   /******************************************************
    ******************************************************
    ******************************************************
    *********************** B NODE ***********************
    ******************************************************
    ******************************************************
    ******************************************************/


   /**********************************************
    * COPY BINARY TREE
    * Copy pSrc->pRight to pDest->pRight and
    * pSrc->pLeft onto pDest->pLeft
    *********************************************/
   template <typename T>
   inline typename BST<T>::BNode* BST<T>::BNode::copy(const BNode* pSrc)
   {
      if (!pSrc)
         return nullptr;

      BNode* pDest = new BNode(pSrc->data);
      pDest->isRed = pSrc->isRed;

      pDest->pLeft = copy(pSrc->pLeft);
      if (pDest->pLeft)
         pDest->pLeft->pParent = pDest;

      pDest->pRight = copy(pSrc->pRight);
      if (pDest->pRight)
         pDest->pRight->pParent = pDest;

      return pDest;
   }

   /******************************************************
    * BINARY NODE :: Assignment
    * Copy the values from pSrc onto pDest preserving
    * as many of the nodes as possible.
    ******************************************************/
   template <typename T>
   inline void BST<T>::BNode::assign(BNode*& pDest, const BNode* pSrc)
   {
      // Case 1: Source is empty.
      if (!pSrc)
      {
         clear(pDest);
         return;
      }

      // Case 2: Destination is empty.
      if (!pDest)
      {
         pDest = copy(pSrc);
         return;
      }

      // Case 3: Both are non-empty.
      if (pSrc && pDest)
      {
         pDest->data = pSrc->data;
         assign(pDest->pLeft, pSrc->pLeft);
         if (pDest->pLeft)
            pDest->pLeft->pParent = pDest;

         assign(pDest->pRight, pSrc->pRight);
         if (pDest->pRight)
            pDest->pRight->pParent = pDest;
      }
   }

   /******************************************************
    * BINARY NODE :: ADD LEFT
    * Add a node to the left of the current node
    ******************************************************/
   template <typename T>
   void BST<T>::BNode::addLeft(BNode* pNode)
   {
      if (pNode)
         pNode->pParent = this;
      pLeft = pNode;
   }

   /******************************************************
    * BINARY NODE :: ADD RIGHT
    * Add a node to the right of the current node
    ******************************************************/
   template <typename T>
   void BST<T>::BNode::addRight(BNode* pNode)
   {
      if (pNode)
         pNode->pParent = this;
      pRight = pNode;
   }

   /******************************************************
    * BINARY NODE :: ADD LEFT
    * Add a node to the left of the current node
    ******************************************************/
   template <typename T>
   void BST<T>::BNode::addLeft(const T& t)
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
      addLeft(new BNode(t));
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

   /*****************************************************
   * BINARY NODE :: CLEAR RECURSIVE
   * Removes all the BNodes from a tree
   ****************************************************/
   template <typename T>
   inline void BST<T>::BNode::clear(BNode*& pNode) noexcept
   {
      if (!pNode)
         return;

      clear(pNode->pLeft);
      clear(pNode->pRight);

      delete pNode;
      pNode = nullptr;
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
   void BST<T>::BNode::balance(BNode*& pRoot)
   {
      // Case 1: if we are the root, then color ourselves black and call it a day.
      if (!pParent)
      {
         isRed = false;
         return;
      }

      // Case 2: if the parent is black, then there is nothing left to do
      if (!pParent->isRed)
         return;

      BNode* pGranny = pParent->pParent;
      BNode* pAunt   = pParent->isLeftChild(pGranny)
         ? pGranny->pRight
         : pGranny->pLeft;

      BNode* pSibling = this->isLeftChild(pParent)
         ? pParent->pRight
         : pParent->pLeft;

      // Case 3: if the aunt is red, then just recolor
      if (pAunt && pAunt->isRed)
      {
         // grandparent's kids turn black
         pParent->isRed = false;
         pAunt->isRed = false;
         // grandparent turns red
         pGranny->isRed = true;
         // recurse off of grandparent
         pGranny->balance(pRoot);
         return;
      }

      // Case 4: if the aunt is black or non-existant, then we need to rotate
      if (!pAunt || !pAunt->isRed)
      {
         // Case 4a: We are mom's left and mom is granny's left
         if (pParent->isRed && !pGranny->isRed
             && pParent->pLeft == this
             && pGranny->pLeft == pParent)
         {
            pParent->pParent = pGranny->pParent;
            if (pGranny->pParent && pGranny->isLeftChild(pGranny->pParent))
               pGranny->pParent->pLeft = pParent;
            else if (pGranny->pParent)
               pGranny->pParent->pRight = pParent;

            pParent->addRight(pGranny);
            pGranny->addLeft(pSibling);

            pGranny->isRed = true;
            pParent->isRed = false;

            if (!pParent->pParent)
               pRoot = pParent;

            return;
         }

         // case 4b: We are mom's right and mom is granny's right
         if (pParent->isRed && !pGranny->isRed
             && pParent->pRight == this
             && pGranny->pRight == pParent)
         {
            pParent->pParent = pGranny->pParent;
            if (pGranny->pParent && pGranny->isLeftChild(pGranny->pParent))
               pGranny->pParent->pLeft = pParent;
            else if (pGranny->pParent)
               pGranny->pParent->pRight = pParent;

            pParent->addLeft(pGranny);
            pGranny->addRight(pSibling);

            pGranny->isRed = true;
            pParent->isRed = false;

            if (!pParent->pParent)
               pRoot = pParent;

            return;
         }

         // Case 4c: We are mom's right and mom is granny's left
         if (this->isRightChild(pParent) && pParent->isLeftChild(pGranny))
         {
            pGranny->addLeft(this->pRight);
            pParent->addRight(this->pLeft);

            BNode* pParentTemp = pParent;  // Save pointer to parent
            if (!pGranny->pParent)
               this->pParent = nullptr;
            else if (pGranny->isLeftChild(pGranny->pParent))
               pGranny->pParent->pLeft = this;
            else
               pGranny->pParent->pRight = this;

            this->addRight(pGranny);
            this->addLeft(pParentTemp);

            pGranny->isRed = true;
            this->isRed = false;

            if (!pParent)
               pRoot = this;

            return;
         }

         // case 4d: we are mom's left and mom is granny's right
         if (this->isLeftChild(pParent) && pParent->isRightChild(pGranny))
         {
            pGranny->addRight(this->pLeft);
            pParent->addLeft(this->pRight);

            BNode* pParentTemp = pParent;  // Save pointer to parent
            if (!pGranny->pParent)
               this->pParent = nullptr;
            else if (pGranny->isLeftChild(pGranny->pParent))
               pGranny->pParent->pLeft = this;
            else
               pGranny->pParent->pRight = this;

            this->addLeft(pGranny);
            this->addRight(pParentTemp);

            pGranny->isRed = true;
            this->isRed = false;

            if (!pParent)
               pRoot = this;

            return;
         }
      }  // Case 4
   }  // balance()

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
      // Don't increment if we're already at the end
      if (!pNode)
         return *this;

      // Case 1: Have a right child
      if (pNode->pRight)
      {
         pNode = pNode->pRight;
         while (pNode->pLeft)
            pNode = pNode->pLeft;
         return *this;
      }

      // Case 2: No right child and pCurr is parent's left child
      if (!pNode->pRight && pNode->isLeftChild(pNode->pParent))
      {
         pNode = pNode->pParent;
         return *this;
      }

      // Case 3: No right child and pCurr is parent's right child
      if (!pNode->pRight && pNode->isRightChild(pNode->pParent))
      {
         while (pNode->pParent && pNode->isRightChild(pNode->pParent))
            pNode = pNode->pParent;
         pNode = pNode->pParent;
         return *this;
      }

      assert(false && "Unreachable");

      return *this;
   }

   /**************************************************
    * BST ITERATOR :: DECREMENT PREFIX
    * advance by one
    *************************************************/
   template <typename T>
   typename BST<T>::iterator& BST<T>::iterator::operator --()
   {
      // Don't increment if we're already at the end
      if (!pNode)
         return *this;

      // Case 1: Have a left child
      if (pNode->pLeft)
      {
         pNode = pNode->pLeft;
         while (pNode->pRight)
            pNode = pNode->pRight;
         return *this;
      }

      // Case 2: No left child and pCurr is parent's right child
      if (!pNode->pLeft && pNode->isRightChild(pNode->pParent))
      {
         pNode = pNode->pParent;
         return *this;
      }

      // Case 3: No left child and pCurr is parent's left child
      if (!pNode->pLeft && pNode->isLeftChild(pNode->pParent))
      {
         while (pNode->pParent && pNode->isLeftChild(pNode->pParent))
            pNode = pNode->pParent;
         pNode = pNode->pParent;
         return *this;
      }

      assert(false && "Unreachable");

      return *this;
   }


} // namespace custom


