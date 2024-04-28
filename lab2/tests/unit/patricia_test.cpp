#include "patricia.h"

#include <gtest/gtest.h>

TEST(patricia_test, modifier01) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    EXPECT_EQ(p.Find("a").value, 1);
    EXPECT_EQ(p.Size(), 1);
}

TEST(patricia_test, modifier02) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    EXPECT_ANY_THROW(p.Insert({"A", 2}));
}

TEST(patricia_test, modifier03) {
    TPatriciaTrie p;
    p.Insert({"abc", 10});
    EXPECT_EQ(p.Size(), 1);
    EXPECT_ANY_THROW(p.Insert({"abc", 20}));
}

TEST(patricia_test, modifier04) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    EXPECT_EQ(p.Size(), 2);
    EXPECT_EQ(p.Find("ab").value, 10);
    EXPECT_EQ(p.Find("abc").value, 20);
}

TEST(patricia_test, modifier05) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    EXPECT_EQ(p.Find("Ab").value, 10);
    EXPECT_EQ(p.Find("ABC").value, 20);
    EXPECT_ANY_THROW(p.Find("abcd"));
}

TEST(patricia_test, modifier06) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    p.Erase("ab");
    EXPECT_EQ(p.Size(), 1);
    EXPECT_ANY_THROW(p.Find("ab"));
    EXPECT_EQ(p.Find("abc").value, 20);
}

TEST(patricia_test, modifier07) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    p.Erase("ABc");
    EXPECT_EQ(p.Find("ab").value, 10);
    EXPECT_ANY_THROW(p.Find("aBc"));
}

TEST(patricia_test, modifier08) {
    TPatriciaTrie p;
    EXPECT_EQ(p.Size(), 0);
    EXPECT_ANY_THROW(p.Erase("ab"));
}

TEST(patricia_test, modifier09) {
    TPatriciaTrie p;
    EXPECT_ANY_THROW(p.Find("ab"));
}

TEST(patricia_test, modifier10) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    p.Insert({"a", 30});
    p.Insert({"b", 40});
    p.Insert({"bac", 50});
    p.Erase("a");
    EXPECT_EQ(p.Size(), 4);
    EXPECT_ANY_THROW(p.Find("a"));
    EXPECT_EQ(p.Find("ab").value, 10);
    EXPECT_EQ(p.Find("abc").value, 20);
}

TEST(patricia_test, modifier11) {
    TPatriciaTrie p;
    p.Insert({"ab", 10});
    p.Insert({"abc", 20});
    p.Insert({"a", 30});
    p.Erase("a");
    p.Erase("ab");
    p.Erase("abc");
    p.Insert({"a", 30});
    EXPECT_EQ(p.Size(), 1);
    EXPECT_EQ(p.Find("a").value, 30);
}

TEST(patricia_test, modifier12) {
    TPatriciaTrie p;
    std::string s = "";
    for (int i = 0; i < 20; ++i) {
        p.Insert({(s + static_cast<char>('a' + i)).c_str(),
                  static_cast<uint64_t>(i)});
    }
    EXPECT_EQ(p.Size(), 20);
    for (int i = 0; i < 20; ++i) {
        EXPECT_EQ(p.Find((s + static_cast<char>('a' + i)).c_str()).value,
                  static_cast<uint64_t>(i));
    }
}

TEST(patricia_test, modifier13) {
    TPatriciaTrie p;
    std::string s = "";
    for (int i = 0; i < 10; ++i) {
        p.Insert({(s + static_cast<char>('a' + i)).c_str(),
                  static_cast<uint64_t>(i)});
        p.Insert({(s + static_cast<char>('z' - i)).c_str(),
                  static_cast<uint64_t>(i)});
    }
    EXPECT_EQ(p.Size(), 20);
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(p.Find((s + static_cast<char>('a' + i)).c_str()).value, i);
        EXPECT_EQ(p.Find((s + static_cast<char>('z' - i)).c_str()).value, i);
    }
}

