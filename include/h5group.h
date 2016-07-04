#ifndef h5group_GUARD_H
#define h5group_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

#include "h5dset.h"
#include "h5attr.h"


class h5group {
public:
    h5group(std::string name, hid_t where);
    explicit h5group(hid_t group_id);

    std::unique_ptr<h5dset> create_dataset(std::string name, hid_t datatype, h5dspace dspace);

    std::unique_ptr<h5attr> create_attribute(std::string name, hid_t datatype, std::vector<hsize_t> dims);

    ~h5group();

private:
    std::string name;
    hid_t group_id;
    herr_t status;
};

#endif
