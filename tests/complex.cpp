#include <iostream>
#include "hdf5.h"
#include <memory>
#include "h5cpp.h"
#include <vector>
#include <complex>
#include "math.h"

using namespace std;
using namespace h5cpp;

struct my_complex {
    my_complex(): real(0), imag(0) {}
    my_complex(double r, double i): real(r), imag(i) {}
    double real;
    double imag;
};

int main() {
    //boost array properties
    
    //complex array
    hsize_t n = 10;
    vector<complex<double>> values;
    vector<my_complex> my_values;
    values.resize(n);
    my_values.resize(n);
    for (hsize_t i = 0; i != n; i++) {
        double phase = double(i)/n*M_PI;
        double amp = i;
        values[i] = amp*complex<double>(cos(phase), sin(phase));
        my_values[i] = my_complex(amp*cos(phase), amp*sin(phase));
    }


    //dimensions
    vector<hsize_t> dims = {n,2};
    h5file f("test.h5", io::w);

    //write
    auto dataset = f.create_dataset("data", dtype::Double, dspace(dims));
    dataset.write(values.data());


    hid_t memtype;
    herr_t status;

    memtype = H5Tcreate (H5T_COMPOUND, sizeof (my_complex));
    status = H5Tinsert (memtype, "real",
                HOFFSET (my_complex, real), H5T_NATIVE_DOUBLE);
    status = H5Tinsert (memtype, "imag", HOFFSET (my_complex, imag),
                H5T_NATIVE_DOUBLE);
    dims = {n};
    auto my_dataset = f.create_dataset("my_data", memtype, dspace(dims));
    my_dataset.write(my_values.data());
}