TEST(patricia_test, modifier14) {
    TPatriciaTrie p;
    std::string s = "";
    for (int i = 0; i < 10; ++i) {
        p.Insert({(s + static_cast<char>('a' + i)).c_str(),
                  static_cast<uint64_t>(i)});
        p.Insert({(s + static_cast<char>('z' - i)).c_str(),
                  static_cast<uint64_t>(i)});
    }
    for (int i = 0; i < 10; ++i) {
        p.Erase((s + static_cast<char>('a' + i)).c_str());
        EXPECT_EQ(p.Find((s + static_cast<char>('z' - i)).c_str()).value, i);
        p.Erase((s + static_cast<char>('z' - i)).c_str());
    }
    EXPECT_EQ(p.Size(), 0);
    EXPECT_ANY_THROW(p.Find("a"));
}

TEST(patricia_test, modifier15) {
    TPatriciaTrie p;
    std::string s = "";
    for (int i = 0; i < 250; ++i) {
        s += 'a';
        p.Insert({s.c_str(), static_cast<uint64_t>(i)});
    }

    EXPECT_EQ(p.Size(), 250);

    for (int i = 249; i >= 0; --i) {
        EXPECT_EQ(p.Find(s.c_str()).value, i);
        s.pop_back();
    }

    for (int i = 0; i < 250; ++i) {
        s += 'a';
        p.Erase(s.c_str());
    }
    EXPECT_EQ(p.Size(), 0);
}

TEST(patricia_test, modifier16) {
    TPatriciaTrie p;
    std::string s = "";
    for (int i = 0; i < 250; ++i) {
        s += 'a';
        p.Insert({s.c_str(), static_cast<uint64_t>(i)});
    }

    p.Erase("a");
    EXPECT_EQ(p.Size(), 249);

    for (int i = 249; i >= 1; --i) {
        EXPECT_EQ(p.Find(s.c_str()).value, i);
        s.pop_back();
    }
}

TEST(patricia_test, modifier17) {
    TPatriciaTrie p;
    std::string s = "";
    for (int i = 0; i < 250; ++i) {
        s += 'a';
    }

    for (int i = 0; i < 250; ++i) {
        p.Insert({s.c_str(), static_cast<uint64_t>(i)});
        s.pop_back();
    }

    s = "";

    for (int i = 0; i < 250; ++i) {
        s += 'a';
        p.Find(s.c_str());
    }
}

TEST(patricia_test, modifier18) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"d", 2});
    p.Insert({"c", 3});
    p.Insert({"f", 4});
    p.Erase("a");
    EXPECT_ANY_THROW(p.Find("a"));
    EXPECT_EQ(p.Find("c").value, 3);
    EXPECT_EQ(p.Find("d").value, 2);
    EXPECT_EQ(p.Find("f").value, 4);
    p.Erase("d");
    EXPECT_ANY_THROW(p.Find("d"));
    EXPECT_EQ(p.Find("c").value, 3);
    EXPECT_EQ(p.Find("f").value, 4);
    p.Erase("c");
    EXPECT_ANY_THROW(p.Find("c"));
    EXPECT_EQ(p.Find("f").value, 4);
    p.Erase("f");
    EXPECT_ANY_THROW(p.Find("f"));
    p.Insert({"a", 1});
    p.Insert({"d", 2});
    p.Insert({"c", 3});
    p.Insert({"f", 4});
    EXPECT_EQ(p.Find("a").value, 1);
    EXPECT_EQ(p.Find("c").value, 3);
    EXPECT_EQ(p.Find("d").value, 2);
    EXPECT_EQ(p.Find("f").value, 4);
}

TEST(patricia_test, insert01) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    EXPECT_EQ(p.Find("a").value, 1);
}

TEST(patricia_test, insert02) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"b", 2});
    EXPECT_EQ(p.Find("a").value, 1);
    EXPECT_EQ(p.Find("b").value, 2);
}

