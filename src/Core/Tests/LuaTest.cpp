// https://github.com/doctest/doctest/blob/master/doc/markdown/tutorial.md
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include <string>
#include <iostream>
#include <list>

#include <sol/sol.hpp>

#include "CoreConstants.h"

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