# include <iostream>
#include <array>
using namespace std;


int main()
{
double ad[4] = {0.0, 1.1, 2.2, 3.3}; 
//cout << &ad[0] << "\n"; // it prints the address of the first element of the array ad,
// which is the same as the address of the array itself.

float af[8] = {0.0, 1.1, 2.2, 3.3, 4.4, 5.5, 6.6, 7.7};
//cout << &af[0] << "\n";

int ai[3] = {0, 1, 2};
//cout << &ai[0] << "\n";

int x = 1;
cout << "indirizzo di x: " << &x << "\n";
float y = 1.1;
cout << "indirizzo di y: " << &y << "\n";

(&y)[1] = 0; // it changes the value of x to 0,
// because I'm saying to go to 4 bytes ahead of y,
// and this could affect the value of x.
cout << x << "\n";

return 0;
}