// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "catch.hpp"
#include "irbis.h"

#include <sstream>

#if defined(_MSC_VER)
#pragma warning(disable: 4068)
#endif

#pragma ide diagnostic ignored "bugprone-infinite-loop"

struct Address
{
    std::string *city;
};

struct Person
{
    Address *address;
};

TEST_CASE("Maybe_operator_1", "[maybe]")
{
    auto *address = new Address;
    address->city = new std::string ("Irkutsk");
    auto *person  = new Person;
    person->address = address;
    std::size_t counter { 0 };

    auto m = irbis::maybe (person)
             & [] (Person *p)  { return p->address; }
             & [] (Address *a) { return a->city; }
             | [&counter] (std::string *s) { counter = s->size (); };

    CHECK (counter == 7);

    std::ostringstream stream;
    stream << m;
    auto text = stream.str ();
    CHECK (text == "Irkutsk");
}

TEST_CASE("Maybe_operator_2", "[maybe]")
{
    auto *address = new Address;
    address->city = nullptr;
    auto *person  = new Person;
    person->address = address;
    std::size_t counter { 0 };

    auto m = irbis::maybe (person)
             & [] (Person *p)  { return p->address; }
             & [] (Address *a) { return a->city; }
             | [&counter] (std::string *s) { counter = s->size (); };

    CHECK (counter == 0);

    std::ostringstream stream;
    stream << m;
    auto text = stream.str ();
    CHECK (text.empty ());
}

TEST_CASE("Maybe_operator_3", "[maybe]")
{
    auto *person = new Person;
    person->address = nullptr;
    std::size_t counter { 0 };

    auto m = irbis::maybe (person)
             & [] (Person *p)  { return p->address; }
             & [] (Address *a) { return a->city; }
             | [&counter] (std::string *s) { counter = s->size (); };

    CHECK (counter == 0);

    std::ostringstream stream;
    stream << m;
    auto text = stream.str ();
    CHECK (text.empty ());
}

TEST_CASE("Maybe_operator_4", "[maybe]")
{
    Person *person = nullptr;
    std::size_t counter { 0 };

    auto m = irbis::maybe (person)
             & [] (Person *p) { return p->address; }
             & [] (Address *a) { return a->city; }
             | [&counter] (std::string *s) { counter = s->size (); };

    CHECK (counter == 0);

    std::ostringstream stream;
    stream << m;
    auto text = stream.str ();
    CHECK (text.empty ());
}

TEST_CASE("Maybe_copy_1", "[maybe]")
{
    auto *address   = new Address;
    address->city   = new std::string ("Irkutsk");
    auto *person    = new Person;
    person->address = address;

    auto maybe1 = irbis::maybe (person);
    auto maybe2 = maybe1;
    CHECK (maybe1.context == maybe2.context);
}

TEST_CASE("Maybe_copy_2", "[maybe]")
{
    auto *address   = new Address;
    address->city   = new std::string ("Irkutsk");
    auto *person    = new Person;
    person->address = address;

    auto maybe1 = irbis::maybe (person);
    auto maybe2 (maybe1);
    CHECK (maybe1.context == maybe2.context);
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-use-after-move"

TEST_CASE("Maybe_move_1", "[maybe]")
{
    auto *address   = new Address;
    address->city   = new std::string ("Irkutsk");
    auto *person    = new Person;
    person->address = address;

    auto maybe1 = irbis::maybe (person);
    auto maybe2 (std::move (maybe1));
    CHECK (maybe1.context == nullptr);
    CHECK (maybe2.context == person);
}

TEST_CASE("Maybe_move_2", "[maybe]")
{
    auto *address   = new Address;
    address->city   = new std::string ("Irkutsk");
    auto *person    = new Person;
    person->address = address;

    auto maybe1 = irbis::maybe (person);
    auto maybe2 = std::move (maybe1);
    CHECK (maybe1.context == nullptr);
    CHECK (maybe2.context == person);
}

#pragma clang diagnostic pop

TEST_CASE("Maybe_Optional_1", "[maybe]")
{
    auto *address   = new Address;
    address->city   = new std::string ("Irkutsk");
    auto *person    = new Person;
    person->address = address;

    irbis::Optional <Person *> anOptional (person);
    auto maybe = irbis::maybe (anOptional);
    CHECK (maybe.context == person);
}
