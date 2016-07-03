#include "h5file.h"
#include <stdexcept>
#include <iostream>

using namespace std;

h5file::h5file(string name, unsigned flags): filename(name) {
    file_id = H5Fcreate(name.c_str(), flags, H5P_DEFAULT,
            H5P_DEFAULT);
}

group_iter h5file::find_group(std::string name) {
    group_iter iter = groups.find(name);
    if (iter == groups.end()) {
        string err = "Group \'" + name + "\' does not exist";
        throw std::invalid_argument(err);
    }
    return iter;
}

dset_iter h5file::find_dset(std::string name) {
    dset_iter iter = dsets.find(name);
    if (iter == dsets.end()) {
        string err = "Dataset \'" + name + "\' does not exist";
        throw std::invalid_argument(err);
    }
    return iter;
}

void h5file::create_group(string name) {
    auto new_group = make_unique<h5group>(name, file_id);
    groups.insert(group_pair(name,move(new_group)));
}

void h5file::create_dataset(string name, string where, hid_t datatype,
            vector<hsize_t> dims) {
    if (where == "/") {
        auto new_dset = make_unique<h5dset>(name, file_id, datatype, dims);
        dsets.insert(dset_pair(name,move(new_dset)));
    }
    else {
        group_iter iter = find_group(where);
        iter->second->create_dataset(name, datatype, dims);
    }
}

void h5file::create_attribute(string name, string where, hid_t datatype,
            vector<hsize_t> dims) {
    if (where == "/") {
        auto new_attr = make_unique<h5attr>(name, file_id, datatype, dims);
        attrs.insert(attr_pair(name,move(new_attr)));
    }
    else {
        group_iter iter = find_group(where);
        iter->second->create_attribute(name, datatype, dims);
    }
}

void h5file::write_data(string name, string where, const void* data, hid_t datatype) {
    if (where == "/") {
        dset_iter iter = find_dset(name);
        iter->second->write(data, datatype);
    }
    else {
        group_iter g_iter = find_group(where);
        dset_iter d_iter = g_iter->second->find_dset(name);
        d_iter->second->write(data, datatype);
    }
}

h5file::~h5file() {
    H5Fclose(file_id);
}
