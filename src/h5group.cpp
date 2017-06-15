#include "h5group.h"
#include "h5err.h"
#include <iostream>

using namespace std;

namespace h5cpp {

h5group::h5group() {
    group_id = -1;
}

h5group::h5group(h5group&& other): name(other.name), group_id(other.group_id) {
        //set other to closed
        other.group_id = -1;
}

h5group& h5group::operator=(h5group&& other) {
    //close current resources
    close();

    //move
    name = other.name;
    group_id = other.group_id;

    //set other to closed
    other.group_id = -1;

    return *this;
}

h5group::h5group(string name, hid_t where): name(name) {
    group_id = H5Gcreate2(where, name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
}

h5group::h5group(hid_t group_id): group_id(group_id) {
    auto path = get_path();
    name = path.substr( path.rfind("/") + 1 );
}

h5group h5group::create_group(string name) const {
    auto new_group = h5group(name, group_id);
    return new_group;
}

h5dset h5group::create_dataset(string name, dtype datatype,
        dspace dataspace) const {
    auto new_dset = h5dset(name, group_id, datatype, dataspace);
    return new_dset;
}

h5dset h5group::create_dataset(string name, dtypeCompound datatype, dspace dataspace) const {
    auto new_dset = h5dset(name, group_id, datatype.fileType(), dataspace);
    new_dset.datatype = datatype.memType();
    return new_dset;
}

h5attr h5group::create_attribute(string name, dtype datatype,
        dspace dataspace) const {
    auto new_attr = h5attr(name, group_id, datatype, dataspace);
    return new_attr;
}

h5group h5group::open_group(string name) const {
    hid_t new_group_id = H5Gopen2(group_id, name.c_str(), H5P_DEFAULT); 
    auto new_group = h5group(new_group_id);
    return new_group;
}

h5group h5group::open_group(h5ref reference) const {
    hid_t group_id = H5Rdereference(group_id, H5P_DEFAULT, H5R_OBJECT, &reference);
    auto new_group = h5group(group_id);
    return new_group;
}

h5dset h5group::open_dataset(string name) const {
    hid_t dset_id = H5Dopen2(group_id, name.c_str(), H5P_DEFAULT); 
    auto new_dset = h5dset(dset_id);
    return new_dset;
}

h5dset h5group::open_dataset(h5ref reference) const {
    hid_t dset_id = H5Rdereference(group_id, H5P_DEFAULT, H5R_OBJECT, &reference);
    auto new_dset = h5dset(dset_id);
    return new_dset;
}

h5attr h5group::open_attribute(string name) const {
    hid_t attr_id = H5Aopen(group_id, name.c_str(), H5P_DEFAULT); 
    auto new_attr = h5attr(attr_id);
    return new_attr;
}

h5attr h5group::open_attribute(hsize_t id) const {
    hid_t attr_id = H5Aopen_idx(group_id, id); 
    auto new_attr = h5attr(attr_id);
    return new_attr;
}

hsize_t h5group::num_attrs() const {
    return H5Aget_num_attrs(group_id);
}

hsize_t h5group::num_objects() const {
    hsize_t num_objs;
    H5Gget_num_objs(group_id, &num_objs);
    return num_objs;
}

string h5group::get_object_name(hsize_t idx) const {
    char group_name[MAX_NAME];
    H5Gget_objname_by_idx(group_id, idx, group_name, MAX_NAME);
    return group_name;
}

hsize_t h5group::get_object_type(hsize_t idx) const {
    return H5Gget_objtype_by_idx(group_id, idx);
}

h5group h5group::create_or_open_group(string name) const {
    if (object_exists(name))
        return open_group(name);
    else
        return create_group(name);
}

h5dset h5group::create_or_open_dataset(string name, dtype datatype, dspace dataspace) const {
    if (object_exists(name))
        return open_dataset(name);
    else
        return create_dataset(name, datatype, dataspace);
}

h5ref h5group::create_reference(string obj_name) const {
    h5ref w;
    H5Rcreate(&w, group_id, obj_name.c_str(),H5R_OBJECT,-1);
    return w;
}

bool h5group::object_exists(string name) const {
    error_lock err_lock;
    H5O_info_t object_info;
    herr_t status = H5Oget_info_by_name(group_id, name.c_str(), &object_info, H5P_DEFAULT);
    if (status >= 0)
        return true;
    else
        return false;
}

std::string h5group::get_path() const {
    char path[MAX_NAME];
    H5Iget_name(group_id, path, MAX_NAME); 
    return string(path);
}

void h5group::close() {
    if (group_id != -1) {
        H5Gclose(group_id);
        group_id = -1;
    }
}

h5group::~h5group() {
    close();
}

}