TEST(patricia_test, insert03) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"d", 2});
    p.Insert({"c", 3});
    p.Insert({"f", 4});
    EXPECT_EQ(p.Find("a").value, 1);
    EXPECT_EQ(p.Find("d").value, 2);
    EXPECT_EQ(p.Find("c").value, 3);
    EXPECT_EQ(p.Find("f").value, 4);
}

TEST(patricia_test, insert04) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Erase("a");
    p.Insert({"a", 2});
    EXPECT_EQ(p.Find("a").value, 2);
}

TEST(patricia_test, erase01) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Erase("a");
    EXPECT_ANY_THROW(p.Find("a"));
}

TEST(patricia_test, erase02) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"b", 2});
    p.Erase("b");
    EXPECT_ANY_THROW(p.Find("b"));
    EXPECT_EQ(p.Find("a").value, 1);
    p.Erase("a");
    EXPECT_ANY_THROW(p.Find("a"));
}

TEST(patricia_test, erase03) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"b", 2});
    p.Erase("a");
    EXPECT_ANY_THROW(p.Find("a"));
    EXPECT_EQ(p.Find("b").value, 2);
    p.Erase("b");
    EXPECT_ANY_THROW(p.Find("b"));
}

TEST(patricia_test, erase04) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"d", 2});
    p.Insert({"c", 3});
    p.Insert({"f", 4});
    p.Erase("c");
    EXPECT_ANY_THROW(p.Find("c"));
    EXPECT_EQ(p.Find("a").value, 1);
    EXPECT_EQ(p.Find("d").value, 2);
    EXPECT_EQ(p.Find("f").value, 4);
    p.Erase("d");
    EXPECT_ANY_THROW(p.Find("d"));
    EXPECT_EQ(p.Find("a").value, 1);
    EXPECT_EQ(p.Find("f").value, 4);
    p.Erase("a");
    EXPECT_ANY_THROW(p.Find("a"));
    EXPECT_EQ(p.Find("f").value, 4);
}

TEST(patricia_test, erase05) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"d", 2});
    p.Insert({"c", 3});
    p.Insert({"f", 4});
    p.Erase("f");
    EXPECT_ANY_THROW(p.Find("f"));
    EXPECT_EQ(p.Find("a").value, 1);
    EXPECT_EQ(p.Find("d").value, 2);
    EXPECT_EQ(p.Find("c").value, 3);
    p.Erase("d");
    EXPECT_ANY_THROW(p.Find("d"));
    EXPECT_EQ(p.Find("a").value, 1);
    EXPECT_EQ(p.Find("c").value, 3);
    p.Erase("a");
    EXPECT_ANY_THROW(p.Find("a"));
    EXPECT_EQ(p.Find("c").value, 3);
}

TEST(patricia_test, erase06) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"d", 2});
    p.Insert({"c", 3});
    p.Insert({"f", 4});
    p.Erase("d");
    EXPECT_ANY_THROW(p.Find("d"));
    EXPECT_EQ(p.Find("a").value, 1);
    EXPECT_EQ(p.Find("c").value, 3);
    EXPECT_EQ(p.Find("f").value, 4);
    p.Erase("a");
    EXPECT_ANY_THROW(p.Find("a"));
    EXPECT_EQ(p.Find("c").value, 3);
    EXPECT_EQ(p.Find("f").value, 4);
}

TEST(patricia_test, erase07) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"d", 2});
    p.Insert({"c", 3});
    p.Insert({"f", 4});
    p.Erase("a");
    EXPECT_ANY_THROW(p.Find("a"));
    EXPECT_EQ(p.Find("c").value, 3);
    EXPECT_EQ(p.Find("d").value, 2);
    EXPECT_EQ(p.Find("f").value, 4);
    p.Erase("d");
    EXPECT_ANY_THROW(p.Find("d"));
    EXPECT_EQ(p.Find("c").value, 3);
    EXPECT_EQ(p.Find("f").value, 4);
}

