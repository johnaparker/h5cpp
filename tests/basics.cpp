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
            A[i][j] = i + j;
        }
    }

    vector<hsize_t> dims = {n,n};
    vector<hsize_t> a_dims = {1};
    int dx = 2;
    double dt = 3.2;
    int new_value = -1;



    h5file f("test.h5", io::w);
    {
        auto g1 = f.create_group("/sub");
        auto g2 = f.create_group("/sub/other");
    }

    auto g1 = f.open_group("/sub");
    auto g2 = f.open_group("/sub/other");

    {
        auto d1 = f.create_dataset("data", dtype::Int, dataspace(dims));
        auto d2 = g1->create_dataset("data", dtype::Int, dataspace(dims));
    }
    auto d3 = g2->create_dataset("data", dtype::Int, dataspace(dims));
    
    auto d1 = f.open_dataset("data");
    auto d2 = g1->open_dataset("data");
    d1->write(A.data());
    d2->write(A.data());
    d3->write(A.data());

    {
    auto a1 = f.create_attribute("dx", dtype::Int,  dataspace(a_dims));
    auto a2 = g1->create_attribute("dx", dtype::Int, dataspace(a_dims));
    }
    auto a3 = g2->create_attribute("dt", dtype::Double, dataspace(a_dims));
    auto a4 = d2->create_attribute("new", dtype::Int, dataspace(a_dims));

    auto a1 = f.open_attribute("dx");
    auto a2 = f.open_attribute_by_name("dx", "/sub");

    a1->write(&dx);
    a2->write(&dx);
    a3->write(&dt);
    a4->write(&new_value);
}

