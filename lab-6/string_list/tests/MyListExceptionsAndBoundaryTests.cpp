#include <gtest/gtest.h>
#include <string>
#include <stdexcept>
#include "../MyList.hpp"

struct ThrowOnCopyString {
    std::string data;
    static bool should_throw;

    ThrowOnCopyString(const std::string& str = "") : data(str) {}

    ThrowOnCopyString(const ThrowOnCopyString& other) {
        if (should_throw) {
            throw std::runtime_error("Simulated exception during copy");
        }
        data = other.data;
    }

    ThrowOnCopyString& operator=(const ThrowOnCopyString& other) {
        if (should_throw) {
            throw std::runtime_error("Simulated exception during assignment");
        }
        data = other.data;
        return *this;
    }
};

bool ThrowOnCopyString::should_throw = false;

TEST(MyListBoundaryTests, AccessEmptyListAsserts) {
    MyList<int> emptyIntList;
    EXPECT_DEATH(emptyIntList.Front(), ".*");
    EXPECT_DEATH(emptyIntList.Back(), ".*");

    MyList<std::string> emptyStrList;
    EXPECT_DEATH(emptyStrList.Front(), ".*");
    EXPECT_DEATH(emptyStrList.Back(), ".*");
}

TEST(MyListBoundaryTests, EraseEndIteratorAsserts) {
    MyList<int> list;
    list.PushBack(1);
    EXPECT_DEATH(list.Erase(list.end()), ".*");
}

TEST(MyListBoundaryTests, DereferenceNullIteratorAsserts) {
    MyList<std::string>::iterator nullIt(nullptr);
    EXPECT_DEATH(*nullIt, ".*");
	EXPECT_DEATH(nullIt.operator->(), ".*");
}

TEST(MyListExceptionSafetyTests, PushBackStrongGuarantee) {
    MyList<ThrowOnCopyString> list;
    list.PushBack(ThrowOnCopyString("first"));
    list.PushBack(ThrowOnCopyString("second"));
    
    size_t initialSize = list.Size();
    auto beginNode = list.begin();
    
    ThrowOnCopyString::should_throw = true;
    ThrowOnCopyString itemToFail("third");
    
    EXPECT_THROW(list.PushBack(itemToFail), std::runtime_error);
    
    ThrowOnCopyString::should_throw = false;

    EXPECT_EQ(list.Size(), initialSize);
    EXPECT_EQ(list.Front().data, "first");
    EXPECT_EQ(list.Back().data, "second");
    EXPECT_EQ(list.begin(), beginNode);
}

TEST(MyListExceptionSafetyTests, CopyConstructorStrongGuarantee) {
    MyList<ThrowOnCopyString> original;
    original.PushBack(ThrowOnCopyString("A"));
    original.PushBack(ThrowOnCopyString("B"));

    ThrowOnCopyString::should_throw = true;
    
    EXPECT_THROW({
        MyList<ThrowOnCopyString> copy(original);
    }, std::runtime_error);

    ThrowOnCopyString::should_throw = false;

    EXPECT_EQ(original.Size(), 2);
    EXPECT_EQ(original.Front().data, "A");
}

TEST(MyListStressTests, NoRecursionOnClearAndDestroy) {
    const size_t HUGE_SIZE = 1'000'000;
    
    {
        MyList<int> stressList;
        for (size_t i = 0; i < HUGE_SIZE; ++i) {
            stressList.PushBack(static_cast<int>(i));
        }
        
        EXPECT_EQ(stressList.Size(), HUGE_SIZE);
        EXPECT_NO_THROW(stressList.Clear());
        EXPECT_TRUE(stressList.Empty());
    }

    {
        MyList<std::string> stressStringList;
        for (size_t i = 0; i < HUGE_SIZE; ++i) {
            stressStringList.PushBack("test");
        }
    }
}