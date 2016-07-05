#include "h5dset.h"
#include <iostream>

using namespace std;


h5dset::h5dset(string name, hid_t where, hid_t datatype,
     h5dspace dspace, vector<hsize_t> chunk_dims, bool compressed): 
      name(name), datatype(datatype), dspace(dspace), chunk_dims(chunk_dims),
        compressed(compressed) {

    memspace = H5P_DEFAULT;
    prop = H5P_DEFAULT;
    if (dspace.isExtendable()) {
        prop = H5Pcreate(H5P_DATASET_CREATE);
        status = H5Pset_chunk(prop, dspace.rank(), &chunk_dims[0]);
    }

    dset_id = H5Dcreate2(where, name.c_str(), datatype,
                    dspace.id(), H5P_DEFAULT, prop,
                    H5P_DEFAULT);
}

h5dset::h5dset(hid_t dset_id): dset_id(dset_id), 
                  dspace(H5Dget_space(dset_id)) { 

    const int MAX_NAME = 1024; 
    char dset_name[MAX_NAME];

    H5Iget_name(dset_id, dset_name, MAX_NAME); 
    name = string(dset_name);
    datatype = H5Dget_type(dset_id);
}

unique_ptr<h5attr> h5dset::create_attribute(string name, hid_t datatype,
        vector<hsize_t> dims) {

    auto new_attr = make_unique<h5attr>(name, dset_id, datatype, dims);
    return new_attr;
}

void h5dset::extend(vector<hsize_t> size) {
    H5Dset_extent(dset_id, &size[0]);
}

void h5dset::select(vector<hsize_t> offset, vector<hsize_t> count,
                    vector<hsize_t> stride, vector<hsize_t> block) {

    filespace = H5Dget_space(dset_id);
    status = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(),
            stride.data(), count.data(), block.data());
    memspace = H5Screate_simple(dspace.rank(), count.data(), nullptr);
}

void h5dset::write(const void* data) {
    if (!dspace.isExtendable())
        status = H5Dwrite(dset_id, datatype, H5S_ALL, H5S_ALL,
               H5P_DEFAULT, data);
    else {
        status = H5Dwrite(dset_id, datatype, memspace, filespace, 
                     H5P_DEFAULT, data);
        H5Sclose(memspace);
        H5Sclose(filespace);
    }
}

void h5dset::append(const void* data) {
    hid_t ds = H5Dget_space(dset_id);
    hsize_t drank = H5Sget_simple_extent_ndims(ds);
    auto p_dims = make_unique<hsize_t[]>(drank);
    auto p_max_dims = make_unique<hsize_t[]>(drank);
    H5Sget_simple_extent_dims(ds, p_dims.get(), p_max_dims.get());
    
    vector<hsize_t> new_dims;
    new_dims.assign(p_dims.get(), p_dims.get()+drank);
    new_dims[drank-1] += 1;
    extend(new_dims);

    vector<hsize_t> offset(drank, 0);
    offset[drank-1] = new_dims[drank-1] - 1;
    vector<hsize_t> count(new_dims);
    count[drank-1] = 1;
    select(offset,count);

    write(data);
}

h5dset::~h5dset() {
    H5Pclose(prop);
    H5Dclose(dset_id);
} 
