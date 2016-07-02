#include <iostream>
#include "boost/multi_array.hpp"
#include "hdf5.h"
#include <memory>
#include "h5io.h"

using namespace std;

int main() {
    //boost array properties
    const int drank = 2;
    using array_type = boost::multi_array<int, drank>;
    using index = array_type::index;
    int n = 4;

    //initialize the array
    array_type A(boost::extents[n][n]);
    for (int i = 0; i != n; i++) {
        for (int j = 0; j != n; j++) {
            A[i][j] = i + j;
        }
    }

    h5io f("test.h5");
    f.create_group("/sub");
    f.create_dataset("data", )

}
