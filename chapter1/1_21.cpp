#include <iostream>
#include "Sales_item.h"
using namespace std;

int main()
{
    Sales_item item1,item2;
    cin>>item1>>item2;
    if(item1.isbn()==item2.isbn())
    {
        item1 += item2;
        cout<<item1;
    }
    else
    cerr<<"different isbn!"<<endl;
}