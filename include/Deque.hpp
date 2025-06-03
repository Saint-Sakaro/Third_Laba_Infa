#pragma once

#include "ListSequence.hpp"
#include "Exceptions.hpp"
#include "Option.hpp"
#include "IEnumerable.hpp"
#include <algorithm>

template<typename T>
class Deque : public IEnumerable<T> {
private:
    ListSequence<T> items;

    class DequeEnumerator : public IEnumerator<T> {
    private:
        const ListSequence<T>& sequence;
        int currentIndex;
        T currentValue;

    public:
        explicit DequeEnumerator(const ListSequence<T>& seq)
            : sequence(seq)
            , currentIndex(-1)
        {
        }

        bool MoveNext() override {
            if (currentIndex + 1 < sequence.GetLength()) {
                currentIndex++;
                currentValue = sequence.Get(currentIndex);
                return true;
            }
            return false;
        }

        const T& Current() const override {
            if (currentIndex < 0 || currentIndex >= sequence.GetLength()) {
                throw InvalidStateException("Enumerator is not in a valid position");
            }
            return currentValue;
        }

        void Reset() override {
            currentIndex = -1;
        }
    };

public:
    Deque() = default;

    void PushFront(const T& item) {
        items.Prepend(item);
    }

    void PushBack(const T& item) {
        items.Append(item);
    }

    Option<T> PopFront() {
        if (items.GetLength() == 0) {
            return Option<T>::None();
        }
        T value = items.GetFirst();
        ListSequence<T> newItems;
        for (int i = 1; i < items.GetLength(); ++i) {
            newItems.Append(items.Get(i));
        }
        items = newItems;
        return Option<T>::Some(value);
    }

    Option<T> PopBack() {
        if (items.GetLength() == 0) {
            return Option<T>::None();
        }
        T value = items.GetLast();
        ListSequence<T> newItems;
        for (int i = 0; i < items.GetLength() - 1; ++i) {
            newItems.Append(items.Get(i));
        }
        items = newItems;
        return Option<T>::Some(value);
    }

    Option<T> PeekFront() const {
        if (items.GetLength() == 0) {
            return Option<T>::None();
        }
        return Option<T>::Some(items.GetFirst());
    }

    Option<T> PeekBack() const {
        if (items.GetLength() == 0) {
            return Option<T>::None();
        }
        return Option<T>::Some(items.GetLast());
    }

    bool IsEmpty() const {
        return items.GetLength() == 0;
    }

    int GetSize() const {
        return items.GetLength();
    }

    T Get(int index) const {
        return items.Get(index);
    }

    Sequence<T>* Map(T (*func)(const T&)) const {
        return items.Map(func);
    }

    Sequence<T>* Where(bool (*predicate)(const T&)) const {
        return items.Where(predicate);
    }

    T Reduce(T (*func)(const T&, const T&), const T& initial) const {
        return items.Reduce(func, initial);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const {
        return items.GetSubsequence(startIndex, endIndex);
    }

    Deque<T>* Concat(const Deque<T>* other) const {
        Deque<T>* result = new Deque<T>();
        for (int i = 0; i < items.GetLength(); ++i) {
            result->PushBack(items.Get(i));
        }
        for (int i = 0; i < other->GetSize(); ++i) {
            result->PushBack(other->Get(i));
        }
        return result;
    }

    std::pair<Deque<T>*, Deque<T>*> Split(bool (*predicate)(const T&)) const {
        Deque<T>* matching = new Deque<T>();
        Deque<T>* notMatching = new Deque<T>();

        for (int i = 0; i < items.GetLength(); ++i) {
            T current = items.Get(i);
            if (predicate(current)) {
                matching->PushBack(current);
            } else {
                notMatching->PushBack(current);
            }
        }

        return std::make_pair(matching, notMatching);
    }

    Sequence<T>* GetSequence() const {
        return new ListSequence<T>(items);
    }

    IEnumerator<T>* GetEnumerator() const override {
        return new DequeEnumerator(items);
    }

    void Sort() {
        if (items.GetLength() <= 1) return;

        T* array = new T[items.GetLength()];
        for (int i = 0; i < items.GetLength(); ++i) {
            array[i] = items.Get(i);
        }

        std::sort(array, array + items.GetLength());

        ListSequence<T> sortedItems;
        for (int i = 0; i < items.GetLength(); ++i) {
            sortedItems.Append(array[i]);
        }

        items = sortedItems;
        delete[] array;
    }

    void Sort(bool (*compare)(const T&, const T&)) {
        if (items.GetLength() <= 1) return;

        T* array = new T[items.GetLength()];
        for (int i = 0; i < items.GetLength(); ++i) {
            array[i] = items.Get(i);
        }

        std::sort(array, array + items.GetLength(), compare);

        ListSequence<T> sortedItems;
        for (int i = 0; i < items.GetLength(); ++i) {
            sortedItems.Append(array[i]);
        }

        items = sortedItems;
        delete[] array;
    }

    bool ContainsSubsequence(const Sequence<T>* subsequence) const {
        if (subsequence->GetLength() == 0) return true;
        if (subsequence->GetLength() > items.GetLength()) return false;

        for (int i = 0; i <= items.GetLength() - subsequence->GetLength(); ++i) {
            bool found = true;
            for (int j = 0; j < subsequence->GetLength(); ++j) {
                if (!(items.Get(i + j) == subsequence->Get(j))) {
                    found = false;
                    break;
                }
            }
            if (found) return true;
        }
        return false;
    }

    static Deque<T>* Merge(const Deque<T>* first, const Deque<T>* second) {
        Deque<T>* result = new Deque<T>();

        int i = 0, j = 0;
        while (i < first->GetSize() && j < second->GetSize()) {
            if (first->Get(i) <= second->Get(j)) {
                result->PushBack(first->Get(i));
                i++;
            } else {
                result->PushBack(second->Get(j));
                j++;
            }
        }

        while (i < first->GetSize()) {
            result->PushBack(first->Get(i));
            i++;
        }

        while (j < second->GetSize()) {
            result->PushBack(second->Get(j));
            j++;
        }

        return result;
    }

    static Deque<T>* Merge(const Deque<T>* first, const Deque<T>* second, 
                          bool (*compare)(const T&, const T&)) {
        Deque<T>* result = new Deque<T>();
        
        std::vector<T> firstElements;
        std::vector<T> secondElements;

        for (int i = first->GetSize() - 1; i >= 0; i--) {
            firstElements.push_back(first->Get(i));
        }
        
        for (int i = second->GetSize() - 1; i >= 0; i--) {
            secondElements.push_back(second->Get(i));
        }
        
        size_t i = 0, j = 0;
        bool useFirst = true;
        
        while (i < firstElements.size() || j < secondElements.size()) {
            if (useFirst && i < firstElements.size()) {
                result->PushBack(firstElements[i]);
                i++;
                useFirst = false;
            } else if (j < secondElements.size()) {
                result->PushBack(secondElements[j]);
                j++;
                useFirst = true;
            } else {
                useFirst = !useFirst; // пофикшено (ненавижу слияние)
            }
        }
        
        return result;
    }
};