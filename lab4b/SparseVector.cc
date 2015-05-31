#include "SparseVector.hh"

SparseVector::SparseVector(int vec_size) {
    size = vec_size;
    start = 0;
}

SparseVector::~SparseVector() {
    clear();
}

SparseVector::SparseVector(const SparseVector &sv) {
    copyList(sv);
}

SparseVector& SparseVector::operator=(const SparseVector &rhs) {
    // check for self assignment 
    if (this == &rhs) {
        return *this;
    }
    // clean up current contents of object 
    clear();
    copyList(rhs);
    return *this;
}


bool SparseVector::operator==(const SparseVector &rhs) const {
    if (size != rhs.getSize()) {
        return false;
    }
    node *rhs_curr = rhs.start;
    node *curr = start;
    while (curr != 0) {
        if ((curr->value != rhs_curr->value) || 
            (curr->index != rhs_curr->index)) {
            return false;
        }
        rhs_curr = rhs_curr->next;
        curr = curr->next;
    }
    // check that there aren't more nodes in rhs than lhs
    if (rhs_curr != 0) {
        return false;
    }
    return true;
}

bool SparseVector::operator!=(const SparseVector &rhs) const {
    return !(*this == rhs);
}

SparseVector& SparseVector::operator+=(const SparseVector &rhs) {
    addSubVector(rhs, true);
    return *this;
}

SparseVector& SparseVector::operator-=(const SparseVector &rhs) {
    addSubVector(rhs, false);
    return *this;
}

const SparseVector SparseVector::operator+(const SparseVector &rhs) const {
    return SparseVector(*this) += rhs;
}

const SparseVector SparseVector::operator-(const SparseVector &rhs) const {
    return SparseVector(*this) -= rhs;
}


int SparseVector::getSize() const {
    return size;
}

int SparseVector::getElem(const int idx) const {
    assert(checkListOrder());
    node *curr = start;
    // if index is larger than size of list, then don't need to iterate
    // through the entire list
    if (idx >= size) {
        return 0;
    }
    // otherwise iterate through the list until we find the value
    while (curr != 0) {
        // stop if we find the index
        if (curr->index == idx) {
            return curr->value;
        }
        // otherwise stop searching if we go past the index
        else if (curr->index > idx) {
            return 0;
        }
        curr = curr->next;
    }
    return 0;
}

void SparseVector::setElem(const int idx, const int val) {
    // set an element depending on whether val is 0 or not
    if (val == 0) 
        removeElem(idx);
    else 
        setNonzeroElem(idx, val);
    assert(checkListOrder());
}

void SparseVector::clear() {
    node *curr = start;
    size = 0;
    while (curr != 0) {
        // Get what is next, before deleting curr
        node *next = curr -> next;
        
        // delete this node
        delete curr;

        // go to the next node in the list
        curr = next;
    }
    start = 0;
}

void SparseVector::copyList(const SparseVector &sv) {
    // iterate through passed in list and copy each node
    node *otherCurr = sv.start;
    node *prev = 0;

    size = sv.getSize();
    // check to see if passed in list is empty
    if (otherCurr == 0) {
        start = 0;
    }
    while (otherCurr != 0) {
        node *curr = new node(otherCurr->index, otherCurr->value);

        if (prev == 0)
            start = curr;
        else
            prev->next = curr;

        prev = curr;
        otherCurr = otherCurr->next;
    }
    assert(checkListOrder());
}

void SparseVector::setNonzeroElem(const int index, const int value) {
    assert(value != 0);
    node *curr = start;
    node *prev = 0;
    
    // first check if we have an empty list
    if (curr == 0) {
        start = new node(index, value);
        // size += 1;
    }
    // otherwise iterate through the list
    else {
        while (curr != 0 && curr->index <= index) {
            if (prev == 0)
                start = curr;
            else
                prev->next = curr;
            prev = curr;
            curr = curr->next;
        }
        // if no iteration happens, then we need to prepend
        if (curr == start) {
            node *pprev = new node(index, value, curr);
            start = pprev;
            // size += 1;
        }
        else {
            // replace node value with new val if already have index in linked list
            if (prev->index == index) {
                prev->value = value;
            }
            // else insert a new node with new values after prev node
            else {
                node *nnext = new node(index, value, prev->next);
                prev->next = nnext;
                // size += 1;
            }
        }
    }
}

