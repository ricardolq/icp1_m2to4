#include "gtest/gtest.h"

extern "C" {
    #include "../src/assignment.c"
    #include "metadata/data.c"
}

// Test fixture for setting up and tearing down test environment
class LibraryTest : public ::testing::Test {
protected:
    Book *library;
    int count;
    int capacity;

    void SetUp() override {
        capacity = 5;
        count = 0;
        library = (Book *)malloc(capacity * sizeof(Book));
    }

    void TearDown() override {
        free(library);
    }
};

// Test for loadLibrary function
TEST_F(LibraryTest, LoadLibrary) {
    createEmptyLibrary("test_library.txt", &library, &count, &capacity);
    addBook(&library, &count, &capacity, (Book){"Title1", "Author1", 2001});
    addBook(&library, &count, &capacity, (Book){"Title2", "Author2", 2002});
    saveLibrary("test_library.txt", library, count);
    EXPECT_EQ(loadLibrary("test_library.txt", &library, &count, &capacity), 0);
    EXPECT_EQ(count, 2); // Assuming test_library.txt has 2 books
}

// Test for createEmptyLibrary function
TEST_F(LibraryTest, CreateEmptyLibrary) {
    EXPECT_EQ(createEmptyLibrary("test_empty_library.txt", &library, &count, &capacity), 0);
    EXPECT_EQ(count, 0);
    EXPECT_EQ(capacity, 5);
    FILE *file = fopen("test_empty_library.txt", "rb");
    ASSERT_NE(file, nullptr);
    int numBooks;
    fread(&numBooks, sizeof(int), 1, file);
    EXPECT_EQ(numBooks, 0);
    fclose(file);
}

// Test for saveLibrary function
TEST_F(LibraryTest, SaveLibrary) {
    Book book1 = {"Title1", "Author1", 2001};
    Book book2 = {"Title2", "Author2", 2002};
    addBook(&library, &count, &capacity, book1);
    addBook(&library, &count, &capacity, book2);
    EXPECT_EQ(saveLibrary("test_library_save.txt", library, count), 0);
    EXPECT_EQ(loadLibrary("test_library_save.txt", &library, &count, &capacity), 0);
    EXPECT_EQ(count, 2);
}

// Test for searchBook function
TEST_F(LibraryTest, SearchBook) {
    Book book = {"Title1", "Author1", 2001};
    addBook(&library, &count, &capacity, book);
    Book *foundBook = searchBook(library, count, "Title1");
    ASSERT_NE(foundBook, nullptr);
    EXPECT_STREQ(foundBook->title, "Title1");
}

// Test for addBook function
TEST_F(LibraryTest, AddBook) {
    Book book = {"Title1", "Author1", 2001};
    addBook(&library, &count, &capacity, book);
    EXPECT_EQ(count, 1);
    EXPECT_STREQ(library[0].title, "Title1");
}

// Test for removeBook function
TEST_F(LibraryTest, RemoveBook) {
    Book book = {"Title1", "Author1", 2001};
    addBook(&library, &count, &capacity, book);
    char title[] = "Title1";
    EXPECT_EQ(removeBook(library, &count, title), 0); // Expect success
    EXPECT_EQ(count, 0);
}

TEST_F(LibraryTest, RemoveBookNotFound) {
    Book book = {"Title1", "Author1", 2001};
    addBook(&library, &count, &capacity, book);
    char title[] = "NonExistentTitle";
    EXPECT_EQ(removeBook(library, &count, title), -1); // Expect failure
    EXPECT_EQ(count, 1);
}

// Test for removeBook function with multiple books
TEST_F(LibraryTest, RemoveBookMultiple) {
    Book book1 = {"Title1", "Author1", 2001};
    Book book2 = {"Title2", "Author2", 2002};
    Book book3 = {"Title3", "Author3", 2003};
    addBook(&library, &count, &capacity, book1);
    addBook(&library, &count, &capacity, book2);
    addBook(&library, &count, &capacity, book3);

    char title[] = "Title2";
    EXPECT_EQ(removeBook(library, &count, title), 0); // Expect success
    EXPECT_EQ(count, 2);
    EXPECT_EQ(searchBook(library, count, "Title2"), nullptr); // Book should be removed
    EXPECT_STREQ(library[0].title, "Title1");
    EXPECT_STREQ(library[1].title, "Title3");
}

TEST_F(LibraryTest, RemoveBookMultipleNotFound) {
    Book book1 = {"Title1", "Author1", 2001};
    Book book2 = {"Title2", "Author2", 2002};
    Book book3 = {"Title3", "Author3", 2003};
    addBook(&library, &count, &capacity, book1);
    addBook(&library, &count, &capacity, book2);
    addBook(&library, &count, &capacity, book3);

    char title[] = "NonExistentTitle";
    EXPECT_EQ(removeBook(library, &count, title), -1); // Expect failure
    EXPECT_EQ(count, 3);
}

// Test for listBooks function
TEST_F(LibraryTest, ListBooks) {
    Book book1 = {"Title1", "Author1", 2001};
    Book book2 = {"Title2", "Author2", 2002};
    addBook(&library, &count, &capacity, book1);
    addBook(&library, &count, &capacity, book2);
    testing::internal::CaptureStdout();
    listBooks(library, count);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("Title1"), std::string::npos);
    EXPECT_NE(output.find("Title2"), std::string::npos);
}

// Test for listBooks function in an empty library
TEST_F(LibraryTest, ListBooksEmptyLibrary) {
    testing::internal::CaptureStdout();
    listBooks(library, count);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, ""); // Expect no output for an empty library
}

// Test for removeBook function in an empty library
TEST_F(LibraryTest, RemoveBookEmptyLibrary) {
    char title[] = "NonExistentTitle";
    EXPECT_EQ(removeBook(library, &count, title), -1); // Expect failure
    EXPECT_EQ(count, 0);
}

// Test for searchBook function in an empty library
TEST_F(LibraryTest, SearchBookEmptyLibrary) {
    Book *foundBook = searchBook(library, count, "NonExistentTitle");
    EXPECT_EQ(foundBook, nullptr); // Expect not to find any book
}

// Test for adding more books than initial capacity
TEST_F(LibraryTest, AddBookResize) {
    for (int i = 0; i < 10; ++i) {
        Book book;
        snprintf(book.title, sizeof(book.title), "Title%d", i);
        snprintf(book.author, sizeof(book.author), "Author%d", i);
        book.year = 2000 + i;
        addBook(&library, &count, &capacity, book);
    }
    EXPECT_EQ(count, 10);
    EXPECT_GT(capacity, 5); // Capacity should have increased from the initial 5
}

// Test for loading library with more books than initial capacity
TEST_F(LibraryTest, LoadLibraryResize) {
    createEmptyLibrary("test_library_resize.txt", &library, &count, &capacity);
    for (int i = 0; i < 10; ++i) {
        Book book;
        snprintf(book.title, sizeof(book.title), "Title%d", i);
        snprintf(book.author, sizeof(book.author), "Author%d", i);
        book.year = 2000 + i;
        addBook(&library, &count, &capacity, book);
    }
    saveLibrary("test_library_resize.txt", library, count);

    // Reset library to initial state
    free(library);
    capacity = 5;
    count = 0;
    library = (Book *)malloc(capacity * sizeof(Book));

    EXPECT_EQ(loadLibrary("test_library_resize.txt", &library, &count, &capacity), 0);
    EXPECT_EQ(count, 10);
    EXPECT_GT(capacity, 5); // Capacity should have increased from the initial 5
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}