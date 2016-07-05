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
    vector<hsize_t> max_dims = {H5S_UNLIMITED, H5S_UNLIMITED};
    vector<hsize_t> chunk_dims = {n,2*n};
    vector<hsize_t> a_dims = {1};
    int dx = 2;
    double dt = 3.2;
    int new_value = -1;

    dataspace ds(dims, max_dims, chunk_dims);
    //dataspace ds(dims);
    h5file f("test.h5", H5F_ACC_TRUNC);

    {
    auto d1 = f.create_dataset("data", H5T_NATIVE_INT, ds);
    }
    auto d1 = f.open_dataset("data");
    d1->extend({n, 2*n});
    d1->select({0,3},{3,3});
    d1->write(A.data());

    //d1->append(A.data());
}

