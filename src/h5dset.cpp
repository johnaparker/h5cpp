#include "h5dset.h"
#include <iostream>

using namespace std;

namespace h5cpp {

h5dset::h5dset(string name, hid_t where, dtype datatype_, dataspace dspace):
      name(name), dspace(dspace) {

    datatype = getDtype(datatype_);
    memspace = H5P_DEFAULT;
    prop = H5P_DEFAULT;
    if (dspace.chunked || dspace.extendable) {
        prop = H5Pcreate(H5P_DATASET_CREATE);
        status = H5Pset_chunk(prop, dspace.drank, dspace.chunk_dims.data());
        if (dspace.compressed) {
            status = H5Pset_deflate(prop, 6);
        }
    }

    dspace_id = H5Screate_simple(dspace.drank, dspace.dims.data(), 
                                 dspace.max_dims.data());
    dset_id = H5Dcreate2(where, name.c_str(), datatype,
                    dspace_id, H5P_DEFAULT, prop,
                    H5P_DEFAULT);
}

h5dset::h5dset(hid_t dset_id): dset_id(dset_id) {

    const int MAX_NAME = 1024; 
    char dset_name[MAX_NAME];

    H5Iget_name(dset_id, dset_name, MAX_NAME); 
    name = string(dset_name);

    dspace_id = H5Dget_space(dset_id);
    datatype = H5Dget_type(dset_id);
    prop = H5Dget_create_plist(dset_id);

    dspace = dataspace(dspace_id, prop); 
}

unique_ptr<h5attr> h5dset::create_attribute(string name, dtype datatype_,
        dataspace dspace) {

    auto new_attr = make_unique<h5attr>(name, dset_id, datatype_, dspace);
    return new_attr;
}

void h5dset::extend(vector<hsize_t> size) {
    H5Dset_extent(dset_id, size.data());
    dspace.dims = size;
}


void h5dset::write(const void* data) {
    status = H5Dwrite(dset_id, datatype, H5S_ALL, H5S_ALL,
           H5P_DEFAULT, data);
}

void h5dset::select_write(const void* data, std::vector<hsize_t> offset, std::vector<hsize_t> count, std::vector<hsize_t> stride, std::vector<hsize_t> block) {
    
    select(offset, count, stride, block);
    status = H5Dwrite(dset_id, datatype, memspace, filespace, 
                 H5P_DEFAULT, data);
    H5Sclose(memspace);
    H5Sclose(filespace);
}

void h5dset::append(const void* data) {
    vector<hsize_t> new_dims(dspace.dims);
    new_dims[dspace.drank-1] += 1;
    extend(new_dims);

    vector<hsize_t> offset(dspace.drank, 0);
    offset[dspace.drank-1] = new_dims[dspace.drank-1] - 1;
    vector<hsize_t> count(new_dims);
    count[dspace.drank-1] = 1;
    select(offset,count);

    write(data);
}

h5dset::~h5dset() {
    H5Pclose(prop);
    H5Dclose(dset_id);
} 

void h5dset::select(vector<hsize_t> offset, vector<hsize_t> count,
                    vector<hsize_t> stride, vector<hsize_t> block) {

    filespace = H5Dget_space(dset_id);
    status = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(),
            stride.data(), count.data(), block.data());
    memspace = H5Screate_simple(dspace.drank, count.data(), nullptr);
}

}
