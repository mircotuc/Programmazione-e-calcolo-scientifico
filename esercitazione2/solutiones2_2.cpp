#include <iostream>
#include <array>
#include <cmath>
using namespace std;

int main()
{ 
static const int N = 10; // in order to avoid variable length array
double arr[N]= {0.1, 0.3, 0.4, 0.4, 0.6, 0.8, 0.7, 0.1, 0.9, 0.1};
double minimo = arr[0];
double massimo = arr[0];
for (int i=0; i<N; i++) 
{
    minimo=min(minimo, arr[i]); // at every iteration I compare the minimum with the current value and I update it
    massimo=max(massimo, arr[i]);
}
cout << "Il minimo è: " << minimo << "\n";
cout << "Il massimo è: " << massimo << "\n";
double somma= 0.0;
for (int i=0; i<N; i++)
{
    somma += arr[i]; // at every iteration I add the current value to the sum
}
double media=somma/N;
cout << "La media è: " << media << "\n";
double somma_scarti_quadratici= 0.0;
for (int i=0; i<N; i++)
{
    somma_scarti_quadratici += pow(arr[i]-media, 2); // at every iteration I add the square of the difference 
    // between the current value and the mean 
}
double varianza= somma_scarti_quadratici/N;
double deviazione_standard= sqrt(varianza);
cout << "La deviazione standard è: " << deviazione_standard << "\n";

return 0;
}