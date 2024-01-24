#include <iostream>
using namespace std;
int main()
{
    cout << "Enter two numbers:" << endl;
    int fir = 0, sec = 0, sum = 0;
    cin >> fir >> sec;
    int fir1 = fir;
    if (fir < sec)
    {
        while (fir <= sec)
        {
            sum += fir;
            ++fir;
        }
        cout << "Sum of " << fir1 << " to " << sec << " is " << sum << endl;
    }
    else
    {
        while (fir >= sec)
        {
            sum += fir;
            --fir;
        }
        cout << "Sum of " << sec << " to " << fir1 << " is " << sum << endl;
    }
    return 0;
}