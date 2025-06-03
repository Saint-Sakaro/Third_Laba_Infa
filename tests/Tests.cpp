#include <gtest/gtest.h>
#include <utility>
#include "Exceptions.hpp"
#include "Option.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "Person.hpp"
#include "Complex.hpp"
#include "PriorityQueue.hpp"
#include "Vector.hpp"
#include "SquareMatrix.hpp"
#include "RectangularMatrix.hpp"
#include "Deque.hpp"
#include <string>
#include <functional>
#include <complex>

// Вспомогательные функции для тестов
bool isEven(const int& x) { return x % 2 == 0; }
int multiplyByTwo(const int& x) { return x * 2; }
int sum(const int& a, const int& b) { return a + b; }

// Тесты для Stack
TEST(StackTest, BasicOperations) {
    Stack<int> stack;
    
    EXPECT_TRUE(stack.IsEmpty());
    EXPECT_EQ(stack.GetSize(), 0);
    EXPECT_TRUE(stack.Top().isNone());
    EXPECT_TRUE(stack.Pop().isNone());
    
    stack.Push(1);
    EXPECT_FALSE(stack.IsEmpty());
    EXPECT_EQ(stack.GetSize(), 1);
    EXPECT_TRUE(stack.Top().isSome());
    EXPECT_EQ(stack.Top().getValue(), 1);
    
    stack.Push(2);
    stack.Push(3);
    EXPECT_EQ(stack.GetSize(), 3);
    EXPECT_EQ(stack.Top().getValue(), 3);
    
    EXPECT_EQ(stack.Pop().getValue(), 3);
    EXPECT_EQ(stack.GetSize(), 2);
    EXPECT_EQ(stack.Pop().getValue(), 2);
    EXPECT_EQ(stack.Pop().getValue(), 1);
    EXPECT_TRUE(stack.IsEmpty());
}

TEST(StackTest, DifferentTypes) {
    Stack<int> intStack;
    intStack.Push(42);
    EXPECT_EQ(intStack.Top().getValue(), 42);
    
    Stack<double> doubleStack;
    doubleStack.Push(3.14);
    EXPECT_DOUBLE_EQ(doubleStack.Top().getValue(), 3.14);
    
    Stack<std::string> stringStack;
    stringStack.Push("test");
    EXPECT_EQ(stringStack.Top().getValue(), "test");
    
    Stack<std::complex<double>> complexStack;
    complexStack.Push(std::complex<double>(1.0, 2.0));
    auto complexValue = complexStack.Top().getValue();
    EXPECT_DOUBLE_EQ(complexValue.real(), 1.0);
    EXPECT_DOUBLE_EQ(complexValue.imag(), 2.0);
}

TEST(StackTest, HigherOrderFunctions) {
    Stack<int> stack;
    stack.Push(1);
    stack.Push(2);
    stack.Push(3);
    
    auto* mapped = stack.Map(multiplyByTwo);
    EXPECT_EQ(mapped->Get(0), 2);
    EXPECT_EQ(mapped->Get(1), 4);
    EXPECT_EQ(mapped->Get(2), 6);
    delete mapped;
    
    auto* filtered = stack.Where(isEven);
    EXPECT_EQ(filtered->GetLength(), 1);
    EXPECT_EQ(filtered->Get(0), 2);
    delete filtered;
    
    int result = stack.Reduce(sum, 0);
    EXPECT_EQ(result, 6);
}

TEST(StackTest, CustomClassOperations) {
    Stack<Person> personStack;
    time_t now = time(nullptr);
    
    Person person1(PersonID{1, 1}, "John", "", "Doe", now - 20 * 365 * 24 * 60 * 60);
    Person person2(PersonID{1, 2}, "Jane", "", "Smith", now - 25 * 365 * 24 * 60 * 60);
    
    personStack.Push(person1);
    personStack.Push(person2);
    
    EXPECT_EQ(personStack.GetSize(), 2);
    EXPECT_EQ(personStack.Top().getValue().GetFullName(), "Jane Smith");
    
    auto popped = personStack.Pop();
    EXPECT_TRUE(popped.isSome());
    EXPECT_EQ(popped.getValue().GetFullName(), "Jane Smith");
}

