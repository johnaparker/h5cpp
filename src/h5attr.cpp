#include "h5attr.h"
#include <iostream>
#include <string>

using namespace std;

namespace h5cpp {

h5attr::h5attr(string name, hid_t where, dtype datatype_, dspace dataspace):
            name(name), dataspace(dataspace) {

    datatype = getDtype(datatype_);
    dspace_id = H5Screate_simple(dataspace.drank, dataspace.dims.data(), nullptr);
    attr_id = H5Acreate2(where, name.c_str(), datatype,
                    dspace_id, H5P_DEFAULT,H5P_DEFAULT);
}

h5attr::h5attr(hid_t attr_id): attr_id(attr_id) {
    const int MAX_NAME = 1024;
    char attr_name[MAX_NAME];
    H5Aget_name(attr_id, MAX_NAME, attr_name); 
    name = string(attr_name);

    dspace_id = H5Aget_space(attr_id);
    datatype = H5Aget_type(attr_id);

    dataspace = dspace(dspace_id); 
}

void h5attr::write(const void* data) {
    status = H5Awrite(attr_id, datatype, data);
}

void h5attr::read(void* dest) {
    status = H5Aread(attr_id, datatype, dest);
}


const dspace h5attr::get_dspace() {
    return dataspace;
};

h5attr::~h5attr() {
    H5Aclose(attr_id);
    H5Sclose(dspace_id);
}

}
