// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <iostream>
#include <list>
#include <memory>

#include <sol/sol.hpp>

class Cat
{
public:
    Cat() : Name("Unnamed")
    {
    }

    Cat(const std::string &name) : Name(name)
    {
    }

    virtual ~Cat()
    {
    }

public:
    void SetName(const std::string &name)
    {
        Name = name;
    }
    const std::string &GetName()
    {
        return Name;
    }
    void Eat(const std::list<std::string> &foods)
    {
        std::cout << "The cat ate ";
        for (auto f : foods)
        {
            std::cout << f << ", ";
        }
        std::cout << std::endl;
    }
    static void Speak(const std::string &w)
    {
        std::cout << "Miaaau~ " << w;
    }

private:
    std::string Name;
};

class Cat2
{
public:
    Cat2() : Name("Unnamed")
    {
    }

    Cat2(const std::string &name) : Name(name)
    {
    }

    virtual ~Cat2()
    {
    }

public:
    struct deleter
    {
        void operator()(Cat2 *p) const
        {
            destroy(*p);
        }
    };

    static void Delete(Cat2 *p)
    {
        destroy(*p);
    }

    static std::shared_ptr<Cat2> create()
    {
        return std::shared_ptr<Cat2>(new Cat2("UNNAMED"), Delete);
    }

    static void initialize(Cat2 &uninitialized_memory)
    {
        new (&uninitialized_memory) Cat2();
    }

    static void destroy(Cat2 &memoryFromLua)
    {
        memoryFromLua.~Cat2();
    }

    void SetName(const std::string &name)
    {
        Name = name;
    }
    const std::string &GetName()
    {
        return Name;
    }
    void Eat(const std::list<std::string> &foods)
    {
        std::cout << "The cat ate ";
        for (auto f : foods)
        {
            std::cout << f << ", ";
        }
        std::cout << std::endl;
    }
    static void Speak(const std::string &w)
    {
        std::cout << "Miaaau~ " << w;
    }

private:
    std::string Name;
};

TEST_CASE("LUA script")
{
    sol::state lua;
    int x = 0;
    lua.set_function("beep", [&x]
                     { ++x; });
    lua.script("beep()");
    CHECK(x == 1);
}

TEST_CASE("LUA Class")
{
    sol::state lua;
    lua.new_usertype<Cat>("Cat", "GetName", &Cat::GetName, "SetName", &Cat::SetName);
    lua.script("cat1 = Cat.new();");
    lua.script("cat1:SetName(\"Kirin\");");
    Cat &cat1 = lua["cat1"];
    CHECK(cat1.GetName() == "Kirin");
    lua.script("cat1:SetName(\"Cafe\");");
    CHECK(cat1.GetName() == "Cafe");
    //  lua.script_file(std::format("{}/Test.lua", CONST_DATA_FOLDER), sol::load_mode::text);
}

TEST_CASE("LUA Class pointer")
{
    sol::state lua;
    lua.new_usertype<Cat2>("Cat2",
                           "new", sol::initializers(&Cat2::initialize),
                           "create", sol::factories(&Cat2::create),
                           sol::meta_function::garbage_collect, sol::destructor(&Cat2::destroy),
                           "GetName", &Cat2::GetName,
                           "SetName", &Cat2::SetName);
    lua.script("cat2 = Cat2.create();");
    lua.script("cat2:SetName(\"Kirin\");");
    std::shared_ptr<Cat2> &cat2 = lua["cat2"];
    Cat2 *cat2ptr = lua["cat2"];
    CHECK(cat2->GetName() == "Kirin");
    CHECK(cat2ptr->GetName() == "Kirin");
    lua.script("cat2:SetName(\"Cafe\");");
    CHECK(cat2->GetName() == "Cafe");
    CHECK(cat2ptr->GetName() == "Cafe");
}