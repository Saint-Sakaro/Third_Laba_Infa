#include <utility>
#include "ListSequence.hpp"
#include "Exceptions.hpp"
#include "IEnumerable.hpp"

template<typename T>
class Queue : public IEnumerable<T> {
private:
    ListSequence<T> items;

    class QueueEnumerator : public IEnumerator<T> {
    private:
        const ListSequence<T>& sequence;
        int currentIndex;
        T currentValue;

    public:
        explicit QueueEnumerator(const ListSequence<T>& seq)
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
    Queue() = default;

    void Enqueue(const T& item)
    {
        items.Append(item);
    }

    Option<T> Dequeue()
    {
        if (items.GetLength() == 0)
        {
            return Option<T>::None();
        }
        T value = items.GetFirst();
        auto* newItems = new ListSequence<T>();
        for (int i = 1; i < items.GetLength(); i++)
        {
            newItems->Append(items.Get(i));
        }
        items = *newItems;
        delete newItems;
        return Option<T>::Some(value);
    }

    Option<T> Front() const
    {
        return items.TryGetFirst();
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
        return new ListSequence<T>(items);
    }

    Sequence<T>* Map(T (*func)(const T&)) const
    {
        ListSequence<T>* result = new ListSequence<T>();
        for (int i = 0; i < items.GetLength(); ++i)
        {
            result->Append(func(items.Get(i)));
        }
        return result;
    }

    Sequence<T>* Where(bool (*predicate)(const T&)) const
    {
        ListSequence<T>* result = new ListSequence<T>();
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

    Queue<T>* Concat(const Queue<T>* other) const
    {
        Queue<T>* result = new Queue<T>();
        for (int i = 0; i < items.GetLength(); ++i)
        {
            result->Enqueue(items.Get(i));
        }
        for (int i = 0; i < other->GetSize(); ++i)
        {
            result->Enqueue(other->items.Get(i));
        }
        return result;
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const
    {
        return items.GetSubsequence(startIndex, endIndex);
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

    std::pair<Queue<T>*, Queue<T>*> Split(bool (*predicate)(const T&)) const
    {
        Queue<T>* matching = new Queue<T>();
        Queue<T>* notMatching = new Queue<T>();

        for (int i = 0; i < items.GetLength(); ++i)
        {
            T current = items.Get(i);
            if (predicate(current))
            {
                matching->Enqueue(current);
            }
            else
            {
                notMatching->Enqueue(current);
            }
        }

        return std::make_pair(matching, notMatching);
    }

    IEnumerator<T>* GetEnumerator() const override
    {
        return new QueueEnumerator(items);
    }
};