#include "h5dset.h"
#include <iostream>

using namespace std;


h5dset::h5dset(string name, hid_t where, hid_t datatype,
     h5dspace dspace, vector<hsize_t> chunk_dims, bool compressed): 
      name(name), datatype(datatype), dspace(dspace), chunk_dims(chunk_dims),
        compressed(compressed) {

    memspace = H5P_DEFAULT;
    prop = H5P_DEFAULT;
    if (dspace.isExtendable()) {
        prop = H5Pcreate(H5P_DATASET_CREATE);
        status = H5Pset_chunk(prop, dspace.rank(), &chunk_dims[0]);
    }

    dset_id = H5Dcreate2(where, name.c_str(), datatype,
                    dspace.id(), H5P_DEFAULT, prop,
                    H5P_DEFAULT);
}

h5dset::h5dset(hid_t dset_id): dset_id(dset_id), 
                  dspace(H5Dget_space(dset_id)) { 

    const int MAX_NAME = 1024; 
    char dset_name[MAX_NAME];

    H5Iget_name(dset_id, dset_name, MAX_NAME); 
    name = string(dset_name);
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
    H5Pclose(prop);
    H5Dclose(dset_id);
} 
