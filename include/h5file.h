#ifndef h5file_GUARD_H
#define h5file_GUARD_H

#include "hdf5.h"
#include <memory>
#include "boost/multi_array.hpp"
#include <string>
#include <vector>
#include <map>

#include "h5group.h"
#include "h5dset.h"
#include "h5attr.h"

using group_iter = std::map<std::string, std::unique_ptr<h5group>>::iterator;
using group_pair = std::pair<std::string, std::unique_ptr<h5group>>;


class h5file {
public:
    h5file(std::string name, unsigned flags = H5F_ACC_TRUNC);

    group_iter find_group(std::string name);
    dset_iter find_dset(std::string name);

    void create_group(std::string name);
    void create_dataset(std::string name, std::string where, 
            hid_t datatype, std::vector<hsize_t> dims);
    void create_attribute(std::string name, std::string where, 
            hid_t datatype, std::vector<hsize_t> dims);

    void write_data(std::string name, std::string where,
            const void* data, hid_t datatype);

    ~h5file();


private:
    std::string filename;
    hid_t file_id;
    herr_t status;

    std::map<std::string, std::unique_ptr<h5group>> groups;
    std::map<std::string, std::unique_ptr<h5dset>> dsets;
    std::map<std::string, std::unique_ptr<h5attr>> attrs;
};






#endif