// Вспомогательные функции для тестирования функциональных типов
int AddOne(int x) { return x + 1; }
int MultiplyByTwo(int x) { return x * 2; }
using UnaryFunction = int (*)(int);

TEST(DequeTest, AllDataTypes) {
    // Тест для целых чисел
    Deque<int> intDeque;
    intDeque.PushBack(1);
    intDeque.PushBack(2);
    EXPECT_EQ(intDeque.Get(0), 1);
    EXPECT_EQ(intDeque.Get(1), 2);

    // Тест для вещественных чисел
    Deque<double> doubleDeque;
    doubleDeque.PushBack(1.5);
    doubleDeque.PushBack(2.7);
    EXPECT_DOUBLE_EQ(doubleDeque.Get(0), 1.5);
    EXPECT_DOUBLE_EQ(doubleDeque.Get(1), 2.7);

    // Тест для комплексных чисел
    Deque<Complex> complexDeque;
    complexDeque.PushBack(Complex(1.0, 2.0));
    complexDeque.PushBack(Complex(3.0, 4.0));
    EXPECT_EQ(complexDeque.Get(0), Complex(1.0, 2.0));
    EXPECT_EQ(complexDeque.Get(1), Complex(3.0, 4.0));

    // Тест для строк
    Deque<std::string> stringDeque;
    stringDeque.PushBack("Hello");
    stringDeque.PushBack("World");
    EXPECT_EQ(stringDeque.Get(0), "Hello");
    EXPECT_EQ(stringDeque.Get(1), "World");

    // Тест для функций
    Deque<UnaryFunction> functionDeque;
    functionDeque.PushBack(AddOne);
    functionDeque.PushBack(MultiplyByTwo);
    EXPECT_EQ(functionDeque.Get(0)(5), 6);      // AddOne(5)
    EXPECT_EQ(functionDeque.Get(1)(5), 10);     // MultiplyByTwo(5)
}

TEST(DequeTest, StudentsAndTeachers) {
    time_t now = time(nullptr);
    
    // Тест для студентов
    Deque<Student> studentDeque;
    Student s1(PersonID{1, 1}, "Alice", "", "Smith", now - 20 * 365 * 24 * 60 * 60);
    Student s2(PersonID{1, 2}, "Bob", "", "Johnson", now - 19 * 365 * 24 * 60 * 60);
    
    studentDeque.PushBack(s1);
    studentDeque.PushBack(s2);
    EXPECT_EQ(studentDeque.Get(0).GetFullName(), "Alice Smith");
    EXPECT_EQ(studentDeque.Get(1).GetFullName(), "Bob Johnson");
    
    // Тест методов с предикатами для студентов
    auto hasIdOne = [](const Student& s) { return s.GetID().number == 1; };
    auto* filtered = studentDeque.Where(hasIdOne);
    EXPECT_EQ(filtered->GetLength(), 1);
    EXPECT_EQ(filtered->Get(0).GetFullName(), "Alice Smith");
    delete filtered;
    
    // Тест для преподавателей
    Deque<Teacher> teacherDeque;
    Teacher t1(PersonID{2, 1}, "John", "M", "Smith", now - 40 * 365 * 24 * 60 * 60);
    Teacher t2(PersonID{2, 2}, "Mary", "", "Johnson", now - 35 * 365 * 24 * 60 * 60);
    
    teacherDeque.PushBack(t1);
    teacherDeque.PushBack(t2);
    EXPECT_EQ(teacherDeque.Get(0).GetFullName(), "John M Smith");
    EXPECT_EQ(teacherDeque.Get(1).GetFullName(), "Mary Johnson");
    
    // Проверяем сортировку
    teacherDeque.Sort();  // Сортировка по ФИО (определено в операторе < класса Person)
    EXPECT_EQ(teacherDeque.Get(0).GetFullName(), "John M Smith");
    EXPECT_EQ(teacherDeque.Get(1).GetFullName(), "Mary Johnson");
}

