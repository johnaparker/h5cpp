#include "h5group.h"
#include <iostream>

using namespace std;

namespace h5cpp {

h5group::h5group(string name, hid_t where): name(name) {
    group_id = H5Gcreate2(where, name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
}

h5group::h5group(hid_t group_id): group_id(group_id) {
    const int MAX_NAME = 1024;
    char group_name[MAX_NAME];
    H5Iget_name(group_id, group_name, MAX_NAME); 
    name = string(group_name);
}

unique_ptr<h5dset> h5group::create_dataset(string name, dtype datatype,
        dataspace dspace) {
    auto new_dset = make_unique<h5dset>(name, group_id, datatype, dspace);
    return new_dset;
}

unique_ptr<h5attr> h5group::create_attribute(string name, dtype datatype,
        dataspace dspace) {
    auto new_attr = make_unique<h5attr>(name, group_id, datatype, dspace);
    return new_attr;
}

h5group::~h5group() {
    H5Gclose(group_id);
}

}
