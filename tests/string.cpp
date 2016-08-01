#include <iostream>
#include "hdf5.h"
#include <memory>
#include "h5cpp.h"
#include <vector>
#include <eigen3/Eigen/Core>
#include <string>

using namespace std;
using namespace h5cpp;

int main() {
    //create a 3x3 matrix of ints
    vector<hsize_t> dims = {3,3};
    Eigen::Matrix<int,3,3,Eigen::RowMajor> A;
    A << 1,2,3,
         4,5,6,
         7,8,9;

    dataspace ds(dims);

    h5file f("test.h5", io::w);

    // write matrix
    auto d1 = f.create_dataset("data", dtype::Int, ds);
    d1->write(A.data());

    // string message
    string message("hello there, how are you?");
    dataspace ds_a(vector<hsize_t>{1});

    // write message
    auto a1 = f.create_attribute("message", dtype::String,  ds_a);
    a1->write(&message);

    // write seres of messages
    const int num = 4;
    const char* series[num] = {"hi","bye","coffee","tea"};
    dataspace ds_a2(vector<hsize_t>{num});
    auto a2 = f.create_attribute("series", dtype::String,  ds_a2);
    a2->write(series);
}

