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

struct crazy {
    crazy(): one(0), two(0) {}
    crazy(double r, string s, int i): one(r), three(s), two(i) {
        data[0] = r;
        data[1] = 2*r;
    }
    double one;
    double data[2];
    string three;
    int two;
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


    // create complex datatype
    //compound_dtype complexType (sizeof(my_complex));
    //complexType.insert("real", dtype::Double);
    //complexType.insert("imag", dtype::Double);

    //dtypeCompound complexType (sizeof(complex<double>));
    //complexType.insert("real", dtype::Double);
    //complexType.insert("imag", dtype::Double);

    //dims = {n};
    //auto my_dataset = f.create_dataset("my_data", complexType.memType(), dspace(dims));
    //my_dataset.write(my_values.data());



    vector<hsize_t> dims_ = {2};
    hid_t array_type = dtypeArray(dtype::Double, dims_);
    dtypeCompound crazyType (sizeof(crazy));
    crazyType.insert("one" , dtype::Double);
    //crazyType.insert("data" , dtypeArray(dtype::Double, {2}));
    crazyType.insert("data" , array_type);
    crazyType.insert("two" , dtype::String);
    crazyType.insert("three" , dtype::Int);

    crazy out[2];
    out[0] = crazy(1.3, "hi",1);
    out[1] = crazy(1.3, "bye",1);

    auto crazy_dataset = f.create_dataset("crazy", crazyType.fileType() , dspace(dims));
    crazy_dataset.write(&(out[0]), crazyType.memType());

}


