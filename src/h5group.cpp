#include "h5group.h"
#include <iostream>

using namespace std;

dset_iter h5group::find_dset(string name) {
    dset_iter iter = dsets.find(name);
    if (iter == dsets.end())
        throw std::invalid_argument("Dataset name does not exist");
    return iter;
}

h5group::h5group(string name, hid_t where): name(name) {
    group_id = H5Gcreate2(where, name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
    //status = H5Gclose(group_id);
}

void h5group::create_dataset(string name, hid_t datatype, int drank,
        hsize_t* dims) {
    auto new_dset = make_unique<h5dset>(name, group_id, datatype, drank, dims);
    dsets.insert(dset_pair(name,move(new_dset)));
}

void h5group::create_attribute(string name, hid_t datatype, int drank,
        hsize_t* dims) {
    auto new_attr = make_unique<h5attr>(name, group_id, datatype, drank, dims);
    attrs.insert(attr_pair(name,move(new_attr)));
}