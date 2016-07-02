#include "h5file.h"
#include <stdexcept>

using namespace std;

h5file::h5file(string name, unsigned flags): filename(name) {
    file_id = H5Fcreate(name.c_str(), flags, H5P_DEFAULT,
            H5P_DEFAULT);
}

void h5file::create_group(string name) {
    auto new_group = make_unique<h5group>(name, file_id);
    groups.insert(group_pair(name,move(new_group)));
}

void h5file::create_dataset(string name, string where, hid_t datatype,
            int drank, hsize_t* dims) {
    if (where == "/") {
        auto new_dset = make_unique<h5dset>(name, file_id, datatype, drank, dims);
        dsets.insert(dset_pair(name,move(new_dset)));
    }
    else {
        group_iter iter = groups.find(where);
        if (iter != groups.end())
            iter->second->create_dataset(name, datatype, drank, dims);
        else
            throw std::invalid_argument("Group name does not exist");
    }
}

void h5file::create_attribute(string name, string where, hid_t datatype,
            int drank, hsize_t* dims) {
    if (where == "/") {
        auto new_attr = make_unique<h5attr>(name, file_id, datatype, drank, dims);
        attrs.insert(attr_pair(name,move(new_attr)));
    }
    else {
        group_iter iter = groups.find(where);
        if (iter != groups.end())
            iter->second->create_attribute(name, datatype, drank, dims);
        else
            throw std::invalid_argument("Group name does not exist");
    }
}

h5file::~h5file() {
    H5Fclose(file_id);
}
