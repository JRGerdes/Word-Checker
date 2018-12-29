// HashSet.hpp
//
// ICS 46 Spring 2018
// Project #4: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"

/*=====HASH NODE FRIEND CLASS=====*/
template<typename T>
class hashNode
{
    public:
    int key;
    T word;
    hashNode* Next;

    //CONSTRUCTOR
    hashNode(int key, T word)
    {
        this->key = key;
        this->word = word;
        this->Next = NULL;
    }

    hashNode(int key, T word, hashNode* n)
    {
        this->key = key;
        this->word = word;
        this->Next = n;
    }

    //FRIEND CLASS TO -> HASHSET
    template<typename ElementType>
    friend class HashSet;
};

template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    hashNode<ElementType> **hTable; //HASH TABLE
    HashFunction hashFunction;//SAVE HASHFUNCTION
    int capacity = DEFAULT_CAPACITY;//SET CAPACITY

    void makeEmpty(hashNode<ElementType> *& h) const;//MAKE HASHTABLE EMPTY
    hashNode<ElementType> **clone(hashNode<ElementType> **t, int capacity) const;//CLONE THE GIVING HASHTABLE
    void initTable(int cap);//INITIALIZE HASHTABLE
    void addNode(const ElementType& element, hashNode<ElementType>** h, int cap);//ADD NEW NODE
};


/*=====NO IDEA=====*/
namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}

/*=====DEFAULT CONSTRUCTOR=====*/
template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : hashFunction{hashFunction}
{
    this->hashFunction = hashFunction; //SET HASH FUNC TO USE
    initTable(this->capacity);//INITIALIZE
}

/*=====INITIALIZE TABLE=====*/
template<typename ElementType>
void HashSet<ElementType>::initTable(int cap)
{
    hTable = new hashNode<ElementType>*[cap];//HTABLE SIZE TO CAP
    for(int i = 0; i < cap; i++)
        hTable[i] = NULL;//SET ALL ELEMENTS TO NULL
}

/*=====MAKE ALL TABLE NODES NULL=====*/
template<typename ElementType>
void HashSet<ElementType>::makeEmpty(hashNode<ElementType> *& h) const
{
    for(int i = 0;i < capacity; i++) //GO THROUGH EVERY NODE
    {
        hashNode<ElementType>* entry = hTable[i];
        while(entry != NULL) //UNTIL END OF TABLE
        {
            hashNode<ElementType>* prev = entry; //TEMP
            entry = entry->Next; 
            delete prev;
        }
    }
    delete[] hTable;//DELETE HTABLE
}

/*=====DESTRUCTOR=====*/
template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{
    makeEmpty(*hTable);
    capacity = DEFAULT_CAPACITY;
}

/*=====COPY CONSTRUCTOR=====*/
template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    this->hashFunction = s.hashFunction;
    this->capacity = s.capacity;//SET NEW CAPACITY
    initTable(capacity);//INITIALIZE HTABLE TO NEW CAPACITY
    for(int i = 0; i < capacity; i++)//UNTIL END
    {
        hashNode<ElementType>* entry = s.hTable[i];//COPY EVERY ELEMENT
        while(entry != NULL)
        {
            hTable[i] = entry;
            entry = entry->Next;//NEXT ELEMENT
        }
    }
}

/*=====MOVE CONSTRUCTOR=====*/
template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>}
{
    this->hashFunction = std::move(s.hashFunction);
    this->capacity = std::move(s.capacity);//""
    initTable(capacity);//""
    for(int i = 0; i < capacity; i++)
    {
        hashNode<ElementType>* entry = std::move(s.hTable[i]);//""
        while(entry != NULL)
        {
            hTable[i] = entry;
            entry = entry->Next;//""
        }
    }
}

/*=====COPY OPERATOR=====*/
template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    
    if(this != &s)
    {
        this->hashFunction = s.hashFunction;
        this->capacity = s.capacity;
        initTable(capacity);
        for(int i = 0; i < capacity; i++)
        {
            hashNode<ElementType>* entry = s.hTable[i];
            while(entry != NULL)
            {
                hTable[i] = entry;
                entry = entry->Next;
            }
        }
        
    }
    return *this;
}

