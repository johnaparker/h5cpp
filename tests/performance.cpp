#include <iostream>
#include "hdf5.h"
#include <memory>
#include "h5cpp.h"
#include <vector>
#include <complex>
#include "math.h"

using namespace std;
using namespace h5cpp;

int main() {
    //boost array properties
    const int iterations = 1e5;
    
    //complex array
    hsize_t n = 100;
    vector<complex<double>> values;
    values.resize(n);
    for (hsize_t i = 0; i != n; i++) {
        double phase = double(i)/n*M_PI;
        double amp = i;
        values[i] = amp*complex<double>(cos(phase), sin(phase));
    }


    h5file f("test.h5", io::w);
    {
        auto group = f.create_group("new");
        vector<hsize_t> dims = {n,2,1};
        vector<hsize_t> max_dims = {n,2, inf};
        vector<hsize_t> chunk_dims = {n,2, 1};
        //write
        auto dataset = group.create_dataset("data", dtype::Double, dataspace(dims, max_dims, chunk_dims));
        dataset.write(values.data());
    }

    for (int i = 0; i != iterations; i++) {
        auto group = f.open_group("new");
        auto dataset = group.open_dataset("data");
        dataset.append(values.data());
    }

}

