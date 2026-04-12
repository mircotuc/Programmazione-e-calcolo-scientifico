#include <iostream>
#include <vector>
#include "randfiller.h"

template<typename T>
void
print_vector(const std::vector<T>& v)
{
    for (size_t i = 0; i < v.size(); i++) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";
}

int main(void)
{
    randfiller rf; /* instantiate a randfiller */

    std::vector<int> vi;
    vi.resize(10);
    rf.fill(vi, 30, 50); /* use it on vectors */
    print_vector(vi);
    
    std::vector<float> vf;
    vf.resize(5);
    rf.fill(vf, 1.5f, 7.5f);
    print_vector(vf);

    std::vector<double> vd;
    vd.resize(8);
    rf.fill(vd, 1.5, 7.5);
    print_vector(vd);

    return 0;
}
