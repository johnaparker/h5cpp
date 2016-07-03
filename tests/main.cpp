#include <iostream>
#include "boost/multi_array.hpp"
#include "hdf5.h"
#include <memory>
#include "h5file.h"
#include <vector>

using namespace std;

int main() {
    //boost array properties
    const int drank = 2;
    using array_type = boost::multi_array<int, drank>;
    //using index = array_type::index;
    size_t n = 3;

    //initialize the array
    array_type A(boost::extents[n][n]);
    for (size_t i = 0; i != n; i++) {
        for (size_t j = 0; j != n; j++) {
            A[i][j] = i + j;
        }
    }

    vector<hsize_t> dims = {n,n};

    h5file f("test.h5");

    auto g1 = f.create_group("/sub");
    auto g2 = f.create_group("/sub/other");

    auto d1 = f.create_dataset("data", H5T_NATIVE_INT, dims);
    //f.create_dataset("data", "/sub", H5T_NATIVE_INT, dims);
    //f.create_dataset("data", "/sub/other", H5T_NATIVE_INT, dims);

    //f.write_data("data", "/", A.data(), H5T_NATIVE_INT);
    //f.write_data("data", "/sub", A.data(), H5T_NATIVE_INT);
    //f.write_data("data", "/sub/other", A.data(), H5T_NATIVE_INT);

    auto a1 = f.create_attribute("dx", H5T_NATIVE_INT,  dims);
    //f.create_attribute("dx", "/sub", H5T_NATIVE_INT, dims);
    //f.create_attribute("dt", "/sub/other", H5T_NATIVE_INT, dims);

}
