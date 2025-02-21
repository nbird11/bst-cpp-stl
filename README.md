# C++ Binary Search Tree Implementation

This project implements a Binary Search Tree (BST) data structure in C++. The implementation provides a foundation for both `set` and `map` containers, offering efficient search, insertion, and deletion operations.

## Overview

The Binary Search Tree is implemented through a BST class template that maintains the binary search tree property. Key features include:

- Template-based implementation supporting any comparable data type
- Red-black tree balancing for guaranteed O(log n) operations
- Support for both unique and non-unique key insertions
- Bidirectional iterator implementation
- Memory-efficient node management
- Support for both copy and move semantics
- Integration with STL-style containers (set and map)

## Class Structure

### `BST<T>`

The main BST class template with one parameter:

- T: Type of element stored in the tree

Key components:

- `BNode`: Private internal node structure
- `iterator`: Public bidirectional iterator class
- Root node pointer and size tracking

### `BST<T>::BNode`

The internal node structure containing:

- `data`: The stored element
- `pLeft`: Pointer to the left child node
- `pRight`: Pointer to the right child node
- `pParent`: Pointer to the parent node
- `isRed`: Red-black tree coloring flag

## Tree Operations

The implementation includes several key operations:

### Core Operations

- `insert()`: Insert elements (with unique/non-unique options)
- `erase()`: Remove elements
- `find()`: Search for elements
- `clear()`: Delete all nodes
- `swap()`: Exchange two trees
- `size()`: Count nodes
- `empty()`: Check if tree is empty

### Iterator Support

- `begin()`: Get iterator to first element
- `end()`: Get iterator past the last element
- Bidirectional iteration support

### Memory Management

- Efficient node reuse in assignment operations
- Proper cleanup of unused nodes
- Prevention of memory leaks

## Usage Example

```cpp
#include "bst.h"

// Create a BST with integers
custom::BST<int> bst;

// Insert elements
bst.insert(5);
bst.insert(3);
bst.insert(7);

// Find an element
auto it = bst.find(3);

// Iterate through the tree
for (auto it = bst.begin(); it != bst.end(); ++it)
    std::cout << *it << " ";

// Clean up in destructor when bst goes out of scope
```

## Testing

The implementation includes comprehensive unit tests that verify:

- Node insertion and deletion
- Tree balancing
- Iterator functionality
- Copy and move operations
- Memory management
- Edge cases

## Files

- `bst.h`: Main BST implementation with utility functions
- `testBST.h`: Unit tests for BST
- `testBST.cpp`: Test driver for unit tests
- `spy.h`: Spy implementation for precise testing measurements
- `testSpy.h`: Unit tests for Spy
- `unitTest.h`: Unit testing framework

## Building

The project includes Visual Studio solution files for building on Windows. Open `LabBST.sln` and build using Visual Studio 2019 or later.

There is no support for Makefiles at this time. Possibly in the future.

## Notes

- The implementation uses red-black tree balancing for performance
- Supports both unique and non-unique key insertions
- Provides foundation for STL-style set and map containers
- Special attention paid to iterator invalidation rules
- Debug mode includes additional validation of tree properties

## License

This code is provided for educational purposes. See included license file for terms of use. [TODO: add LICENSE]

## Authors

Nathan Bird  
[nathanbirdka@gmail.com](mailto:nathanbirdka@gmail.com)

Brock Hoskins  
[](.)
