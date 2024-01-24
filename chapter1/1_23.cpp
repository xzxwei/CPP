#include <iostream>
#include "Sales_item.h"
using namespace std;

int main()
{
    Sales_item currBook, cinBook;
    if (cin >> currBook)
    {   
        int cnt=1;
        while (cin >> cinBook)
        {
            if (currBook.isbn() == cinBook.isbn())
            {
                cnt++;
            }
            else
            {
                cout << currBook.isbn()<<" occurs "<<cnt<<" times."<<endl;
                cnt=1;
                currBook = cinBook;
            }
        }
        cout << currBook.isbn()<<" occurs "<<cnt<<" times."<<endl;
    }
    else
        cerr<<"No data?"<<endl;
    return 0;
}