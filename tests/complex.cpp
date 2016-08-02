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
    crazy(double r, int i, string s): one(r), two(i), three(s.c_str()) {}
    double one;
    int two;
    const char* three;
};

class compound_dtype {
public:
    compound_dtype(size_t memsize) {
        memtype = H5Tcreate (H5T_COMPOUND, memsize);
        memDisplace = 0;
    }

    void insert(string name, dtype datatype) {
        hid_t h5dtype = getDtype(datatype);
        status = H5Tinsert (memtype, name.c_str(),
                    memDisplace, h5dtype);

        if (datatype == dtype::String)
            memDisplace += sizeof(hvl_t);
        else
            memDisplace += H5Tget_size(h5dtype);

    }

    hid_t getType() {
        return memtype;
    }

private:
    hid_t memtype;
    herr_t status;
    size_t memDisplace;
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

    compound_dtype complexType (sizeof(complex<double>));
    complexType.insert("real", dtype::Double);
    complexType.insert("imag", dtype::Double);

    dims = {n};
    auto my_dataset = f.create_dataset("my_data", complexType.getType(), dspace(dims));
    my_dataset.write(my_values.data());


    compound_dtype crazyType (sizeof(crazy));
    crazyType.insert("one" , dtype::Double);
    crazyType.insert("two" , dtype::Int);
    crazyType.insert("three" , dtype::String);

    crazy out[2];
    out[0] = crazy(1.3, 0, "hi");
    out[1] = crazy(1.3, -1, "bye");

    dims = {2};
    auto crazy_dataset = f.create_dataset("crazy", crazyType.getType(), dspace(dims));
    crazy_dataset.write(&(out[0]));

}


