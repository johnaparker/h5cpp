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
    
    //complex array
    hsize_t n = 100;
    vector<complex<double>> values;
    values.resize(n);
    for (hsize_t i = 0; i != n; i++) {
        double phase = double(i)/n*M_PI;
        double amp = i;
        values[i] = amp*complex<double>(cos(phase), sin(phase));
    }


    //dimensions
    vector<hsize_t> dims = {n,2};
    h5file f("test.h5", io::w);

    //write
    auto dataset = f.create_dataset("data", dtype::Double, dataspace(dims));
    dataset.write(values.data());

}

