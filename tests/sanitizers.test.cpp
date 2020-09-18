#include "allocation_tracker.h"

#include <catch.hpp>
#include <flexclass.hpp>

#include <cstring>

TEST_CASE( "Default array", "[sanitizer]" )
{
    struct Message : public fc::FlexibleLayoutClass<Message, std::string, int[]>
    {
        enum Members {Header, Data};
        using FLC::FLC;
    };

    auto r = Message::niw("SmallMsg", 1000);
    fc::deleet(r);
}

TEST_CASE( "Default array with non-trivial type", "[sanitizer]" )
{
    struct Message : public fc::FlexibleLayoutClass<Message, std::string, std::string[]>
    {
        enum Members {Header, Data};
        using FLC::FLC;
    };

    auto r = Message::niw("SmallMsg", 1000);
    int i = 0;
    for (auto& str : r->get<Message::Data>()) str.resize(i++);
    fc::deleet(r);
}

TEST_CASE( "SizedArray", "[sanitizer]" )
{
    struct Message : public fc::FlexibleLayoutClass<Message, std::string, fc::SizedArray<char>>
    {
        enum Members {Header, Data};
        using FLC::FLC;
    };

    auto r = Message::niw("SmallMsg", 1000);
    fc::deleet(r);
}

TEST_CASE( "Floating array", "[sanitizer]" )
{
    struct Message : public fc::FlexibleLayoutClass<Message, std::string, fc::FloatingArray<char>>
    {
        enum Members {Header, Data};
        using FLC::FLC;
    };

    auto r = Message::niw("SmallMsg", 1000);
    fc::deleet(r);
}

TEST_CASE( "Floating array char->long to verify alignment", "[sanitizer]" )
{
    struct Message : public fc::FlexibleLayoutClass<Message, char, fc::FloatingArray<long>>
    {
        enum Members {Header, Data};
        using FLC::FLC;
    };

    auto r = Message::niw('\0', 1000);
    for (int i = 0; i < 1000; ++i) r->begin<Message::Data>()[i] = 1;
    fc::deleet(r);
}