TEST(DequeTest, ComplexOperationsWithTypes) {
    // Тест конкатенации деков с комплексными числами
    Deque<Complex> deque1;
    deque1.PushBack(Complex(1, 1));
    deque1.PushBack(Complex(2, 2));
    
    Deque<Complex> deque2;
    deque2.PushBack(Complex(3, 3));
    deque2.PushBack(Complex(4, 4));
    
    auto* merged = deque1.Concat(&deque2);
    EXPECT_EQ(merged->GetSize(), 4);
    EXPECT_EQ(merged->Get(0), Complex(1, 1));
    EXPECT_EQ(merged->Get(3), Complex(4, 4));
    delete merged;
    
    // Тест поиска подпоследовательности для строк
    Deque<std::string> strDeque;
    strDeque.PushBack("First");
    strDeque.PushBack("Second");
    strDeque.PushBack("Third");
    
    ListSequence<std::string> subseq;
    subseq.Append("Second");
    subseq.Append("Third");
    EXPECT_TRUE(strDeque.ContainsSubsequence(&subseq));
    
    // Тест Split для функций
    Deque<UnaryFunction> funcDeque;
    funcDeque.PushBack(AddOne);
    funcDeque.PushBack(MultiplyByTwo);
    funcDeque.PushBack(AddOne);
    
    auto isAddOne = [](const UnaryFunction& f) { return f(0) == 1; };
    auto [addOneFuncs, otherFuncs] = funcDeque.Split(isAddOne);
    
    EXPECT_EQ(addOneFuncs->GetSize(), 2);  // Две функции AddOne
    EXPECT_EQ(otherFuncs->GetSize(), 1);   // Одна функция MultiplyByTwo
    EXPECT_EQ(addOneFuncs->Get(0)(5), 6);  // AddOne(5)
    EXPECT_EQ(otherFuncs->Get(0)(5), 10);  // MultiplyByTwo(5)
    
    delete addOneFuncs;
    delete otherFuncs;
}

// Вспомогательные функции для тестов Queue
int queueMultiplyByTwo(const int& x) { return x * 2; }
int queueSum(const int& a, const int& b) { return a + b; }
std::string queueToUpperCase(const std::string& str) {
    std::string result = str;
    for (char& c : result) c = std::toupper(c);
    return result;
}

// Тесты для Queue
TEST(QueueTest, BasicOperations) {
    Queue<int> queue;
    
    EXPECT_TRUE(queue.IsEmpty());
    EXPECT_EQ(queue.GetSize(), 0);
    EXPECT_TRUE(queue.Front().isNone());
    EXPECT_TRUE(queue.Dequeue().isNone());
    
    queue.Enqueue(1);
    EXPECT_FALSE(queue.IsEmpty());
    EXPECT_EQ(queue.GetSize(), 1);
    EXPECT_TRUE(queue.Front().isSome());
    EXPECT_EQ(queue.Front().getValue(), 1);
    
    queue.Enqueue(2);
    queue.Enqueue(3);
    EXPECT_EQ(queue.GetSize(), 3);
    EXPECT_EQ(queue.Front().getValue(), 1);
    
    EXPECT_EQ(queue.Dequeue().getValue(), 1);
    EXPECT_EQ(queue.GetSize(), 2);
    EXPECT_EQ(queue.Dequeue().getValue(), 2);
    EXPECT_EQ(queue.Dequeue().getValue(), 3);
    EXPECT_TRUE(queue.IsEmpty());
}

TEST(QueueTest, DifferentTypes) {
    Queue<int> intQueue;
    intQueue.Enqueue(42);
    EXPECT_EQ(intQueue.Front().getValue(), 42);
    
    Queue<double> doubleQueue;
    doubleQueue.Enqueue(3.14);
    EXPECT_DOUBLE_EQ(doubleQueue.Front().getValue(), 3.14);
    
    Queue<std::string> stringQueue;
    stringQueue.Enqueue("test");
    EXPECT_EQ(stringQueue.Front().getValue(), "test");
    
    Queue<std::complex<double>> complexQueue;
    complexQueue.Enqueue(std::complex<double>(1.0, 2.0));
    auto complexValue = complexQueue.Front().getValue();
    EXPECT_DOUBLE_EQ(complexValue.real(), 1.0);
    EXPECT_DOUBLE_EQ(complexValue.imag(), 2.0);
}

