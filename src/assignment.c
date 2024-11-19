#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Define the book struct

typedef struct {
    char title[50];
    char author[50];
    int year;
} Book;

// Function prototypes

/**
 * @brief Creates an empty library and initializes it.
 *
 * This function creates an empty library by writing an initial count of 0 books
 * to the specified file. It also initializes the library array with a given capacity.
 *
 * @param filename The name of the file to create and write the initial library data to.
 * @param library A pointer to the array of Book structures that will be initialized.
 * @param count A pointer to an integer that will be set to the initial count of books (0).
 * @param capacity A pointer to an integer that will be set to the initial capacity of the library (5).
 * @return 0 on success, -1 on failure (e.g., file cannot be opened, memory allocation error).
 */
int createEmptyLibrary(const char *filename, Book **library, int *count, int *capacity) {
    FILE *file = fopen(filename, "wb");
    if (!file) return -1;
    int numBooks = 0;
    fwrite(&numBooks, sizeof(int), 1, file);
    fclose(file);
    *count = 0;
    *capacity = 5;
    if (*library != NULL) free(*library);
    *library = (Book*)malloc(*capacity * sizeof(Book));
    if (!*library) {
        perror("Error allocating memory");
        return -1;
    }
    return 0;
}


/**
 * @brief Loads the library from the specified file.
 * 
 * This function reads the contents of a file and loads the data into the provided library array.
 * It updates the count of books in the library and ensures the capacity is sufficient.
 * 
 * @param filename The name of the file to load the library from.
 * @param library The array of books to load into. This should be a pointer to an array of Book pointers.
 * @param count Pointer to the number of books in the library. This will be updated with the new count.
 * @param capacity Pointer to the capacity of the library array. This will be updated if the array is resized.
 * @return int 0 if successful, -1 if the file doesn't exist or an error occurs.
 */
int loadLibrary(const char *filename, Book **library, int *count, int *capacity) {
    return -1;
}


/**
 * @brief Saves the current state of the library to the specified file.
 * 
 * @param filename The name of the file to save the library to.
 * @param library The array of books to save.
 * @param count The number of books in the library.
 * @return int 0 if successful, -1 if the file doesn't exist.
 */
int saveLibrary(const char *filename, Book library[], int count) {
    return -1;
}

/**
 * @brief Searches for a book by title in the library.
 * 
 * @param library The array of books to search in.
 * @param count The number of books in the library.
 * @param title The title of the book to search for.
 * @return Book* Pointer to the found book, or NULL if not found.
 */
Book* searchBook(Book library[], int count, const char *title) {
    return NULL;
}

/**
 * @brief Adds a new book to the library. Resizes the library if required.
 * 
 * This function adds a new book to the library. If the library is at full capacity,
 * it will resize the library array to accommodate the new book.
 * 
 * @param library The array of books to add to.
 * @param count Pointer to the number of books in the library.
 * @param capacity Pointer to the capacity of the library array.
 * @param newBook The new book to add.
 */
void addBook(Book **library, int *count, int *capacity, Book newBook) {
    return;
}

/**
 * @brief Removes a book by title from the library.
 * 
 * @param library The array of books to remove from.
 * @param count Pointer to the number of books in the library.
 * @param title The title of the book to remove.
 * @return int 0 if the book is removed, -1 if the book is not found.
 */
int removeBook(Book library[], int *count, char *title) {
    return -1; // Book not found
}

/**
 * @brief Lists all the books in the library.
 * 
 * @param library The array of books to list.
 * @param count The number of books in the library.
 */
void listBooks(Book library[], int count) {
    return;
}

#ifdef NDEBUG
int main() {
    int count = 0;
    int capacity = 5;
    Book *library = NULL; // Change to single pointer
    char title[50];
    if (loadLibrary("library.txt", &library, &count, &capacity) == -1) { // Pass address of library
        printf("Library file not found. Creating a new library.\n");
        if (createEmptyLibrary("library.txt", &library, &count, &capacity) == -1) { // Pass address of library
            printf("Error creating new library.\n");
            return -1;
        }
    }
    Book newBook;

    int choice;
    while (1) {
        printf("\nMenu:\n");
        printf("1. Add Book\n");
        printf("2. Remove Book\n");
        printf("3. List Books\n");
        printf("4. Search Book\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Add Book
                printf("Enter book title: ");
                scanf("%s", newBook.title);
                printf("Enter book author: ");
                scanf("%s", newBook.author);
                printf("Enter book year: ");
                scanf("%d", &newBook.year);
                addBook(&library, &count, &capacity, newBook);
                break;
            case 2:
                // Remove Book
                printf("Enter book title to remove: ");
                scanf("%s", title);
                removeBook(library, &count, title); // Pass library directly
                break;
            case 3:
                // List Books
                listBooks(library, count); // Pass library directly
                break;
            case 4:
                // Search Book
                printf("Enter book title to search: ");
                scanf("%s", title);
                Book *foundBook = searchBook(library, count, title); // Pass library directly
                if (foundBook) {
                    printf("Book found: %s by %s, published in %d\n", foundBook->title, foundBook->author, foundBook->year);
                } else {
                    printf("Book not found.\n");
                }
                break;
            case 5:
                // Exit
                saveLibrary("library.txt", library, count); // Pass library directly
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
    return 0;
}
#endif
