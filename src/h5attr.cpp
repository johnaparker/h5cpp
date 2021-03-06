#include "h5attr.h"
#include <iostream>
#include <string>

using namespace std;

namespace h5cpp {

const int MAX_NAME = 1024;

h5attr::h5attr(h5attr&& other): name(other.name), attr_id(other.attr_id), 
    dataspace(other.dataspace), datatype(other.datatype) {
        //set other to closed
        other.attr_id = -1;
}

h5attr& h5attr::operator=(h5attr&& other) {
    //close current resources
    close();

    //move
    name = other.name;
    attr_id = other.attr_id;
    dspace_id = other.dspace_id;
    dataspace = other.dataspace;
    datatype = other.datatype;

    //set other to closed
    other.attr_id= -1;

    return *this;
}

h5attr::h5attr(string name, hid_t where, dtype datatype_, dspace dataspace):
            name(name), dataspace(dataspace) {

    datatype = getDtype(datatype_);
    if (dataspace.isScalar())
        dspace_id = H5Screate(H5S_SCALAR);
    else
        dspace_id = H5Screate_simple(dataspace.drank, dataspace.dims.data(), nullptr);

    attr_id = H5Acreate2(where, name.c_str(), datatype,
                    dspace_id, H5P_DEFAULT,H5P_DEFAULT);
}

h5attr::h5attr(hid_t attr_id): attr_id(attr_id) {
    char attr_name[MAX_NAME];
    H5Aget_name(attr_id, MAX_NAME, attr_name); 
    name = string(attr_name);

    dspace_id = H5Aget_space(attr_id);
    datatype = H5Aget_type(attr_id);

    dataspace = dspace(dspace_id); 
}

void h5attr::write(const void* data) const {
    H5Awrite(attr_id, datatype, data);
}

void h5attr::read(void* dest) const {
    H5Aread(attr_id, datatype, dest);
}


const dspace h5attr::get_dspace() const {
    return dataspace;
};

hid_t h5attr::get_dtype() const {
    return H5Tget_class(datatype);
};

std::string h5attr::get_path() const {
    char c_path[MAX_NAME];
    H5Iget_name(attr_id, c_path, MAX_NAME); 
    return string(c_path) + string("/") + get_name();
}

void h5attr::close() {
    if (attr_id != -1) {
        H5Aclose(attr_id);
        H5Sclose(dspace_id);
    }
}

h5attr::~h5attr() {
    close();
}

}
