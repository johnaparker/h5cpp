#include "h5cpp.h"
#include <iostream>

using namespace h5cpp;
using namespace std;

int main() {
    h5file f("out.h5", io::w);
    auto g = f.create_group("sub");

    Eigen::Array<double, 3, 2> a;
    a << 1,2,
         4,5,
         7,8 ;

    write_matrix<double> (a, f, "a");


    Eigen::Vector3d b;
    b << 1,2,3;

    write_vector<double, dtype::Double> (b, g, "b");

}
