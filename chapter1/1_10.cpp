#include<iostream>
using namespace std;
int main()
{
    int sum=0,val=10;
    while(val>=0)
    {
          sum+=val;
          --val;
    }
    cout<<"Sum of 10 to 0 is "<<sum<<endl;
    return 0;
}