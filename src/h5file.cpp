#include "h5file.h"
#include "h5err.h"
#include <stdexcept>
#include <iostream>

using namespace std;

namespace h5cpp {

h5file::h5file(string name, io flag): filename(name) {
    switch(flag) {
        case io::w:
            file_id = H5Fcreate(name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT,
            H5P_DEFAULT); break;
        case io::wn:
            file_id = H5Fcreate(name.c_str(), H5F_ACC_EXCL, H5P_DEFAULT,
            H5P_DEFAULT); break;
        case io::wp: {
            error_lock err_lock;
            file_id = H5Fcreate(name.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, H5P_DEFAULT);
            if (file_id < 0)
                file_id = H5Fopen(name.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
            break;
        }
        case io::r:
            file_id = H5Fopen(name.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
            break;
        case io::rw:
            file_id = H5Fopen(name.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
            break;
    }
}

h5group h5file::create_group(string name) {
    auto new_group = h5group(name, file_id);
    return new_group;
}

h5dset h5file::create_dataset(string name, dtype datatype, 
        dspace dataspace) {
    auto new_dset = h5dset(name, file_id, datatype, dataspace);
    return new_dset;
}

h5dset h5file::create_dataset(string name, hid_t datatype, 
        dspace dataspace) {
    auto new_dset = h5dset(name, file_id, datatype, dataspace);
    return new_dset;
}

h5dset h5file::create_dataset(string name, dtypeCompound datatype, dspace dataspace) {
    auto new_dset = h5dset(name, file_id, datatype.fileType(), dataspace);
    new_dset.datatype = datatype.memType();
    return new_dset;
}

h5attr h5file::create_attribute(string name, dtype datatype, dspace dataspace) {
    auto new_attr = h5attr(name, file_id, datatype, dataspace);
    return new_attr;
}


h5group h5file::open_group(string name) {
    hid_t group_id = H5Gopen2(file_id, name.c_str(), H5P_DEFAULT); 
    auto new_group = h5group(group_id);
    return new_group;
}

h5dset h5file::open_dataset(string name) {
    hid_t dset_id = H5Dopen2(file_id, name.c_str(), H5P_DEFAULT); 
    auto new_dset = h5dset(dset_id);
    return new_dset;
}

h5attr h5file::open_attribute(string name, string base) {
    hid_t attr_id = H5Aopen_by_name(file_id, base.c_str(),
                      name.c_str(), H5P_DEFAULT, H5P_DEFAULT); 
    auto new_attr = h5attr(attr_id);
    return new_attr;
}


h5group h5file::create_or_open_group(string name) {
    if (object_exists(name))
        return open_group(name);
    else
        return create_group(name);
}

h5dset h5file::create_or_open_dataset(string name, dtype datatype, dspace dataspace) {
    if (object_exists(name))
        return open_dataset(name);
    else
        return create_dataset(name, datatype, dataspace);
}

bool h5file::object_exists(string name) {
    status = H5Eset_auto1(nullptr, nullptr);
    H5O_info_t object_info;
    status = H5Oget_info_by_name(file_id, name.c_str(), &object_info, H5P_DEFAULT);
    if (status >= 0)
        return true;
    else
        return false;
}

h5file::~h5file() {
    H5Fclose(file_id);
}

}
