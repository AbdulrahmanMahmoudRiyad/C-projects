#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>

using namespace std;

// Maximum limits for books and users
const int MAX_BOOKS = 10;
const int MAX_USERS = 10;

// Book class to represent individual books
class Book {
public:
    int id;
    string name;
    int total_quantity;
    int total_borrowed;

    // Constructor to initialize a book with default values
    Book() : id(-1), name(""), total_quantity(0), total_borrowed(0) {}

    // Method to read book information from input
    void read() {
        cout << "Enter book info: id & name & total quantity: ";
        cin >> id >> name >> total_quantity;
        total_borrowed = 0;
    }

    // Method to borrow a book copy
    bool borrow() {
        if (total_quantity - total_borrowed == 0)
            return false;
        ++total_borrowed;
        return true;
    }

    // Method to return a borrowed book copy
    void return_copy() {
        assert(total_borrowed > 0);
        --total_borrowed;
    }

    // Method to check if book name has a specific prefix
    bool has_prefix(const string& prefix) const {
        if (name.size() < prefix.size())
            return false;
        return name.substr(0, prefix.size()) == prefix;
    }

    // Method to print book details
    void print() const {
        cout << "id = " << id << " name = " << name << " total_quantity = "
             << total_quantity << " total_borrowed = " << total_borrowed << "\n";
    }
};

// Function to compare books by name
bool cmp_book_by_name(const Book &a, const Book &b) {
    return a.name < b.name;
}

// Function to compare books by ID
bool cmp_book_by_id(const Book &a, const Book &b) {
    return a.id < b.id;
}

// User class to represent individual users
class User {
public:
    int id;
    string name;
    vector<int> borrowed_books_ids;

    // Constructor to initialize a user with default values
    User() : id(-1), name("") {}

    // Method to read user information from input
    void read() {
        cout << "Enter user name & national id: ";
        cin >> name >> id;
    }

    // Method to borrow a book by ID
    void borrow(int book_id) {
        borrowed_books_ids.push_back(book_id);
    }

    // Method to return a borrowed book by ID
    void return_copy(int book_id) {
        auto it = find(borrowed_books_ids.begin(), borrowed_books_ids.end(), book_id);
        if (it != borrowed_books_ids.end()) {
            borrowed_books_ids.erase(it);
        } else {
            cout << "User " << name << " never borrowed book id " << book_id << "\n";
        }
    }

    // Method to check if the user has borrowed a specific book by ID
    bool is_borrowed(int book_id) const {
        return find(borrowed_books_ids.begin(), borrowed_books_ids.end(), book_id) != borrowed_books_ids.end();
    }

    // Method to print user details and borrowed books
    void print() const {
        cout << "user " << name << " id " << id << " borrowed books ids: ";
        for (int book_id : borrowed_books_ids)
            cout << book_id << " ";
        cout << "\n";
    }
};

// LibrarySystem class to manage the library operations
class LibrarySystem {
private:
    vector<Book> books;
    vector<User> users;

    // Method to display the menu and get the user's choice
    int menu() {
        int choice = -1;
        while (choice == -1) {
            cout << "\nLibrary Menu:\n";
            cout << "1) Add book\n";
            cout << "2) Search books by prefix\n";
            cout << "3) Print who borrowed book by name\n";
            cout << "4) Print library by ID\n";
            cout << "5) Print library by name\n";
            cout << "6) Add user\n";
            cout << "7) User borrow book\n";
            cout << "8) User return book\n";
            cout << "9) Print users\n";
            cout << "10) Exit\n";
            cout << "\nEnter your menu choice [1 - 10]: ";
            cin >> choice;
            if (!(1 <= choice && choice <= 10)) {
                cout << "Invalid choice. Try again\n";
                choice = -1; // Loop keeps working
            }
        }
        return choice;
    }

public:
    // Method to run the library system
    void run() {
        while (true) {
            int choice = menu();
            if (choice == 1)
                add_book();
            else if (choice == 2)
                search_books_by_prefix();
            else if (choice == 3)
                print_who_borrowed_book_by_name();
            else if (choice == 4)
                print_library_by_id();
            else if (choice == 5)
                print_library_by_name();
            else if (choice == 6)
                add_user();
            else if (choice == 7)
                user_borrow_book();
            else if (choice == 8)
                user_return_book();
            else if (choice == 9)
                print_users();
            else
                break;
        }
    }

