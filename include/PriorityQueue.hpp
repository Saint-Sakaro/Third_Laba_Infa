#pragma once

#include <utility>
#include <algorithm>
#include "ListSequence.hpp"
#include "Exceptions.hpp"
#include "IEnumerable.hpp"

template<typename T>
class PriorityQueue : public IEnumerable<T> {
private:
    ListSequence<std::pair<T, int>> items;

    class PriorityQueueEnumerator : public IEnumerator<T> {
    private:
        const ListSequence<std::pair<T, int>>& sequence;
        int currentIndex;
        T currentValue;

    public:
        explicit PriorityQueueEnumerator(const ListSequence<std::pair<T, int>>& seq)
            : sequence(seq)
            , currentIndex(-1)
        {
        }

        bool MoveNext() override
        {
            if (currentIndex + 1 < sequence.GetLength())
            {
                currentIndex++;
                currentValue = sequence.Get(currentIndex).first;
                return true;
            }
            return false;
        }

        const T& Current() const override
        {
            if (currentIndex < 0 || currentIndex >= sequence.GetLength())
            {
                throw InvalidStateException("Enumerator is not in a valid position");
            }
            return currentValue;
        }

        void Reset() override
        {
            currentIndex = -1;
        }
    };

public:
    PriorityQueue() = default;

    void Enqueue(const T& item, int priority)
    {
        items.Append(std::make_pair(item, priority));
    }

    Option<T> Dequeue()
    {
        if (items.GetLength() == 0)
        {
            return Option<T>::None();
        }
        int maxPriorityIndex = 0;
        int maxPriority = items.Get(0).second;
        for (int i = 1; i < items.GetLength(); ++i)
        {
            if (items.Get(i).second > maxPriority)
            {
                maxPriority = items.Get(i).second;
                maxPriorityIndex = i;
            }
        }

        T value = items.Get(maxPriorityIndex).first;
        ListSequence<std::pair<T, int>> newItems;
        for (int i = 0; i < items.GetLength(); ++i)
        {
            if (i != maxPriorityIndex)
            {
                newItems.Append(items.Get(i));
            }
        }
        items = newItems;
        return Option<T>::Some(value);
    }

    Option<T> Front() const
    {
        if (items.GetLength() == 0)
        {
            return Option<T>::None();
        }

        int maxPriorityIndex = 0;
        int maxPriority = items.Get(0).second;
        for (int i = 1; i < items.GetLength(); ++i)
        {
            if (items.Get(i).second > maxPriority)
            {
                maxPriority = items.Get(i).second;
                maxPriorityIndex = i;
            }
        }

        return Option<T>::Some(items.Get(maxPriorityIndex).first);
    }

    bool IsEmpty() const
    {
        return items.GetLength() == 0;
    }

    int GetSize() const
    {
        return items.GetLength();
    }

    Sequence<T>* GetSequence() const
    {
        ListSequence<T>* result = new ListSequence<T>();
        for (int i = 0; i < items.GetLength(); ++i)
        {
            result->Append(items.Get(i).first);
        }
        return result;
    }

    Sequence<T>* Map(T (*func)(const T&)) const
    {
        int size = items.GetLength();
        auto* pairs = new std::pair<T, int>[size];
        for (int i = 0; i < size; ++i)
        {
            pairs[i] = items.Get(i);
        }
        
        for (int i = 0; i < size - 1; ++i)
        {
            for (int j = 0; j < size - i - 1; ++j)
            {
                if (pairs[j].second < pairs[j + 1].second)
                {
                    auto temp = pairs[j];
                    pairs[j] = pairs[j + 1];
                    pairs[j + 1] = temp;
                }
            }
        }
        
        ListSequence<T>* result = new ListSequence<T>();
        for (int i = 0; i < size; ++i)
        {
            result->Append(func(pairs[i].first));
        }
        
        delete[] pairs;
        return result;
    }

    Sequence<T>* Where(bool (*predicate)(const T&)) const
    {
        ListSequence<T>* result = new ListSequence<T>();
        for (int i = 0; i < items.GetLength(); ++i)
        {
            T current = items.Get(i).first;
            if (predicate(current))
            {
                result->Append(current);
            }
        }
        return result;
    }

    T Reduce(T (*func)(const T&, const T&), const T& initial) const
    {
        T result = initial;
        for (int i = 0; i < items.GetLength(); ++i)
        {
            result = func(result, items.Get(i).first);
        }
        return result;
    }

    PriorityQueue<T>* Concat(const PriorityQueue<T>* other) const
    {
        PriorityQueue<T>* result = new PriorityQueue<T>();
        for (int i = 0; i < items.GetLength(); ++i)
        {
            result->Enqueue(items.Get(i).first, items.Get(i).second);
        }
        for (int i = 0; i < other->items.GetLength(); ++i)
        {
            result->Enqueue(other->items.Get(i).first, other->items.Get(i).second);
        }
        return result;
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const
    {
        if (startIndex < 0 || endIndex >= items.GetLength() || startIndex > endIndex)
        {
            throw IndexOutOfRangeException("Invalid subsequence indices");
        }

        int size = items.GetLength();
        auto* pairs = new std::pair<T, int>[size];
        for (int i = 0; i < size; ++i)
        {
            pairs[i] = items.Get(i);
        }
        
        for (int i = 0; i < size - 1; ++i)
        {
            for (int j = 0; j < size - i - 1; ++j)
            {
                if (pairs[j].second < pairs[j + 1].second)
                {
                    auto temp = pairs[j];
                    pairs[j] = pairs[j + 1];
                    pairs[j + 1] = temp;
                }
            }
        }

        ListSequence<T>* result = new ListSequence<T>();
        for (int i = startIndex; i <= endIndex; ++i)
        {
            result->Append(pairs[i].first);
        }
        
        delete[] pairs;
        return result;
    }

    bool ContainsSubsequence(const Sequence<T>* subseq) const
    {
        if (subseq->GetLength() == 0)
        {
            return true;
        }
        if (subseq->GetLength() > items.GetLength())
        {
            return false;
        }

        for (int i = 0; i <= items.GetLength() - subseq->GetLength(); ++i)
        {
            bool match = true;
            for (int j = 0; j < subseq->GetLength(); ++j)
            {
                if (!(items.Get(i + j).first == subseq->Get(j)))
                {
                    match = false;
                    break;
                }
            }
            if (match)
            {
                return true;
            }
        }
        return false;
    }

    std::pair<PriorityQueue<T>*, PriorityQueue<T>*> Split(bool (*predicate)(const T&)) const
    {
        PriorityQueue<T>* matching = new PriorityQueue<T>();
        PriorityQueue<T>* notMatching = new PriorityQueue<T>();

        for (int i = 0; i < items.GetLength(); ++i)
        {
            T current = items.Get(i).first;
            int priority = items.Get(i).second;
            if (predicate(current))
            {
                matching->Enqueue(current, priority);
            }
            else
            {
                notMatching->Enqueue(current, priority);
            }
        }

        return std::make_pair(matching, notMatching);
    }

    IEnumerator<T>* GetEnumerator() const override
    {
        return new PriorityQueueEnumerator(items);
    }
    
};