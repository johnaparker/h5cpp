#include "h5attr.h"
#include <iostream>

using namespace std;


h5attr::h5attr(string name, hid_t where, hid_t datatype, dataspace dspace):
            name(name), datatype(datatype), dspace(dspace) {

    dspace_id = H5Screate_simple(dspace.drank, dspace.dims.data(), nullptr);
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

    dspace = dataspace(dspace_id); 
}

void h5attr::write(const void* data) {
    status = H5Awrite(attr_id, datatype, data);
}


h5attr::~h5attr() {
    H5Aclose(attr_id);
    H5Sclose(dspace_id);
}