TEST(QueueTest, HigherOrderFunctions) {
    Queue<int> queue;
    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);
    
    auto* mapped = queue.Map(queueMultiplyByTwo);
    EXPECT_EQ(mapped->Get(0), 2);
    EXPECT_EQ(mapped->Get(1), 4);
    EXPECT_EQ(mapped->Get(2), 6);
    delete mapped;
    
    auto* evens = queue.Where(isEven);
    EXPECT_EQ(evens->GetLength(), 1);
    EXPECT_EQ(evens->Get(0), 2);
    delete evens;
    
    int result = queue.Reduce(queueSum, 0);
    EXPECT_EQ(result, 6);
}

TEST(QueueTest, CustomClassOperations) {
    Queue<Person> personQueue;
    time_t now = time(nullptr);
    
    Person person1(PersonID{1, 1}, "John", "", "Doe", now - 20 * 365 * 24 * 60 * 60);
    Person person2(PersonID{1, 2}, "Jane", "", "Smith", now - 25 * 365 * 24 * 60 * 60);
    
    personQueue.Enqueue(person1);
    personQueue.Enqueue(person2);
    
    EXPECT_EQ(personQueue.GetSize(), 2);
    EXPECT_EQ(personQueue.Front().getValue().GetFullName(), "John Doe");
    
    auto dequeued = personQueue.Dequeue();
    EXPECT_TRUE(dequeued.isSome());
    EXPECT_EQ(dequeued.getValue().GetFullName(), "John Doe");
}

TEST(QueueTest, SplitOperations) {
    Queue<int> queue;
    queue.Enqueue(1);
    queue.Enqueue(2);
    queue.Enqueue(3);
    queue.Enqueue(4);
    
    auto [evens, odds] = queue.Split(isEven);
    
    EXPECT_EQ(evens->GetSize(), 2);
    EXPECT_EQ(odds->GetSize(), 2);
    
    EXPECT_EQ(evens->Dequeue().getValue(), 2);
    EXPECT_EQ(evens->Dequeue().getValue(), 4);
    
    EXPECT_EQ(odds->Dequeue().getValue(), 1);
    EXPECT_EQ(odds->Dequeue().getValue(), 3);
    
    delete evens;
    delete odds;
}

TEST(QueueTest, ComplexNumberOperations) {
    Queue<std::complex<double>> complexQueue;
    
    std::complex<double> c1(1.0, 2.0);
    std::complex<double> c2(3.0, 4.0);
    
    complexQueue.Enqueue(c1);
    complexQueue.Enqueue(c2);
    
    EXPECT_EQ(complexQueue.GetSize(), 2);
    
    auto front = complexQueue.Front().getValue();
    EXPECT_DOUBLE_EQ(front.real(), 1.0);
    EXPECT_DOUBLE_EQ(front.imag(), 2.0);
    
    auto dequeued = complexQueue.Dequeue().getValue();
    EXPECT_DOUBLE_EQ(dequeued.real(), 1.0);
    EXPECT_DOUBLE_EQ(dequeued.imag(), 2.0);
}

