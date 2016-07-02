#include <iostream>
#include "boost/multi_array.hpp"
#include "hdf5.h"
#include <memory>

using namespace std;

int main() {
    //boost array properties
    const int drank = 2;
    using array_type = boost::multi_array<int, drank>;
    using index = array_type::index;
    int n = 4;

    //initialize the array
    array_type A(boost::extents[n][n]);
    for (int i = 0; i != n; i++) {
        for (int j = 0; j != n; j++) {
            A[i][j] = i + j;
        }
    }

    //hdf5 handles and dims
    hid_t file_id, dataset_id, dataspace_id, attribute_id, group_id;
    herr_t status;
    hsize_t dims[2] = {hsize_t(n),hsize_t(n)};

    //create the file
    file_id = H5Fcreate("test.h5", H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    //create the group
    group_id = H5Gcreate2(file_id, "/sub", H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    //create and write a dataset to root, /
    dataspace_id = H5Screate_simple(drank, dims, nullptr);
    dataset_id = H5Dcreate2(file_id, "data", H5T_NATIVE_INT,
                    dataspace_id, H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
    status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, A.data()); 

    //write the same data to /sub
    dataset_id = H5Dcreate2(group_id, "data", H5T_NATIVE_INT,
                    dataspace_id, H5P_DEFAULT,H5P_DEFAULT,H5P_DEFAULT);
    status = H5Dwrite(dataset_id, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, A.data()); 
    

    //create an attribute
    int atr_len = 1;
    auto a_dims = make_unique<hsize_t[]>(atr_len);
    a_dims[0] = 1;
    double a_value = 3.2;

    //write the attribute
    dataspace_id = H5Screate_simple(1, a_dims.get(), nullptr);
    attribute_id = H5Acreate2(dataset_id, "dx", H5T_NATIVE_DOUBLE, dataspace_id, H5P_DEFAULT, H5P_DEFAULT);
    status = H5Awrite(attribute_id, H5T_NATIVE_DOUBLE, &a_value);


    //close all hdf5 handles
    status = H5Aclose(attribute_id);
    status = H5Dclose(dataset_id);
    status = H5Sclose(dataspace_id);
    status = H5Gclose(group_id);
    status = H5Fclose(file_id); 
}

