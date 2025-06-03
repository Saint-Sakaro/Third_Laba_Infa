#pragma once

#include "ArraySequence.hpp"
#include "Exceptions.hpp"
#include "IEnumerable.hpp"

template<typename T>
class Stack : public IEnumerable<T> {
private:
    ArraySequence<T> items;

    class StackEnumerator : public IEnumerator<T> {
    private:
        const ArraySequence<T>& sequence;
        int currentIndex;
        T currentValue;

    public:
        explicit StackEnumerator(const ArraySequence<T>& seq)
            : sequence(seq)
            , currentIndex(-1)
            , currentValue()
        {}
        bool MoveNext() override
        {
            if (currentIndex + 1 < sequence.GetLength())
            {
                currentIndex++;
                currentValue = sequence.Get(currentIndex);
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
    Stack() = default;

    void Push(const T& item)
    {
        items.Append(item);
    }

    Option<T> Pop()
    {
        if (items.GetLength() == 0)
        {
            return Option<T>::None();
        }
        T value = items.GetLast();
        auto* newItems = new ArraySequence<T>();
        for (int i = 0; i < items.GetLength() - 1; i++)
        {
            newItems->Append(items.Get(i));
        }
        items = *newItems;
        delete newItems;
        return Option<T>::Some(value);
    }

    Option<T> Top() const {
        return items.TryGetLast();
    }

    bool IsEmpty() const
    {
        return items.GetLength() == 0;
    }

    int GetLength() const
    {
        return items.GetLength();
    }

    int GetSize() const {
        return GetLength();
    }

    T Get(int index) const
    {
        return items.Get(index);
    }

    Sequence<T>* GetSequence() const
    {
        return new ArraySequence<T>(items);
    }

    template<typename R>
    Sequence<R>* Map(R (*func)(const T&)) const
    {
        auto* result = new ArraySequence<R>();
        for (int i = 0; i < items.GetLength(); ++i)
        {
            result->Append(func(items.Get(i)));
        }
        return result;
    }

    Sequence<T>* Where(bool (*predicate)(const T&)) const
    {
        auto* result = new ArraySequence<T>();
        for (int i = 0; i < items.GetLength(); ++i)
        {
            T current = items.Get(i);
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
            result = func(result, items.Get(i));
        }
        return result;
    }

    Stack<T>* Concat(const Stack<T>* other) const
    {
        auto* result = new Stack<T>();
        for (int i = 0; i < items.GetLength(); ++i)
        {
            result->Push(items.Get(i));
        }
        for (int i = 0; i < other->GetLength(); ++i)
        {
            result->Push(other->Get(i));
        }
        return result;
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const
    {
        auto* result = new ArraySequence<T>();
        for (int i = startIndex; i <= endIndex; i++)
        {
            result->Append(items.Get(i));
        }
        return result;
    }

    bool ContainsSubsequence(const Stack<T>* subseq) const
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
                if (!(items.Get(i + j) == subseq->Get(j)))
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

    IEnumerator<T>* GetEnumerator() const override
    {
        return new StackEnumerator(items);
    }
};