// Тесты для PriorityQueue
TEST(PriorityQueueTest, BasicOperations) {
    PriorityQueue<int> pq;
    
    EXPECT_TRUE(pq.IsEmpty());
    EXPECT_EQ(pq.GetSize(), 0);
    EXPECT_TRUE(pq.Front().isNone());
    EXPECT_TRUE(pq.Dequeue().isNone());
    
    pq.Enqueue(1, 1);  // value 1, priority 1
    EXPECT_FALSE(pq.IsEmpty());
    EXPECT_EQ(pq.GetSize(), 1);
    EXPECT_TRUE(pq.Front().isSome());
    EXPECT_EQ(pq.Front().getValue(), 1);
    
    pq.Enqueue(2, 2);  // value 2, priority 2
    pq.Enqueue(3, 0);  // value 3, priority 0
    EXPECT_EQ(pq.GetSize(), 3);
    EXPECT_EQ(pq.Front().getValue(), 2);  // highest priority (2)
    
    EXPECT_EQ(pq.Dequeue().getValue(), 2);  // highest priority first
    EXPECT_EQ(pq.GetSize(), 2);
    EXPECT_EQ(pq.Dequeue().getValue(), 1);  // then priority 1
    EXPECT_EQ(pq.Dequeue().getValue(), 3);  // then priority 0
    EXPECT_TRUE(pq.IsEmpty());
}

TEST(PriorityQueueTest, ComplexNumbers) {
    PriorityQueue<Complex> pq;
    
    Complex c1(1.0, 2.0);
    Complex c2(3.0, 4.0);
    Complex c3(0.0, 1.0);
    
    pq.Enqueue(c1, 2);  // priority 2
    pq.Enqueue(c2, 1);  // priority 1
    pq.Enqueue(c3, 3);  // priority 3
    
    EXPECT_EQ(pq.GetSize(), 3);
    
    auto front = pq.Front().getValue();
    EXPECT_DOUBLE_EQ(front.GetReal(), 0.0);  // c3 has highest priority
    EXPECT_DOUBLE_EQ(front.GetImag(), 1.0);
    
    auto first = pq.Dequeue().getValue();  // should get c3 (priority 3)
    EXPECT_DOUBLE_EQ(first.GetReal(), 0.0);
    EXPECT_DOUBLE_EQ(first.GetImag(), 1.0);
    
    auto second = pq.Dequeue().getValue();  // should get c1 (priority 2)
    EXPECT_DOUBLE_EQ(second.GetReal(), 1.0);
    EXPECT_DOUBLE_EQ(second.GetImag(), 2.0);
}

TEST(PriorityQueueTest, PersonOperations) {
    PriorityQueue<Person> pq;
    time_t now = time(nullptr);
    
    Person person1(PersonID{1, 1}, "John", "", "Doe", now - 20 * 365 * 24 * 60 * 60);
    Person person2(PersonID{1, 2}, "Jane", "", "Smith", now - 25 * 365 * 24 * 60 * 60);
    Person person3(PersonID{1, 3}, "Alice", "", "Brown", now - 30 * 365 * 24 * 60 * 60);
    
    pq.Enqueue(person1, 2);  // John Doe with priority 2
    pq.Enqueue(person2, 3);  // Jane Smith with priority 3
    pq.Enqueue(person3, 1);  // Alice Brown with priority 1
    
    EXPECT_EQ(pq.GetSize(), 3);
    EXPECT_EQ(pq.Front().getValue().GetFullName(), "Jane Smith");  // highest priority
    
    auto first = pq.Dequeue().getValue();
    EXPECT_EQ(first.GetFullName(), "Jane Smith");  // priority 3
    
    auto second = pq.Dequeue().getValue();
    EXPECT_EQ(second.GetFullName(), "John Doe");   // priority 2
}

TEST(PriorityQueueTest, HigherOrderFunctions) {
    PriorityQueue<int> pq;
    pq.Enqueue(1, 2);
    pq.Enqueue(2, 1);
    pq.Enqueue(3, 3);
    
    // Test Map
    auto* mapped = pq.Map(multiplyByTwo);
    EXPECT_EQ(mapped->Get(0), 6);  // 3 * 2 (highest priority)
    EXPECT_EQ(mapped->Get(1), 2);  // 1 * 2
    EXPECT_EQ(mapped->Get(2), 4);  // 2 * 2
    delete mapped;
    
    // Test Where
    auto* filtered = pq.Where(isEven);
    EXPECT_EQ(filtered->GetLength(), 1);
    EXPECT_EQ(filtered->Get(0), 2);
    delete filtered;
    
    // Test Reduce
    int result = pq.Reduce(sum, 0);
    EXPECT_EQ(result, 6);  // 1 + 2 + 3
}

