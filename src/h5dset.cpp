#include "h5dset.h"
#include <iostream>

using namespace std;


h5dset::h5dset(string name, hid_t where, hid_t datatype,
            vector<hsize_t> dims): name(name), drank(dims.size()),
                        dims(dims), datatype(datatype) {

    hsize_t drank = dims.size();
    dataspace_id = H5Screate_simple(drank, &dims[0], nullptr);
    dset_id = H5Dcreate2(where, name.c_str(), datatype,
                    dataspace_id, H5P_DEFAULT,H5P_DEFAULT,
                    H5P_DEFAULT);
}

h5dset::h5dset(hid_t dset_id): dset_id(dset_id) {
    const int MAX_NAME = 1024;
    char dset_name[MAX_NAME];
    H5Iget_name(dset_id, dset_name, MAX_NAME); 

    name = string(dset_name);
    dataspace_id = H5Dget_space(dset_id);

    drank = H5Sget_simple_extent_ndims(dataspace_id);
    auto p_dims = make_unique<hsize_t[]>(drank);
    auto p_max_dims = make_unique<hsize_t[]>(drank);
    H5Sget_simple_extent_dims(dataspace_id, p_dims.get(), p_max_dims.get());

    dims.assign(p_dims.get(), p_dims.get()+drank);
    max_dims.assign(p_max_dims.get(), p_max_dims.get()+drank);
    datatype = H5Dget_type(dset_id);
}

unique_ptr<h5attr> h5dset::create_attribute(string name, hid_t datatype,
        vector<hsize_t> dims) {

    auto new_attr = make_unique<h5attr>(name, dset_id, datatype, dims);
    return new_attr;
}

void h5dset::write(const void* data) {
    status = H5Dwrite(dset_id, datatype, H5S_ALL, H5S_ALL,
               H5P_DEFAULT, data);
}

h5dset::~h5dset() {
    H5Dclose(dset_id);
    H5Sclose(dataspace_id);
} 
