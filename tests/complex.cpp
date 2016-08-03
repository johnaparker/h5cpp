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

class compound_dtype {
public:
    compound_dtype(size_t memsize) {
        memtype = H5Tcreate (H5T_COMPOUND, memsize);
        memDisplace = 0;
        fileSize = 0;
    }

    void insert(string name, dtype datatype) {
        hid_t h5dtype = getDtype(datatype);
        if (datatype == dtype::String) {
            memDisplace += sizeof(char*);
            memDisplace -= H5Tget_size(h5dtype);
        }
        insert(name, h5dtype);
    }

    void insert(string name, hid_t h5dtype) {
        status = H5Tinsert (memtype, name.c_str(),
                    memDisplace, h5dtype);
        memDisplace += H5Tget_size(h5dtype);

        fileDisplacements.push_back(fileSize);
        fileSize += H5Tget_size(h5dtype);
        names.push_back(name);
        types.push_back(h5dtype);
    }

    hid_t memType() {
        return memtype;
    }

    hid_t fileType() {

        hid_t filetype = H5Tcreate (H5T_COMPOUND, fileSize);
        for (size_t i = 0; i != names.size(); i++) {
            status = H5Tinsert (filetype, names[i].c_str(), fileDisplacements[i], types[i]);
        }

        return filetype;
    }

private:
    hid_t memtype;
    herr_t status;
    size_t memDisplace;
    size_t fileSize;

    vector<string> names;
    vector<hid_t> types;
    vector<hsize_t> fileDisplacements;
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
    auto my_dataset = f.create_dataset("my_data", complexType.memType(), dspace(dims));
    my_dataset.write(my_values.data());


    dims = {2};
    hsize_t dims_[1] = {2};
    hid_t array_type = H5Tarray_create(H5T_NATIVE_DOUBLE, 1, dims_);

    compound_dtype crazyType (sizeof(crazy));
    crazyType.insert("one" , dtype::Double);
    crazyType.insert("data" , array_type);
    crazyType.insert("two" , dtype::String);
    crazyType.insert("three" , dtype::Int);

    crazy out[2];
    out[0] = crazy(1.3, "hi",1);
    out[1] = crazy(1.3, "bye",1);

    auto crazy_dataset = f.create_dataset("crazy", crazyType.fileType() , dspace(dims));
    crazy_dataset.write(&(out[0]), crazyType.memType());

}