TEST(PriorityQueueTest, Subsequences) {
    PriorityQueue<int> pq;
    pq.Enqueue(1, 3);
    pq.Enqueue(2, 2);
    pq.Enqueue(3, 1);
    pq.Enqueue(4, 4);
    
    auto* subseq = pq.GetSubsequence(1, 2);
    EXPECT_EQ(subseq->GetLength(), 2);
    EXPECT_EQ(subseq->Get(0), 1);  // Second highest priority
    EXPECT_EQ(subseq->Get(1), 2);  // Third highest priority
    delete subseq;
    
    ListSequence<int> searchSeq;
    searchSeq.Append(1);
    searchSeq.Append(2);
    EXPECT_TRUE(pq.ContainsSubsequence(&searchSeq));
}

TEST(PriorityQueueTest, SplitOperations) {
    PriorityQueue<int> pq;
    pq.Enqueue(1, 2);
    pq.Enqueue(2, 1);
    pq.Enqueue(3, 3);
    pq.Enqueue(4, 4);
    
    auto [evens, odds] = pq.Split(isEven);
    
    EXPECT_EQ(evens->GetSize(), 2);
    EXPECT_EQ(odds->GetSize(), 2);
    
    EXPECT_EQ(evens->Dequeue().getValue(), 4);  // Highest priority even
    EXPECT_EQ(evens->Dequeue().getValue(), 2);  // Lower priority even
    
    EXPECT_EQ(odds->Dequeue().getValue(), 3);   // Highest priority odd
    EXPECT_EQ(odds->Dequeue().getValue(), 1);   // Lower priority odd
    
    delete evens;
    delete odds;
}

TEST(PriorityQueueTest, ConcatOperation) {
    PriorityQueue<int> pq1;
    pq1.Enqueue(1, 1);
    pq1.Enqueue(2, 2);
    
    PriorityQueue<int> pq2;
    pq2.Enqueue(3, 3);
    pq2.Enqueue(4, 4);
    
    auto* combined = pq1.Concat(&pq2);
    EXPECT_EQ(combined->GetSize(), 4);
    EXPECT_EQ(combined->Dequeue().getValue(), 4);  // Highest priority
    EXPECT_EQ(combined->Dequeue().getValue(), 3);
    EXPECT_EQ(combined->Dequeue().getValue(), 2);
    EXPECT_EQ(combined->Dequeue().getValue(), 1);  // Lowest priority
    
    delete combined;
}

// Тесты для Vector
TEST(VectorTest, BasicOperations) {
    // Тест для целых чисел
    Vector<int> v1(3);
    v1.Set(0, 1);
    v1.Set(1, 2);
    v1.Set(2, 3);

    Vector<int> v2(3);
    v2.Set(0, 4);
    v2.Set(1, 5);
    v2.Set(2, 6);

    // Тест сложения
    auto sum = v1 + v2;
    EXPECT_EQ(sum.Get(0), 5);
    EXPECT_EQ(sum.Get(1), 7);
    EXPECT_EQ(sum.Get(2), 9);

    // Тест умножения на скаляр
    auto scaled = v1 * 2;
    EXPECT_EQ(scaled.Get(0), 2);
    EXPECT_EQ(scaled.Get(1), 4);
    EXPECT_EQ(scaled.Get(2), 6);

    // Тест скалярного произведения
    EXPECT_EQ(v1.DotProduct(v2), 32);
}

TEST(VectorTest, ComplexNumbers) {
    Vector<Complex> v1(2);
    v1.Set(0, Complex(1, 1));
    v1.Set(1, Complex(2, 2));

    Vector<Complex> v2(2);
    v2.Set(0, Complex(1, -1));
    v2.Set(1, Complex(2, -2));

    // Тест сложения комплексных векторов
    auto sum = v1 + v2;
    EXPECT_EQ(sum.Get(0), Complex(2, 0));
    EXPECT_EQ(sum.Get(1), Complex(4, 0));
}