    // Method to add a new book to the library
    void add_book() {
        if (books.size() >= MAX_BOOKS) {
            cout << "Cannot add more books. Maximum limit reached.\n";
            return;
        }
        Book book;
        book.read();
        books.push_back(book);
    }

    // Method to search for books by a name prefix
    void search_books_by_prefix() const {
        cout << "Enter book name prefix: ";
        string prefix;
        cin >> prefix;

        int cnt = 0;
        for (const auto& book : books) {
            if (book.has_prefix(prefix)) {
                cout << book.name << "\n";
                ++cnt;
            }
        }

        if (!cnt)
            cout << "No books with such prefix\n";
    }

    // Method to add a new user to the library
    void add_user() {
        if (users.size() >= MAX_USERS) {
            cout << "Cannot add more users. Maximum limit reached.\n";
            return;
        }
        User user;
        user.read();
        users.push_back(user);
    }

    // Method to find a book index by its name
    int find_book_idx_by_name(const string& name) const {
        for (int i = 0; i < books.size(); ++i) {
            if (name == books[i].name)
                return i;
        }
        return -1;
    }

    // Method to find a user index by their name
    int find_user_idx_by_name(const string& name) const {
        for (int i = 0; i < users.size(); ++i) {
            if (name == users[i].name)
                return i;
        }
        return -1;
    }

    // Method to read user and book names and return their indices
    bool read_user_name_and_book_name(int& user_idx, int& book_idx, int trials = 3) const {
        string user_name, book_name;

        while (trials--) {
            cout << "Enter user name and book name: ";
            cin >> user_name >> book_name;

            user_idx = find_user_idx_by_name(user_name);
            if (user_idx == -1) {
                cout << "Invalid user name. Try again\n";
                continue;
            }

            book_idx = find_book_idx_by_name(book_name);
            if (book_idx == -1) {
                cout << "Invalid book name. Try again\n";
                continue;
            }
            return true;
        }
        cout << "You did several trials! Try later.\n";
        return false;
    }

    // Method for a user to borrow a book
    void user_borrow_book() {
        int user_idx, book_idx;

        if (!read_user_name_and_book_name(user_idx, book_idx))
            return;

        if (!books[book_idx].borrow()) {
            cout << "No more copies available right now\n";
        } else {
            users[user_idx].borrow(books[book_idx].id);
        }
    }

    // Method for a user to return a borrowed book
    void user_return_book() {
        int user_idx, book_idx;

        if (!read_user_name_and_book_name(user_idx, book_idx))
            return;

        books[book_idx].return_copy();
        users[user_idx].return_copy(books[book_idx].id);
    }

    // Method to print the library books sorted by ID
    void print_library_by_id() {
        sort(books.begin(), books.end(), cmp_book_by_id);

        cout << "\n";
        for (const auto& book : books)
            book.print();
    }

    // Method to print the library books sorted by name
    void print_library_by_name() {
        sort(books.begin(), books.end(), cmp_book_by_name);

        cout << "\n";
        for (const auto& book : books)
            book.print();
    }

    // Method to print all users
    void print_users() const {
        cout << "\n";
        for (const auto& user : users)
            user.print();
    }

    // Method to print who borrowed a specific book by name
    void print_who_borrowed_book_by_name() {
        string book_name;
        cout << "Enter book name: ";
        cin >> book_name;

        int book_idx = find_book_idx_by_name(book_name);
        if (book_idx == -1) {
            cout << "Invalid book name.\n";
            return;
        }

        if (books[book_idx].total_borrowed == 0) {
            cout << "No borrowed copies\n";
            return;
        }

        for (const auto& user : users) {
            if (user.is_borrowed(books[book_idx].id))
                cout << user.name << "\n";
        }
    }
};

int main() {
    LibrarySystem library;
    library.run();
    return 0;
}