TEST(patricia_test, erase08) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"d", 2});
    p.Insert({"c", 3});
    p.Insert({"f", 4});
    p.Erase("a");
    p.Erase("c");
    EXPECT_EQ(p.Find("d").value, 2);
    EXPECT_EQ(p.Find("f").value, 4);
    p.Erase("d");
    EXPECT_EQ(p.Find("f").value, 4);
}

TEST(patricia_test, erase09) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"d", 2});
    p.Insert({"c", 3});
    p.Insert({"f", 4});
    p.Erase("f");
    EXPECT_EQ(p.Find("d").value, 2);
    EXPECT_EQ(p.Find("c").value, 3);
    EXPECT_EQ(p.Find("a").value, 1);
    p.Erase("c");
    EXPECT_EQ(p.Find("d").value, 2);
    EXPECT_EQ(p.Find("a").value, 1);
    p.Erase("d");
    EXPECT_EQ(p.Find("a").value, 1);
}

TEST(patricia_test, erase10) {
    TPatriciaTrie p;
    p.Insert({"a", 1});
    p.Insert({"d", 2});
    p.Insert({"c", 3});
    p.Insert({"f", 4});
    p.Erase("f");
    EXPECT_EQ(p.Find("d").value, 2);
    EXPECT_EQ(p.Find("c").value, 3);
    EXPECT_EQ(p.Find("a").value, 1);
    p.Erase("d");
    EXPECT_EQ(p.Find("a").value, 1);
    EXPECT_EQ(p.Find("c").value, 3);
    p.Erase("c");
    EXPECT_EQ(p.Find("a").value, 1);
}

TEST(patricia_test, file01) {
    TPatriciaTrie p;
    {
        CFile out("patricia_test.txt", CFile::FileType::Save);
        p.Insert({"a", 1});
        p.SaveToFile(out.GetFile());
    }
    CFile in("patricia_test.txt", CFile::FileType::Load);
    p.LoadFromFile(in.GetFile());
    EXPECT_EQ(p.Find("a").value, 1);
    // remove("patricia_test.txt");
}
TEST(patricia_test, file02) {
    TPatriciaTrie p;
    {
        CFile out("patricia_test.txt", CFile::FileType::Save);
        p.Insert({"a", 1});
        p.SaveToFile(out.GetFile());
        p.Insert({"b", 2});
    }
    CFile in("patricia_test.txt", CFile::FileType::Load);
    p.LoadFromFile(in.GetFile());
    EXPECT_ANY_THROW(p.Find("b"));
    remove("patricia_test.txt");
}

TEST(patricia_test, file03) {
    TPatriciaTrie p;
    {
        CFile out("patricia_test.txt", CFile::FileType::Save);
        p.Insert({"a", 1});
        p.Insert({"d", 2});
        p.Insert({"c", 3});
        p.Insert({"f", 4});
        p.SaveToFile(out.GetFile());
        p.Erase("f");
    }
    CFile in("patricia_test.txt", CFile::FileType::Load);
    p.LoadFromFile(in.GetFile());
    EXPECT_EQ(p.Find("f").value, 4);
    remove("patricia_test.txt");
}

TEST(patricia_test, file04) {
    TPatriciaTrie p;
    std::string s = "";
    {
        CFile out("patricia_test.txt", CFile::FileType::Save);
        for (int i = 0; i < 250; ++i) {
            s += 'a';
            p.Insert({s.c_str(), static_cast<uint64_t>(i)});
        }
        p.SaveToFile(out.GetFile());
    }
    CFile in("patricia_test.txt", CFile::FileType::Load);
    p.LoadFromFile(in.GetFile());
    s = "";
    for (int i = 0; i < 250; ++i) {
        s += 'a';
        EXPECT_EQ(p.Find(s.c_str()).value, static_cast<uint64_t>(i));
    }
    remove("patricia_test.txt");
}