void SparseVector::removeElem(const int index) { 
    node *curr = start;
    node *prev = 0;
    // iterate through list to remove an element
    while (curr != 0 && curr->index != index) {
        if (prev == 0)
            start = curr;
        else
            prev->next = curr;

        prev = curr;
        curr = curr->next;
    }
    // did not get to iterate through - remove the first element
    if (prev == 0) {
        delete start;
        start = curr->next;
        // size -= 1;
    }
    // else check that we are at the index that we need to remove
    else if (curr->index == index) {
        prev->next = curr->next;
        delete curr;
        // size -= 1;
    }
    assert(checkListOrder());
}

// checks the list order
bool SparseVector::checkListOrder() const {
    node *curr = start;
    int prev_index = -1; 
    while (curr != 0) {
        if (prev_index > curr->index) {
            return false;
        }
        prev_index = curr->index;
        curr = curr->next; 
    }
    return true;
}

void SparseVector::addSubVector(const SparseVector &sv, bool add) {
    node *curr = start;
    node *prev = 0;
    node *othercurr = sv.start;
    // checks to see that we are adding vectors of the same dimension
    assert(size == sv.size);
    int sign = (add ? 1 : -1);

    // keep iterating while there are values in both vectors
    while (curr != 0 && othercurr != 0) {
        // my node's index is less than the other node's index
        // - can assume that corresponding value in other list is zero
        // - nothing to add / subtract - my node's value doesn't change
        // - move my node-pointer forward down the list
        if (curr->index < othercurr->index) {
            prev = curr;
            curr = curr->next;
        }
        // node's index is equal to other node's index
        // - perform addition / subtraction operation with the two vals
        // - store result into my node
        // - move both node-pointers forward down the list
        else if (curr->index == othercurr->index) {
            curr->value += sign * othercurr->value;    
            prev = curr;
            curr = curr->next;
            othercurr = othercurr->next;
        }
        // my node's index is greater than other node's index
        // - my value for index j is zero, but the result is nonzero
        // - add a new node to my list for index j, containing the result
        // - move other node-pointer forward down the list
        else if (curr->index > othercurr->index) {
            node *nnode = new node(othercurr->index, sign * othercurr->value, curr);
            // first check for the first node 
            if (prev == 0) {
                // prepend new node to list
                start = nnode;
            }
            // otherwise add new node 
            else {
                prev->next = nnode;
            }
            prev = nnode;
            othercurr = othercurr->next;
            //  size += 1;
        }
        
        // reached end of both lists at the same time
        if (curr == 0 && othercurr == 0) {
            // do nothing
        }
        // reached end of other list first
        else if (othercurr == 0 && curr != 0) {
            // do nothing
        }
        // reached end of my list first
        // copy remaining nodes in other list to the end of my list
        else if (curr == 0 && othercurr != 0) {
            node *cp_node = new node(othercurr->index, sign * 
                othercurr->value);

            prev->next = cp_node;
            prev = cp_node;
            othercurr = othercurr->next;
        }
    }
    if (start == 0) {
        emptyLHSAddSub(sv, sign);
    }
    removeZeros();
}

void SparseVector::emptyLHSAddSub(const SparseVector &sv, const int sign) {
    // iterate through passed in list and copy each node
    node *otherCurr = sv.start;
    node *prev = 0;

    size = sv.getSize();
    // check to see if passed in list is empty
    if (otherCurr == 0) {
        start = 0;
    }
    while (otherCurr != 0) {
        node *curr = new node(otherCurr->index, sign * otherCurr->value);

        if (prev == 0)
            start = curr;
        else
            prev->next = curr;

        prev = curr;
        otherCurr = otherCurr->next;
    }
    assert(checkListOrder());
}
void SparseVector::removeZeros() {
    node *curr = start;
    node *prev = 0;
    while (curr != 0) {
        if (curr->value == 0) {
            if (prev == 0) {
                start = curr->next;
                prev = 0;
                delete curr;
                curr = curr->next;
            }
            else {
                prev->next = curr->next;
                delete curr;
                curr = prev->next;
            }
        }
        else {
            prev = curr;
            curr = curr->next;
        }
    }
    assert(checkZeros());
}

bool SparseVector::checkZeros() {
    node *curr = start;
    while (curr != 0) {
        if (curr->value == 0) {
            return false;
        }
        curr = curr->next;
    }
    return true;
}

void SparseVector::printList() {
    node *curr = start;
    while (curr != 0) {
         std::cout << "(index, value) = (" << curr->index << "," << curr->value
             << ")";
        curr = curr->next;
    }
}


	



