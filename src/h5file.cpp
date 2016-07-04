#include "h5file.h"
#include <stdexcept>
#include <iostream>

using namespace std;

h5file::h5file(string name, unsigned flags): filename(name) {
    file_id = H5Fcreate(name.c_str(), flags, H5P_DEFAULT,
            H5P_DEFAULT);
}

unique_ptr<h5group> h5file::create_group(string name) {
    auto new_group = make_unique<h5group>(name, file_id);
    return new_group;
}

unique_ptr<h5dset> h5file::create_dataset(string name, hid_t datatype, 
        h5dspace dspace) {
    auto new_dset = make_unique<h5dset>(name, file_id, datatype, dspace);
    return new_dset;
}

unique_ptr<h5attr> h5file::create_attribute(string name, hid_t datatype,
            vector<hsize_t> dims) {
    auto new_attr = make_unique<h5attr>(name, file_id, datatype, dims);
    return new_attr;
}


unique_ptr<h5group> h5file::open_group(string name) {
    hid_t group_id = H5Gopen2(file_id, name.c_str(), H5P_DEFAULT); 
    auto new_group = make_unique<h5group>(group_id);
    return new_group;
}

unique_ptr<h5dset> h5file::open_dataset(string name) {
    hid_t dset_id = H5Dopen2(file_id, name.c_str(), H5P_DEFAULT); 
    auto new_dset = make_unique<h5dset>(dset_id);
    return new_dset;
}

unique_ptr<h5attr> h5file::open_attribute(string name) {
    hid_t attr_id = H5Aopen(file_id, name.c_str(), H5P_DEFAULT); 
    auto new_attr = make_unique<h5attr>(attr_id);
    return new_attr;
}

unique_ptr<h5attr> h5file::open_attribute_by_name(string attr_name, string name) {
    hid_t attr_id = H5Aopen_by_name(file_id, name.c_str(),
                      attr_name.c_str(), H5P_DEFAULT, H5P_DEFAULT); 
    auto new_attr = make_unique<h5attr>(attr_id);
    return new_attr;
}

h5file::~h5file() {
    H5Fclose(file_id);
}