/*=====MOVE OPERATOR=====*/
template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    if(this != &s)
    {
        this->hashFunction = std::move(s.hashFunction);
        this->capacity = std::move(s.capacity);
        initTable(capacity);
        for(int i = 0; i < capacity; i++)
        {
            hashNode<ElementType>* entry = std::move(s.hTable[i]);
            while(entry != NULL)
            {
                hTable[i] = entry;
                entry = entry->Next;
            }
        }
    }
    return *this;
}

/*=====CLONE TABLE=====*/
template <typename ElementType>
hashNode<ElementType> **HashSet<ElementType>::clone(hashNode<ElementType>** t, int capacity) const
{
    if(t == NULL)
        return NULL;
    else
    {
        hashNode<ElementType>** hn = new hashNode<ElementType>*[capacity];//NEW HASHTABLE
        for(int i = 0; i < capacity; i++)
        {
            hashNode<ElementType>* entry = hn[i];//COPY EVERY ELEMENT
            while(entry != NULL)
                hn[i] = t[i];
        }
        return hn;
    }
}

/*=====IS IMPLEMENTED=====*/
template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

/*=====ADD NODE=====*/
template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    int hashVal = hashFunction(element) % capacity;
    hashNode<ElementType>* prev = NULL;
    hashNode<ElementType>* entry = hTable[hashVal];
    while(entry != NULL)
    {
        prev = entry;
        entry = entry->Next;
    }
    if(entry == NULL)
    {
        entry = new hashNode(hashVal, element);
        if(prev == NULL)
            hTable[hashVal] = entry;
        else
            prev->Next = entry;
    }
    else
        entry->word = element;

    if(size() > capacity * .8)//IF TOO FULL, DOUBLE THE HTABLE CAPACITY
    {
        int newCap = capacity * 2;
        hashNode<ElementType> ** ht = new hashNode<ElementType>*[newCap];
        for(int i = 0; i < newCap; i++)
            ht[i] = NULL;
        for(int j = 0; j < this->capacity; j++)
        {
            hashNode<ElementType>* entry = hTable[j];
            while(entry != NULL)
            {
                addNode(entry->word, ht, newCap);
                entry = entry->Next;
            }
        }
        capacity = newCap;
        hTable = ht;
    }
}

/*=====ADD NODE HELPER; USED FOR RESIZING HTABLE=====*/
template <typename ElementType>
void HashSet<ElementType>::addNode(const ElementType& element, hashNode<ElementType>** h, int cap)
{
    int hashVal = hashFunction(element) % cap;
    hashNode<ElementType>* prev = NULL;
    hashNode<ElementType>* entry = h[hashVal];
    while(entry != NULL)
    {
        prev = entry;
        entry = entry->Next;
    }
    if(entry == NULL)
    {
        entry = new hashNode(hashVal, element);
        if(prev == NULL)
            h[hashVal] = entry;
        else
            prev->Next = entry;
    }
    else
        entry->word = element;
}

/*=====CONTAINS=====*/
template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    int hashVal = hashFunction(element) % capacity;
    hashNode<ElementType>* entry = hTable[hashVal];
    while(entry != NULL)
    {
        if(entry->word == element)//IF ELEMENT IS IN THE HTABLE RETURN TRUE
            return true;
        entry = entry->Next;
    }
    return false;//ELSE RETURN FALSE
}

/*=====SIZE OF HTABLE=====*/
template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    int count = 0;
    for(int i = 0; i < capacity; i++)
    {
        if(hTable[i] == NULL)
            continue;
        else
        {
            hashNode<ElementType>* entry = hTable[i];
            while(entry != NULL)
            {
                count++;//ADD ONE FOR EVERY NEW ELEMENT FOUND
                entry = entry->Next;
            }
        }
    }
    return count;
}

/*=====GET NUMBER OF ELEMENTS AT INDEX=====*/
template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    int count = 0;
    if(hTable[index] == NULL)
        return count;
    else
    {
        hashNode<ElementType>* entry = hTable[index];
        while(entry != NULL)
        {
            count++;//ADD ONE FOR EVERY NEW ELEMENT
            entry = entry->Next;
        }
    }
    return count;
}

/*=====CHECK IF ELEMENT IS AT INDEX=====*/
template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if(hTable[index] == NULL)
        return false;
    else
    {
        hashNode<ElementType>* entry = hTable[index];
        while(entry != NULL)
        {
            if(entry->word == element)//IF ELEMENT IS IN GIVEN INDEX RETURN TRUE
                return true;
            entry = entry->Next;
        }
    }
    return false;
}



#endif // HASHSET_HPP