// Тесты для SquareMatrix
TEST(SquareMatrixTest, BasicOperations) {
    SquareMatrix<int> m1(2);
    m1.Set(0, 0, 1);
    m1.Set(0, 1, 2);
    m1.Set(1, 0, 3);
    m1.Set(1, 1, 4);

    SquareMatrix<int> m2(2);
    m2.Set(0, 0, 5);
    m2.Set(0, 1, 6);
    m2.Set(1, 0, 7);
    m2.Set(1, 1, 8);

    // Тест сложения
    auto sum = m1 + m2;
    EXPECT_EQ(sum.Get(0, 0), 6);
    EXPECT_EQ(sum.Get(0, 1), 8);
    EXPECT_EQ(sum.Get(1, 0), 10);
    EXPECT_EQ(sum.Get(1, 1), 12);

    // Тест элементарных преобразований
    m1.MultiplyRow(0, 2);
    EXPECT_EQ(m1.Get(0, 0), 2);
    EXPECT_EQ(m1.Get(0, 1), 4);
}

TEST(SquareMatrixTest, ComplexNumbers) {
    SquareMatrix<Complex> m1(2);
    m1.Set(0, 0, Complex(1, 1));
    m1.Set(0, 1, Complex(2, 2));
    m1.Set(1, 0, Complex(3, 3));
    m1.Set(1, 1, Complex(4, 4));

    SquareMatrix<Complex> m2(2);
    m2.Set(0, 0, Complex(1, -1));
    m2.Set(0, 1, Complex(2, -2));
    m2.Set(1, 0, Complex(3, -3));
    m2.Set(1, 1, Complex(4, -4));

    // Тест сложения
    auto sum = m1 + m2;
    EXPECT_EQ(sum.Get(0, 0), Complex(2, 0));
    EXPECT_EQ(sum.Get(0, 1), Complex(4, 0));
    EXPECT_EQ(sum.Get(1, 0), Complex(6, 0));
    EXPECT_EQ(sum.Get(1, 1), Complex(8, 0));

    // Тест умножения на комплексное число
    auto scaled = m1 * Complex(2, 0);
    EXPECT_EQ(scaled.Get(0, 0), Complex(2, 2));
    EXPECT_EQ(scaled.Get(0, 1), Complex(4, 4));
    EXPECT_EQ(scaled.Get(1, 0), Complex(6, 6));
    EXPECT_EQ(scaled.Get(1, 1), Complex(8, 8));
}

TEST(RectangularMatrixTest, BasicOperations) {
    RectangularMatrix<double> m1(2, 3);
    m1.Set(0, 0, 1.0);
    m1.Set(0, 1, 2.0);
    m1.Set(0, 2, 3.0);
    m1.Set(1, 0, 4.0);
    m1.Set(1, 1, 5.0);
    m1.Set(1, 2, 6.0);

    // Тест получения строки и столбца
    auto row = m1.GetRow(0);
    EXPECT_EQ(row.Get(0), 1.0);
    EXPECT_EQ(row.Get(1), 2.0);
    EXPECT_EQ(row.Get(2), 3.0);

    auto col = m1.GetColumn(1);
    EXPECT_EQ(col.Get(0), 2.0);
    EXPECT_EQ(col.Get(1), 5.0);

    // Тест умножения на скаляр
    auto scaled = m1 * 2.0;
    EXPECT_EQ(scaled.Get(0, 0), 2.0);
    EXPECT_EQ(scaled.Get(0, 1), 4.0);
    EXPECT_EQ(scaled.Get(0, 2), 6.0);
}

