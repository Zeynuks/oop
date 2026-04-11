#include <gtest/gtest.h>
#include <string>
#include "../MyList.hpp"

TEST(MyListModificationTests, PushBack) {
    MyList<int> intList;
    intList.PushBack(1);
    intList.PushBack(2);
    EXPECT_EQ(intList.Size(), 2);
    EXPECT_EQ(intList.Back(), 2);

    MyList<std::string> stringList;
    stringList.PushBack("first");
    stringList.PushBack("second");
    EXPECT_EQ(stringList.Size(), 2);
    EXPECT_EQ(stringList.Back(), "second");
}

TEST(MyListModificationTests, PushFront) {
    MyList<int> intList;
    intList.PushFront(10);
    intList.PushFront(20);
    EXPECT_EQ(intList.Size(), 2);
    EXPECT_EQ(intList.Front(), 20);

    MyList<std::string> stringList;
    stringList.PushFront("tail");
    stringList.PushFront("head");
    EXPECT_EQ(stringList.Size(), 2);
    EXPECT_EQ(stringList.Front(), "head");
}

TEST(MyListModificationTests, Emplace) {
    MyList<int> intList;
    auto it = intList.Emplace(intList.begin(), 100);
    EXPECT_EQ(*it, 100);
    intList.Emplace(intList.end(), 300);
    intList.Emplace(++intList.begin(), 200);

    auto checkIt = intList.begin();
    EXPECT_EQ(*checkIt++, 100);
    EXPECT_EQ(*checkIt++, 200);
    EXPECT_EQ(*checkIt++, 300);

    MyList<std::string> stringList;
    stringList.Emplace(stringList.begin(), "middle");
    stringList.Emplace(stringList.begin(), "start");
    stringList.Emplace(stringList.end(), "end");

    auto sIt = stringList.begin();
    EXPECT_EQ(*sIt++, "start");
    EXPECT_EQ(*sIt++, "middle");
    EXPECT_EQ(*sIt++, "end");
}

TEST(MyListModificationTests, Erase) {
    MyList<int> intList;
    intList.PushBack(1);
    intList.PushBack(2);
    intList.PushBack(3);

    auto it = intList.Erase(++intList.begin());
    EXPECT_EQ(intList.Size(), 2);
    EXPECT_EQ(*it, 3);
    EXPECT_EQ(intList.Front(), 1);
    EXPECT_EQ(intList.Back(), 3);

    it = intList.Erase(intList.begin());
    EXPECT_EQ(intList.Front(), 3);
    
    intList.Erase(intList.begin());
    EXPECT_TRUE(intList.Empty());

    MyList<std::string> stringList;
    stringList.PushBack("A");
    stringList.PushBack("B");
    stringList.Erase(stringList.begin());
    EXPECT_EQ(stringList.Front(), "B");
}

TEST(MyListModificationTests, Clear) {
    MyList<int> intList;
    intList.Clear();
    EXPECT_TRUE(intList.Empty());

    intList.PushBack(1);
    intList.PushBack(2);
    intList.Clear();
    EXPECT_TRUE(intList.Empty());
    EXPECT_EQ(intList.Size(), 0);
    EXPECT_EQ(intList.begin(), intList.end());

    MyList<std::string> stringList;
    stringList.PushBack("data");
    stringList.Clear();
    EXPECT_TRUE(stringList.Empty());
}

TEST(MyListModificationTests, Assign) {
    std::vector<int> vec = {10, 20, 30};
    MyList<int> intList;
    intList.Assign(vec.begin(), vec.end());
    EXPECT_EQ(intList.Size(), 3);
    EXPECT_EQ(intList.Front(), 10);
    EXPECT_EQ(intList.Back(), 30);

    std::vector<std::string> sVec = {"one", "two"};
    MyList<std::string> stringList;
    stringList.PushBack("old");
    stringList.Assign(sVec.begin(), sVec.end());
    EXPECT_EQ(stringList.Size(), 2);
    EXPECT_EQ(stringList.Front(), "one");
}