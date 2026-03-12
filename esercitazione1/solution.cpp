#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, const char *argv[])
{
string filename;
filename=argv[1];
ifstream file(filename);
if (file.is_open()) {
    string location;
    double temp1, temp2, temp3, temp4;
    while (file >> location >> temp1 >> temp2 >> temp3 >> temp4) {
        double tempmedia=(temp1+temp2+temp3+temp4)/4;
        cout << location << " " << tempmedia << "\n";
    }
}
else {cerr << "error opening the file\n";
return 1;}
return 0;
}