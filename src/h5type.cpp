#include "h5type.h"
#include <stdexcept>
#include <complex>

using namespace std;

namespace h5cpp {

hid_t getDtype(dtype datatype) {
    switch(datatype) {
        case dtype::Int: return H5T_NATIVE_INT; break;
        case dtype::UInt: return H5T_NATIVE_UINT; break;
        case dtype::Short: return H5T_NATIVE_SHORT; break;
        case dtype::UShort: return H5T_NATIVE_USHORT; break;
        case dtype::Long: return H5T_NATIVE_LONG; break;
        case dtype::ULong: return H5T_NATIVE_ULONG; break;
        case dtype::LLong: return H5T_NATIVE_LLONG; break;
        case dtype::ULLong: return H5T_NATIVE_ULLONG; break;
        case dtype::Float: return H5T_NATIVE_FLOAT; break;
        case dtype::Double: return H5T_NATIVE_DOUBLE; break;
        case dtype::Reference: return H5T_STD_REF_OBJ; break;

        case dtype::String: 
            return H5Tcreate(H5T_STRING, H5T_VARIABLE); break;

        case dtype::Complexf: {
            dtypeCompound complexType (sizeof(complex<float>));
            complexType.insert("real", dtype::Double);
            complexType.insert("imag", dtype::Double);
            return complexType.fileType();
            break;
        }

        case dtype::Complexd: {
            dtypeCompound complexType (sizeof(complex<double>));
            complexType.insert("real", dtype::Double);
            complexType.insert("imag", dtype::Double);
            return complexType.fileType();
            break;
        }

        default: throw invalid_argument("No corresponding H5datatype to your datatype");
    }
}

hid_t dtypeArray(dtype datatype, vector<hsize_t> dims) {
    return H5Tarray_create(getDtype(datatype), dims.size(), &(dims[0]));
}


dtypeCompound::dtypeCompound(size_t memsize) {
    memtype = H5Tcreate (H5T_COMPOUND, memsize);
    memDisplace = 0;
    fileSize = 0;
}

void dtypeCompound::insert(string name, dtype datatype) {
    hid_t h5dtype = getDtype(datatype);
    if (datatype == dtype::String) {
        memDisplace += sizeof(char*);
        memDisplace -= H5Tget_size(h5dtype);
    }
    insert(name, h5dtype);
}

void dtypeCompound::insert(string name, hid_t h5dtype) {
    status = H5Tinsert (memtype, name.c_str(),
                memDisplace, h5dtype);
    memDisplace += H5Tget_size(h5dtype);

    fileDisplacements.push_back(fileSize);
    fileSize += H5Tget_size(h5dtype);
    names.push_back(name);
    types.push_back(h5dtype);
}

hid_t dtypeCompound::memType() {
    return memtype;
}

hid_t dtypeCompound::fileType() {

    hid_t filetype = H5Tcreate (H5T_COMPOUND, fileSize);
    for (size_t i = 0; i != names.size(); i++) {
        status = H5Tinsert (filetype, names[i].c_str(), fileDisplacements[i], types[i]);
    }

    return filetype;
}


}
