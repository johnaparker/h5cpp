#include "h5file.h"
#include "h5err.h"
#include <stdexcept>
#include <iostream>

using namespace std;

namespace h5cpp {

const int MAX_NAME = 1024;

h5file::h5file() {
    file_id = -1;
}

h5file::h5file(h5file&& other): filename(other.filename), file_id(other.file_id),
                 prop_id(other.prop_id) {
    //set other to closed
    other.file_id = -1;
}

h5file& h5file::operator=(h5file&& other) {
    //close current resources
    close();

    //move
    filename = other.filename;
    file_id = other.file_id;
    prop_id = other.prop_id;

    //set other to closed
    other.file_id = -1;

    return *this;
}

h5file::h5file(string name, io flag, bool mpi): filename(name) {
    prop_id = H5Pcreate(H5P_FILE_ACCESS);
    if (mpi)
        H5Pset_fapl_mpio(prop_id, MPI_COMM_WORLD, MPI_INFO_NULL);

    switch(flag) {
        case io::w:
            file_id = H5Fcreate(name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT,
            prop_id); break;
        case io::wn:
            file_id = H5Fcreate(name.c_str(), H5F_ACC_EXCL, H5P_DEFAULT,
            prop_id); break;
        case io::wp: {
            error_lock err_lock;
            file_id = H5Fcreate(name.c_str(), H5F_ACC_EXCL, H5P_DEFAULT, prop_id);
            if (file_id < 0)
                file_id = H5Fopen(name.c_str(), H5F_ACC_RDWR, prop_id);
            break;
        }
        case io::r:
            file_id = H5Fopen(name.c_str(), H5F_ACC_RDONLY, prop_id);
            break;
        case io::rw:
            file_id = H5Fopen(name.c_str(), H5F_ACC_RDWR, prop_id);
            break;
    }
}

h5file::h5file(const h5file& other) {
    filename = other.filename;
    file_id = H5Freopen(other.file_id);
    prop_id = H5Fget_create_plist(other.file_id);
}

h5file& h5file::operator=(const h5file& rhs) {
    if (this != &rhs) {
        filename = rhs.filename;
        file_id = H5Freopen(rhs.file_id);
        prop_id = H5Fget_create_plist(rhs.file_id);
    }
    return *this;

}

h5group h5file::create_group(string name) const {
    auto new_group = h5group(name, file_id);
    return new_group;
}

h5dset h5file::create_dataset(string name, dtype datatype, 
        dspace dataspace) const {
    auto new_dset = h5dset(name, file_id, datatype, dataspace);
    return new_dset;
}

h5dset h5file::create_dataset(string name, hid_t datatype, 
        dspace dataspace) const {
    auto new_dset = h5dset(name, file_id, datatype, dataspace);
    return new_dset;
}

h5dset h5file::create_dataset(string name, dtypeCompound datatype, dspace dataspace) const {
    auto new_dset = h5dset(name, file_id, datatype.fileType(), dataspace);
    new_dset.datatype = datatype.memType();
    return new_dset;
}

h5attr h5file::create_attribute(string name, dtype datatype, dspace dataspace) const {
    auto new_attr = h5attr(name, file_id, datatype, dataspace);
    return new_attr;
}


h5group h5file::open_group(string name) const {
    hid_t group_id = H5Gopen2(file_id, name.c_str(), H5P_DEFAULT); 
    auto new_group = h5group(group_id);
    return new_group;
}

h5group h5file::open_group(h5ref reference) const {
    hid_t group_id = H5Rdereference(file_id, H5P_DEFAULT, H5R_OBJECT, &reference);
    auto new_group = h5group(group_id);
    return new_group;
}

h5dset h5file::open_dataset(string name) const {
    hid_t dset_id = H5Dopen2(file_id, name.c_str(), H5P_DEFAULT); 
    auto new_dset = h5dset(dset_id);
    return new_dset;
}


h5dset h5file::open_dataset(h5ref reference) const {
    hid_t dset_id = H5Rdereference(file_id, H5P_DEFAULT, H5R_OBJECT, &reference);
    auto new_dset = h5dset(dset_id);
    return new_dset;
}

h5attr h5file::open_attribute(string name, string base) const {
    hid_t attr_id = H5Aopen_by_name(file_id, base.c_str(),
                      name.c_str(), H5P_DEFAULT, H5P_DEFAULT); 
    auto new_attr = h5attr(attr_id);
    return new_attr;
}

h5attr h5file::open_attribute(hsize_t id) const {
    hid_t attr_id = H5Aopen_idx(file_id, id); 
    auto new_attr = h5attr(attr_id);
    return new_attr;
}

hsize_t h5file::num_attrs() const {
    return H5Aget_num_attrs(file_id);
}

hsize_t h5file::num_objects() const {
    hsize_t num_objs;
    H5Gget_num_objs(file_id, &num_objs);
    return num_objs;
}

string h5file::get_object_name(hsize_t idx) const {
    char group_name[MAX_NAME];
    H5Gget_objname_by_idx(file_id, idx, group_name, MAX_NAME);
    return group_name;
}

hsize_t h5file::get_object_type(hsize_t idx) const {
    return H5Gget_objtype_by_idx(file_id, idx);
}

h5group h5file::create_or_open_group(string name) const {
    if (object_exists(name))
        return open_group(name);
    else
        return create_group(name);
}

h5dset h5file::create_or_open_dataset(string name, dtype datatype, dspace dataspace) const {
    if (object_exists(name))
        return open_dataset(name);
    else
        return create_dataset(name, datatype, dataspace);
}

void h5file::create_reference(void* refer, string obj_name) const {
    H5Rcreate(refer, file_id, obj_name.c_str(),H5R_OBJECT,-1);
}

bool h5file::object_exists(string name) const {
    herr_t status = H5Eset_auto1(nullptr, nullptr);
    H5O_info_t object_info;
    status = H5Oget_info_by_name(file_id, name.c_str(), &object_info, H5P_DEFAULT);
    if (status >= 0)
        return true;
    else
        return false;
}

string h5file::get_name() const {
    return filename;
}

void h5file::close() {
    if (file_id != -1) {
        H5Pclose(prop_id);
        H5Fclose(file_id);
    }
}

h5file::~h5file() {
    close();

}

}
