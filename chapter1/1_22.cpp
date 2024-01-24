#include <iostream>
#include "Sales_item.h"
using namespace std;

int main()
{
    Sales_item currBook, cinBook;
    if (cin >> currBook)
    {
        while (cin >> cinBook)
        {
            if (currBook.isbn() == cinBook.isbn())
            {
                currBook += cinBook;
            }
            else
            {
                cout << currBook << endl;
                currBook = cinBook;
            }
        }
        cout << currBook << endl;
    }
    return 0;
}