#include <iostream>
#include "boost/multi_array.hpp"
#include "hdf5.h"
#include <memory>
#include "h5file.h"

using namespace std;

int main() {
    //boost array properties
    const int drank = 2;
    using array_type = boost::multi_array<int, drank>;
    //using index = array_type::index;
    int n = 4;

    //initialize the array
    array_type A(boost::extents[n][n]);
    for (int i = 0; i != n; i++) {
        for (int j = 0; j != n; j++) {
            A[i][j] = i + j;
        }
    }

    hsize_t* dims = new hsize_t[2];
    dims[0] = 3;
    dims[1] = 3;
    h5file f("test.h5");
    f.create_group("/sub");
    f.create_dataset("data", "/", H5T_NATIVE_INT, 2, dims);
    f.create_dataset("data", "/sub", H5T_NATIVE_INT, 2, dims);

    delete[] dims;
}
