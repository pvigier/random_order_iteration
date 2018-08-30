#pragma once

#include <vector>
#include <numeric>
#include <algorithm>

// Offset

template<typename ForwardIt>
class OffsetTraversal
{
public:
    using difference_type = typename std::iterator_traits<ForwardIt>::difference_type;

    OffsetTraversal(ForwardIt first, ForwardIt last, difference_type offset, std::forward_iterator_tag) :
        mFirst(first), mLast(last), mOffset(offset)
    {

    }

    class iterator
    {
    public:
        using iterator_category = typename std::iterator_traits<ForwardIt>::iterator_category;
        using value_type = typename std::iterator_traits<ForwardIt>::value_type;
        using difference_type = typename std::iterator_traits<ForwardIt>::difference_type;
        using pointer = typename std::iterator_traits<ForwardIt>::pointer;
        using reference = typename std::iterator_traits<ForwardIt>::reference;

        iterator() {};
        iterator(ForwardIt it, ForwardIt first, ForwardIt last, bool isFirst) :
            mIt(it), mFirst(first), mLast(last), mIsFirst(isFirst) {};
        iterator(const iterator& it) : mIt(it.mIt), mFirst(it.mFirst), mLast(it.mLast), mIsFirst(it.mIsFirst) {}

        bool operator==(const iterator& it) { return mIt == it.mIt && mFirst == it.mFirst && mLast == it.mLast && mIsFirst == it.mIsFirst; }
        bool operator!=(const iterator& it) { return mIt != it.mIt || mFirst != it.mFirst || mLast != it.mLast || mIsFirst != it.mIsFirst; }

        reference operator*() { return *mIt; }
        pointer operator->() { return mIt; }

        iterator& operator++()
        {
            ++mIt;
            if (mIt == mLast)
                mIt = mFirst;
            mIsFirst = false;
            return *this;
        }

        iterator operator++(int)
        {
            if (mIt++ == mLast)
                return iterator(mFirst, mFirst, mLast, false);
            else
                return iterator(mIt++, mFirst, mLast, false);
        }

    private:
        ForwardIt mIt;
        ForwardIt mFirst;
        ForwardIt mLast;
        bool mIsFirst;
    };

    iterator begin()
    {
        return iterator(mFirst + mOffset, mFirst, mLast, true);
    }

    iterator end()
    {
        return iterator(mFirst + mOffset, mFirst, mLast, false);
    }

private:
    ForwardIt mFirst;
    ForwardIt mLast;
    difference_type mOffset;
};

template<typename Iter>
OffsetTraversal<Iter> offset(Iter first, Iter last, typename std::iterator_traits<Iter>::difference_type n)
{
    return OffsetTraversal<Iter>(first, last, n, typename std::iterator_traits<Iter>::iterator_category());
}

// Offset and fixed step

template<typename RandomIt>
class PseudoRandomTraversal
{
public:
    using difference_type = typename std::iterator_traits<RandomIt>::difference_type;

    PseudoRandomTraversal(RandomIt first, RandomIt last, difference_type offset, difference_type prime,
        std::random_access_iterator_tag) :
        mFirst(first), mSize(last - first), mOffset(offset), mPrime(prime)
    {

    }

    class iterator
    {
    public:
        using iterator_category = typename std::iterator_traits<RandomIt>::iterator_category;
        using value_type = typename std::iterator_traits<RandomIt>::value_type;
        using difference_type = typename std::iterator_traits<RandomIt>::difference_type;
        using pointer = typename std::iterator_traits<RandomIt>::pointer;
        using reference = typename std::iterator_traits<RandomIt>::reference;

        iterator() {};
        iterator(RandomIt it, RandomIt first, difference_type size, difference_type offset, difference_type prime, bool isFirst) :
            mIt(it), mFirst(first), mSize(size), mOffset(offset), mPrime(prime), mIsFirst(isFirst) {};
        iterator(const iterator& it) :
            mIt(it.mIt), mFirst(it.mFirst), mSize(it.mSize), mOffset(it.mOffset), mPrime(it.mPrime), mIsFirst(it.mIsFirst) {}

        bool operator==(const iterator& it) { return mIt == it.mIt && mFirst == it.mFirst && mSize == it.mSize && mOffset == it.mOffset && mPrime == it.mPrime && mIsFirst == it.mIsFirst; }
        bool operator!=(const iterator& it) { return mIt != it.mIt || mFirst != it.mFirst || mSize != it.mSize || mOffset != it.mOffset || mPrime != it.mPrime || mIsFirst != it.mIsFirst; }

        reference operator*() { return *mIt; }
        pointer operator->() { return mIt; }

