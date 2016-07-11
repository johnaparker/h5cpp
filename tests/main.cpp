#include <iostream>
#include "boost/multi_array.hpp"
#include "hdf5.h"
#include <memory>
#include "h5cpp.h"
#include <vector>

using namespace std;
using namespace h5cpp;

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
            A[i][j] = i+j;
        }
    }

    vector<hsize_t> dims = {n,n};
    vector<hsize_t> max_dims = {inf, inf};
    vector<hsize_t> chunk_dims = {2,2};
    vector<hsize_t> a_dims = {1};
    //int dx = 2;
    //double dt = 3.2;
    //int new_value = -1;

    dataspace ds(dims, max_dims, chunk_dims, true);
    //dataspace ds_a(vector<hsize_t>{1});
    dataspace ds_a(vector<hsize_t>{1});
    h5file f("test.h5", io::w);

    auto d1 = f.create_dataset("data", dtype::Int, ds);
    //auto d1 = f.open_dataset("data");
    d1->extend({n, 2*n});
    d1->select_write(A.data(), {0,3}, {3,3});
    {
    auto a1 = d1->create_attribute("dx", dtype::Int,  ds_a);
    }

    auto a1 = d1->open_attribute("dx");
    a1->write(A.data());
    //d1->select({0,3},{3,3});
    //d1->write(A.data());
    //d1->append(A.data());
    auto g1 = f.create_group("/sub");
    auto d2 = f.create_dataset("/sub/data2", dtype::Int, ds_a);
    auto a2 = d2->create_attribute("dx", dtype::Int, ds_a);
    //d1->create_attribute("dx", dtype::Int, ds_a);
    a2->write(A.data());
}

