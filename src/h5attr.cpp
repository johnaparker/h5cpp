#include "h5attr.h"
#include <iostream>

using namespace std;


h5attr::h5attr(string name, hid_t where, hid_t datatype,
            vector<hsize_t> dims): name(name), drank(dims.size()),
                              dims(dims), datatype(datatype) {
    dataspace_id = H5Screate_simple(drank, dims.data(), nullptr);
    attr_id = H5Acreate2(where, name.c_str(), datatype,
                    dataspace_id, H5P_DEFAULT,H5P_DEFAULT);
}

h5attr::h5attr(hid_t attr_id): attr_id(attr_id) {
    const int MAX_NAME = 1024;
    char attr_name[MAX_NAME];
    H5Aget_name(attr_id, MAX_NAME, attr_name); 
    name = string(attr_name);

    dataspace_id = H5Aget_space(attr_id);

    drank = H5Sget_simple_extent_ndims(dataspace_id);
    auto p_dims = make_unique<hsize_t[]>(drank);
    auto p_max_dims = make_unique<hsize_t[]>(drank);
    H5Sget_simple_extent_dims(dataspace_id, p_dims.get(), p_max_dims.get());

    dims.assign(p_dims.get(), p_dims.get()+drank);
    max_dims.assign(p_max_dims.get(), p_max_dims.get()+drank);
    datatype = H5Aget_type(attr_id);
}

void h5attr::write(const void* data) {
    status = H5Awrite(attr_id, datatype, data);
}


h5attr::~h5attr() {
    H5Aclose(attr_id);
    H5Sclose(dataspace_id);
}
