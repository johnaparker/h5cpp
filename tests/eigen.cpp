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

    write_matrix<double> (a, f, "a1");
    auto dset = write_matrix<double> (a, f, "a2", append::True);
    dset.append(a.data());


    Eigen::Vector3d b;
    b << 1,2,3;

    write_vector<double>(b, g, "b");

    Eigen::Tensor<double,3, Eigen::RowMajor> t(2,3,4);
    t.setConstant(1.5);
    t(1,1,2) = 100;

    dset = write_tensor(t, f, "tensor", append::True);
    dset.append(t.data());


}