        iterator& operator++()
        {
            mOffset = (mOffset + mPrime) % mSize;
            mIt = mFirst + mOffset;
            mIsFirst = false;
            return *this;
        }

        iterator operator++(int)
        {
            difference_type newOffset = (mOffset + mPrime) % mSize;
            return iterator(mFirst + newOffset, mFirst, mSize, mOffset, mPrime, false);
        }

    private:
        RandomIt mIt;
        RandomIt mFirst;
        difference_type mSize;
        difference_type mOffset;
        difference_type mPrime;
        bool mIsFirst;
    };

    iterator begin()
    {
        return iterator(mFirst + mOffset, mFirst, mSize, mOffset, mPrime, true);
    }

    iterator end()
    {
        return iterator(mFirst + mOffset, mFirst, mSize, mOffset, mPrime, false);
    }

private:
    RandomIt mFirst;
    difference_type mSize;
    difference_type mOffset;
    difference_type mPrime;
};

template<typename Iter>
PseudoRandomTraversal<Iter> pseudoShuffled(Iter first, Iter last, typename std::iterator_traits<Iter>::difference_type offset, typename std::iterator_traits<Iter>::difference_type prime)
{
    return PseudoRandomTraversal<Iter>(first, last, offset, prime, typename std::iterator_traits<Iter>::iterator_category());
}

// Shuffle the indices

template<typename RandomIt, typename URBG>
class RandomTraversal
{
public:
    using difference_type = typename std::iterator_traits<RandomIt>::difference_type;

    RandomTraversal(RandomIt first, RandomIt last, URBG&& g, std::random_access_iterator_tag) :
        mFirst(first), mLast(last), mIndices(mLast - mFirst)
    {
        std::iota(mIndices.begin(), mIndices.end(), 0);
        std::shuffle(mIndices.begin(), mIndices.end(), g);
    }

    class iterator
    {
    public:
        using iterator_category = typename std::iterator_traits<RandomIt>::iterator_category;
        using value_type = typename std::iterator_traits<RandomIt>::value_type;
        using difference_type = typename std::iterator_traits<RandomIt>::difference_type;
        using pointer = typename std::iterator_traits<RandomIt>::pointer;
        using reference = typename std::iterator_traits<RandomIt>::reference;

        iterator() {};
        iterator(RandomIt it, RandomIt first, RandomIt last, const std::vector<difference_type>& indices, typename std::vector<difference_type>::iterator indicesIt) :
            mIt(it), mFirst(first), mLast(last), mIndices(indices), mIndicesIt(indicesIt) {}
        iterator(const iterator& it) : mIt(it.mIt), mFirst(it.mFirst), mIndices(it.mIndices), mIndicesIt(it.mIndicesIt) {}

        bool operator==(const iterator& it) { return mIt == it.mIt && mFirst == it.mFirst && mLast == it.mLast && &mIndices == &it.mIndices && mIndicesIt == it.mIndicesIt; }
        bool operator!=(const iterator& it) { return mIt != it.mIt || mFirst != it.mFirst || mLast != it.mLast || &mIndices != &it.mIndices || mIndicesIt != it.mIndicesIt; }

        reference operator*() { return *mIt; }
        pointer operator->() { return mIt; }

        iterator& operator++()
        {
            ++mIndicesIt;
            if (mIndicesIt != mIndices.end())
                mIt = mFirst + (*mIndicesIt);
            else
                mIt = mLast;
            return *this;
        }

        iterator operator++(int)
        {
            const typename std::vector<difference_type>::iterator newIndicesIt = mIndicesIt++;
            if (mIndicesIt != mIndices.end())
                return iterator(mFirst + (*newIndicesIt), mFirst, mLast, mIndices, newIndicesIt);
            else
                return iterator(mLast, mFirst, mLast, mIndices, newIndicesIt);
        }

    private:
        RandomIt mIt;
        RandomIt mFirst;
        RandomIt mLast;
        const std::vector<difference_type>& mIndices;
        typename std::vector<difference_type>::iterator mIndicesIt;
    };

    iterator begin()
    {
        return iterator(mFirst + mIndices.front(), mFirst, mLast, mIndices, mIndices.begin());
    }

    iterator end()
    {
        return iterator(mLast, mFirst, mLast, mIndices, mIndices.end());
    }

private:
    RandomIt mFirst;
    RandomIt mLast;
    std::vector<difference_type> mIndices;
};

template<typename Iter, typename URBG>
RandomTraversal<Iter, URBG> shuffled(Iter first, Iter last, URBG&& g)
{
    return RandomTraversal<Iter, URBG>(first, last, g, typename std::iterator_traits<Iter>::iterator_category());
}
