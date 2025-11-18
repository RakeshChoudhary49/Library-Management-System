#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

class Book
{
protected:
    string title, author;
    int id;

public:
    Book(string t = "", string a = "", int i = 0)
        : title(t), author(a), id(i) {}

    virtual void display() const
    {
        cout << "ID: " << id
             << "\nTitle : " << title
             << "\nAuthor: " << author << endl;
    }

    int getID() const { return id; }
};

class LibraryBook : public Book
{
    bool available;

public:
    LibraryBook(string t = "", string a = "", int i = 0, bool av = true)
        : Book(t, a, i), available(av) {}

    void display() const override
    {
        Book::display();
        cout << "Status: " << (available ? "Available" : "Issued") << endl;
    }

    bool isAvailable() const { return available; }
    void issue() { available = false; }
    void ret() { available = true; }
};

class Library
{
    vector<LibraryBook> books;
    static int bookCount;

public:
    void operator+=(const LibraryBook &b)
    {
        books.push_back(b);
        bookCount++;
    }

    LibraryBook *search(int id)
    {
        for (auto &b : books)
            if (b.getID() == id)
                return &b;
        return nullptr;
    }

    void showAll()
    {
        if (books.empty())
        {
            cout << "No books in library.\n";
            return;
        }
        cout << "\n===== Library Books =====\n";
        for (auto &b : books)
        {
            b.display();
            cout << "------------------------\n";
        }
    }

    void save()
    {
        ofstream f("library.txt");
        for (auto &b : books)
            f << b.getID() << " " << b.isAvailable() << endl;
        f.close();
    }

    static int getCount() { return bookCount; }

    friend void viewCount(const Library &l);
};
int Library::bookCount = 0;

void viewCount(const Library &l)
{
    cout << "\nTotal Books in Library: " << Library::getCount() << endl;
}

void issueBook(Library &L, int id)
{
    LibraryBook *b = L.search(id);

    if (!b)
        throw runtime_error("Book not found!");

    if (!b->isAvailable())
        throw runtime_error("Book already issued!");

    b->issue();
    cout << "Book issued successfully!\n";
}

int main()
{
    Library L;
    int choice;

    while (true)
    {
        cout << "\n===== Library Management Menu =====\n";
        cout << "1. Add Book\n";
        cout << "2. Display All Books\n";
        cout << "3. Issue Book\n";
        cout << "4. Return Book\n";
        cout << "5. Save to File\n";
        cout << "6. Total Book Count\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1)
        {
            string t, a;
            int id;
            cout << "Enter Book ID: ";
            cin >> id;
            cin.ignore();
            cout << "Enter Title : ";
            getline(cin, t);
            cout << "Enter Author: ";
            getline(cin, a);

            L += LibraryBook(t, a, id);
            cout << "Book Added Successfully!\n";
        }

        else if (choice == 2)
        {
            L.showAll();
        }

        else if (choice == 3)
        {
            int id;
            cout << "Enter Book ID to Issue: ";
            cin >> id;

            try
            {
                issueBook(L, id);
            }
            catch (exception &e)
            {
                cout << "Error: " << e.what() << endl;
            }
        }

        else if (choice == 4)
        {
            int id;
            cout << "Enter Book ID to Return: ";
            cin >> id;

            LibraryBook *b = L.search(id);
            if (!b)
                cout << "Book not found!\n";
            else
            {
                b->ret();
                cout << "Book Returned Successfully!\n";
            }
        }

        else if (choice == 5)
        {
            L.save();
            cout << "Data saved to file.\n";
        }

        else if (choice == 6)
        {
            viewCount(L);
        }

        else if (choice == 7)
        {
            cout << "Exiting Program...\n";
            break;
        }

        else
        {
            cout << "Invalid choice! Try again.\n";
        }
    }

    return 0;
}