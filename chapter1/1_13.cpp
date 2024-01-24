#include <iostream>
using namespace std;
int main()
{
    int sum = 0;
    for (int val = 50; val <= 100; val++)
        sum += val;
    cout << "Sum of 50 to 100 is " << sum << endl;
    return 0;
}