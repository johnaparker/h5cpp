#include "h5dset.h"
#include "hdf5.h"
#include <iostream>

using namespace std;

namespace h5cpp {

h5dset::h5dset() {
    dset_id = -1;
};

h5dset::h5dset(h5dset&& other): name(other.name), dset_id(other.dset_id), dspace_id(other.dspace_id), 
    filespace(other.filespace), memspace(other.memspace), prop(other.prop),
    dataspace(other.dataspace), datatype(other.datatype) {
        //set other to closed
        other.dset_id = -1;
}

h5dset& h5dset::operator=(h5dset&& other) {
    //close current resources
    close();

    //move
    name = other.name;
    dset_id = other.dset_id;
    dspace_id = other.dspace_id;
    filespace = other.filespace;
    memspace = other.memspace;
    prop = other.prop;
    dataspace = other.dataspace;
    datatype = other.datatype;

    //set other to closed
    other.dset_id = -1;

    return *this;
}

h5dset::h5dset(string name, hid_t where, hid_t datatype_, dspace dataspace):
      name(name), dataspace(dataspace), datatype(datatype_) {
    memspace = H5P_DEFAULT;
    prop = H5P_DEFAULT;
    if (dataspace.chunked || dataspace.extendable) {
        prop = H5Pcreate(H5P_DATASET_CREATE);
        H5Pset_chunk(prop, dataspace.drank, dataspace.chunk_dims.data());
        if (dataspace.compressed) {
            H5Pset_deflate(prop, 6);
        }
    }

    if (dataspace.isScalar())
        dspace_id = H5Screate(H5S_SCALAR);
    else
        dspace_id = H5Screate_simple(dataspace.drank, dataspace.dims.data(), 
                                 dataspace.max_dims.data());

    dset_id = H5Dcreate2(where, name.c_str(), datatype,
                    dspace_id, H5P_DEFAULT, prop,
                    H5P_DEFAULT);
}

h5dset::h5dset(string name, hid_t where, dtype datatype_, dspace dataspace):
      h5dset(name, where, getDtype(datatype_), dataspace) {}

h5dset::h5dset(hid_t dset_id): dset_id(dset_id) {
    auto path = get_path();
    name = path.substr( path.rfind("/") );

    dspace_id = H5Dget_space(dset_id);
    datatype = H5Dget_type(dset_id);
    prop = H5Dget_create_plist(dset_id);

    dataspace = dspace(dspace_id, prop); 
}

h5attr h5dset::create_attribute(string name, dtype datatype_,
        dspace dataspace) const {

    auto new_attr = h5attr(name, dset_id, datatype_, dataspace);
    return new_attr;
}

void h5dset::extend(vector<hsize_t> size) {
    H5Dset_extent(dset_id, size.data());
    dataspace.dims = size;
}


void h5dset::write(const void* data) const {
    H5Dwrite(dset_id, datatype, H5S_ALL, H5S_ALL,
           H5P_DEFAULT, data);
}

void h5dset::select_write(const void* data, std::vector<hsize_t> offset, std::vector<hsize_t> count, std::vector<hsize_t> stride, std::vector<hsize_t> block) {
    
    select(offset, count, stride, block);
    H5Dwrite(dset_id, datatype, memspace, filespace, 
                 H5P_DEFAULT, data);
    H5Sclose(memspace);
    H5Sclose(filespace);
}

void h5dset::append(const void* data) {
    vector<hsize_t> new_dims(dataspace.dims);
    new_dims[dataspace.drank-1] += 1;
    extend(new_dims);

    vector<hsize_t> offset(dataspace.drank, 0);
    offset[dataspace.drank-1] = new_dims[dataspace.drank-1] - 1;
    vector<hsize_t> count(new_dims);
    count[dataspace.drank-1] = 1;

    select_write(data, offset, count);
}

void h5dset::read(void* dest) const {
    H5Dread(dset_id, datatype, H5S_ALL, H5S_ALL,
           H5P_DEFAULT, dest);
}

void h5dset::select_read(void* dest, std::vector<hsize_t> offset, std::vector<hsize_t> count, std::vector<hsize_t> stride, std::vector<hsize_t> block) {
    
    select(offset, count, stride, block);
    H5Dread(dset_id, datatype, memspace, filespace, 
                 H5P_DEFAULT, dest);
    H5Sclose(memspace);
    H5Sclose(filespace);
}

h5attr h5dset::open_attribute(string name) const {
    hid_t attr_id = H5Aopen(dset_id, name.c_str(), H5P_DEFAULT); 
    auto new_attr = h5attr(attr_id);
    return new_attr;
}

h5attr h5dset::open_attribute(hsize_t id) const {
    hid_t attr_id = H5Aopen_idx(dset_id, id); 
    auto new_attr = h5attr(attr_id);
    return new_attr;
}

hsize_t h5dset::num_attrs() const {
    return H5Aget_num_attrs(dset_id);
}


std::string h5dset::get_path() const {
    char c_path[MAX_NAME];
    H5Iget_name(dset_id, c_path, MAX_NAME); 
    return string(c_path);
}

void h5dset::close() {
    if (dset_id != -1) {
        H5Pclose(prop);
        H5Dclose(dset_id);
        H5Sclose(dspace_id);
        dset_id = -1;
    }
}

h5dset::~h5dset() {
    close();
} 

void h5dset::select(vector<hsize_t> offset, vector<hsize_t> count,
                    vector<hsize_t> stride, vector<hsize_t> block) {

    filespace = H5Dget_space(dset_id);
    H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(),
            stride.data(), count.data(), block.data());
    memspace = H5Screate_simple(dataspace.drank, count.data(), nullptr);
}

}
