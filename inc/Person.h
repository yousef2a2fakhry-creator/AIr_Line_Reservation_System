#ifndef PERSON_H
#define PERSON_H
#include <string>
#include <optional>
class Person
{
protected:
    std::string name;
    std::string password;
    std::string email;
    int age;

public:
    Person(std::string name, std::string password, std::string email, int age);

    void setName(const std::string &name);
    void setPassword(const std::string &password);
    void setEmail(const std::string &email);
    void setAge(int age);

    std::string getName() const;
    std::string getPassword() const;
    std::string getEmail() const;
    int getAge() const;

    virtual std::string getID() const = 0;

    virtual void display() const = 0;
};

#endif
