#include <fstream>
#include <iostream>
using namespace std;

int main(int argc, const char *argv[]) // argc is the number of command line arguments, 
//argv is an array of strings containing the command line arguments
{if (argc < 2) // check if the name of the file is insered
    {
        cerr << "Non hai inserito il nome del file da leggere\n"; 
        return 1;
    }
string filename;
filename=argv[1]; // the first command line argument is the filename
ifstream file(filename); // it opens the file for reading
if (file.is_open()) {
    string location;
    double temp1, temp2, temp3, temp4;
    while (file >> location >> temp1 >> temp2 >> temp3 >> temp4) { // it keeps going until it reads something,
        // in order to avoid an infinite cycle if there are infinite null rows
        double tempmedia=(temp1+temp2+temp3+temp4)/4;
        cout << location << " " << tempmedia << "\n";
    }
}
else {cerr << "error opening the file\n";
return 1;}
return 0;
}