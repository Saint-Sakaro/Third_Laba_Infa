#pragma once

#include <string>
#include <ctime>
#include <iostream>

struct PersonID
{
    int type;
    int number;

    bool operator==(const PersonID& other) const
    {
        return type == other.type && number == other.number;
    }
};

class Person
{
private:
    PersonID id;
    std::string firstName;
    std::string middleName;
    std::string lastName;
    time_t birthDate;

public:
    Person() : id{0, 0}, birthDate(0) {}

    Person(PersonID id, std::string first, std::string middle, std::string last, time_t birth)
        : id(id)
        , firstName(std::move(first))
        , middleName(std::move(middle))
        , lastName(std::move(last))
        , birthDate(birth)
    {
    }

    PersonID GetID() const
    {
        return id;
    }

    std::string GetFirstName() const
    {
        return firstName;
    }

    std::string GetMiddleName() const
    {
        return middleName;
    }

    std::string GetLastName() const
    {
        return lastName;
    }

    std::string GetFullName() const
    {
        std::string fullName = firstName;
        if (!middleName.empty()) {
            fullName += " " + middleName;
        }
        fullName += " " + lastName;
        return fullName;
    }

    time_t GetBirthDate() const
    {
        return birthDate;
    }

    int GetAge() const
    {
        time_t now = time(nullptr);
        struct tm* birth_tm = localtime(&birthDate);
        struct tm* now_tm = localtime(&now);
        
        int age = now_tm->tm_year - birth_tm->tm_year;
        if (now_tm->tm_mon < birth_tm->tm_mon ||
            (now_tm->tm_mon == birth_tm->tm_mon && now_tm->tm_mday < birth_tm->tm_mday)) {
            age--;
        }
        return age;
    }

    bool operator==(const Person& other) const
    {
        return id == other.id;
    }

    bool operator<(const Person& other) const
    {
        return GetFullName() < other.GetFullName();
    }

    void Print(std::ostream& os) const
    {
        os << GetFullName() << " (ID: " << id.type << "-" << id.number << ")";
    }
};

std::ostream& operator<<(std::ostream& os, const Person& person)
{
    person.Print(os);
    return os;
}

class Student : public Person
{
public:
    Student() : Person() {}

    Student(PersonID id, std::string first, std::string middle, std::string last, time_t birth)
        : Person(id, std::move(first), std::move(middle), std::move(last), birth)
    {
    }
};

class Teacher : public Person
{
public:
    Teacher() : Person() {}

    Teacher(PersonID id, std::string first, std::string middle, std::string last, time_t birth)
        : Person(id, std::move(first), std::move(middle), std::move(last), birth)
    {
    }
};