TEST(RectangularMatrixTest, ComplexNumbers) {
    RectangularMatrix<Complex> m1(2, 3);
    m1.Set(0, 0, Complex(1, 1));
    m1.Set(0, 1, Complex(2, 2));
    m1.Set(0, 2, Complex(3, 3));
    m1.Set(1, 0, Complex(4, 4));
    m1.Set(1, 1, Complex(5, 5));
    m1.Set(1, 2, Complex(6, 6));

    // Тест получения строки
    auto row = m1.GetRow(0);
    EXPECT_EQ(row.Get(0), Complex(1, 1));
    EXPECT_EQ(row.Get(1), Complex(2, 2));
    EXPECT_EQ(row.Get(2), Complex(3, 3));

    // Тест получения столбца
    auto col = m1.GetColumn(1);
    EXPECT_EQ(col.Get(0), Complex(2, 2));
    EXPECT_EQ(col.Get(1), Complex(5, 5));

    // Тест умножения на комплексное число
    auto scaled = m1 * Complex(2, 0);
    EXPECT_EQ(scaled.Get(0, 0), Complex(2, 2));
    EXPECT_EQ(scaled.Get(0, 1), Complex(4, 4));
    EXPECT_EQ(scaled.Get(0, 2), Complex(6, 6));
}

// Вспомогательная функция для сортировки в обратном порядке
bool reverseCompare(const int& a, const int& b) {
    return a > b;
}

TEST(DequeTest, SortingOperations) {
    Deque<int> deque;
    deque.PushBack(3);
    deque.PushBack(1);
    deque.PushBack(4);
    deque.PushBack(2);

    // Тест обычной сортировки
    deque.Sort();
    EXPECT_EQ(deque.Get(0), 1);
    EXPECT_EQ(deque.Get(1), 2);
    EXPECT_EQ(deque.Get(2), 3);
    EXPECT_EQ(deque.Get(3), 4);

    // Тест сортировки с компаратором
    deque.Sort(reverseCompare);
    EXPECT_EQ(deque.Get(0), 4);
    EXPECT_EQ(deque.Get(1), 3);
    EXPECT_EQ(deque.Get(2), 2);
    EXPECT_EQ(deque.Get(3), 1);
}

TEST(DequeTest, SubsequenceOperations) {
    Deque<int> deque;
    deque.PushBack(1);
    deque.PushBack(2);
    deque.PushBack(3);
    deque.PushBack(4);
    deque.PushBack(5);

    // Тест поиска существующей подпоследовательности
    ListSequence<int> subseq1;
    subseq1.Append(2);
    subseq1.Append(3);
    EXPECT_TRUE(deque.ContainsSubsequence(&subseq1));

    // Тест поиска несуществующей подпоследовательности
    ListSequence<int> subseq2;
    subseq2.Append(2);
    subseq2.Append(5);
    EXPECT_FALSE(deque.ContainsSubsequence(&subseq2));

    // Тест с пустой подпоследовательностью
    ListSequence<int> emptySubseq;
    EXPECT_TRUE(deque.ContainsSubsequence(&emptySubseq));
}

TEST(DequeTest, MergeOperations) {
    Deque<int> deque1;
    deque1.PushBack(1);
    deque1.PushBack(3);
    deque1.PushBack(5);

    Deque<int> deque2;
    deque2.PushBack(2);
    deque2.PushBack(4);
    deque2.PushBack(6);

    // Тест обычного слияния
    auto* merged = Deque<int>::Merge(&deque1, &deque2);
    EXPECT_EQ(merged->GetSize(), 6);
    EXPECT_EQ(merged->Get(0), 1);
    EXPECT_EQ(merged->Get(1), 2);
    EXPECT_EQ(merged->Get(2), 3);
    EXPECT_EQ(merged->Get(3), 4);
    EXPECT_EQ(merged->Get(4), 5);
    EXPECT_EQ(merged->Get(5), 6);
    delete merged;

    // Тест слияния с компаратором (в обратном порядке)
    auto* mergedReverse = Deque<int>::Merge(&deque1, &deque2, reverseCompare);
    EXPECT_EQ(mergedReverse->GetSize(), 6);
    EXPECT_EQ(mergedReverse->Get(0), 5);
    EXPECT_EQ(mergedReverse->Get(1), 6);
    EXPECT_EQ(mergedReverse->Get(2), 3);
    EXPECT_EQ(mergedReverse->Get(3), 4);
    EXPECT_EQ(mergedReverse->Get(4), 1);
    EXPECT_EQ(mergedReverse->Get(5), 2);
    delete mergedReverse;
}