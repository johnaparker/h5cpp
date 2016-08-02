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

unique_ptr<h5group> h5group::create_group(string name) {
    auto new_group = make_unique<h5group>(name, group_id);
    return new_group;
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

unique_ptr<h5group> h5group::open_group(string name) {
    hid_t new_group_id = H5Gopen2(group_id, name.c_str(), H5P_DEFAULT); 
    auto new_group = make_unique<h5group>(new_group_id);
    return new_group;
}

unique_ptr<h5dset> h5group::open_dataset(string name) {
    hid_t dset_id = H5Dopen2(group_id, name.c_str(), H5P_DEFAULT); 
    auto new_dset = make_unique<h5dset>(dset_id);
    return new_dset;
}

unique_ptr<h5attr> h5group::open_attribute(string name) {
    hid_t attr_id = H5Aopen(group_id, name.c_str(), H5P_DEFAULT); 
    auto new_attr = make_unique<h5attr>(attr_id);
    return new_attr;
}


unique_ptr<h5group> h5group::create_or_open_group(string name) {
    if (object_exists(name))
        return open_group(name);
    else
        return create_group(name);
}

unique_ptr<h5dset> h5group::create_or_open_dataset(string name, dtype datatype, dataspace dspace) {
    if (object_exists(name))
        return open_dataset(name);
    else
        return create_dataset(name, datatype, dspace);
}

bool h5group::object_exists(string name) {
    status = H5Eset_auto1(nullptr, nullptr);
    H5O_info_t object_info;
    status = H5Oget_info_by_name(group_id, name.c_str(), &object_info, H5P_DEFAULT);
    if (status >= 0)
        return true;
    else
        return false;
}

h5group::~h5group() {
    H5Gclose(group_id);
}

}
