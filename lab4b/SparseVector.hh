#include <cassert>
#include <iostream>

/*
 * This is the SparseVector class. 
 * The sparse vector class will represent a mathematical vector of some 
 * fixed size. Because the internal representation is sparse, the vector 
 * will be able to span as many as 2,147,483,647 elements
 */
class SparseVector {
    public:
        // one argument constructor
        SparseVector(int vec_size);
        // destructor clears list
        ~SparseVector();
        // copy constructor performs a deep copy on list
        SparseVector(const SparseVector &sv);
        // assignment operator 
        SparseVector& operator=(const SparseVector &rhs);
        
        // checks equality and inequality
        bool operator==(const SparseVector &rhs) const;
        bool operator!=(const SparseVector &rhs) const;
        // add and subtraction
        SparseVector& operator+=(const SparseVector &rhs);
        SparseVector& operator-=(const SparseVector &rhs); 
        const SparseVector operator+(const SparseVector &rhs) const;
        const SparseVector operator-(const SparseVector &rhs) const; 

        // gets the current size of the linked list
        int getSize() const;
        // gets the element at some index
        int getElem(const int idx) const;
        // sets the element at some index
        void setElem(const int idx, const int val);

    private:

        // A linked-list node for our sparse vector elements.
        struct node {
            int index;   // Element number, in the range [0, size)
            int value;   // The value of this element
            node *next;  // A pointer to the next node in the linked-list

            // Node constructor - simply initializes the data-members
            node(int index, int value, node *next = 0) : 
                index(index), value(value), next(next) { };
        };
        void clear(); // clear helper function
        void copyList(const SparseVector &sv); // copies passed in list
        void copyList2(const SparseVector &sv, const int sign);

        // sets a nonzero value
        void setNonzeroElem(const int index, const int value);         
        void removeElem(const int index); // removes element at some index
        bool checkListOrder() const; // checks the order list

        // add or subtract SparseVectors
        void addSubVector(const SparseVector &sv, bool add);
        // remove zeros helper function
        void removeZeros();
        // check whether we have zeros 
        bool checkZeros();
        // print list helper function
        void printList();
        // add copy or negative
        void emptyLHSAddSub(const SparseVector &sv, const int sign);


        // start of the linked list
        node *start;
        // size of the linked list
        int size;
};
