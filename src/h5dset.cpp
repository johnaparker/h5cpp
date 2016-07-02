#include "h5dset.h"
#include <iostream>

using namespace std;


h5dset::h5dset(string name, hid_t where, hid_t datatype,
            int drank, hsize_t* dims): name(name) {

    dataspace_id = H5Screate_simple(drank, dims, nullptr);
    dset_id = H5Dcreate2(where, name.c_str(), datatype,
                    dataspace_id, H5P_DEFAULT,H5P_DEFAULT,
                    H5P_DEFAULT);
    //status = H5Dclose(dset_id);
    //status = H5Sclose(dataspace_id);
}

void h5dset::create_attribute(string name, hid_t datatype, int drank,
        hsize_t* dims) {

    auto new_attr = make_unique<h5attr>(name, dset_id, datatype, drank, dims);
    attrs.insert(attr_pair(name,move(new_attr)));
}
