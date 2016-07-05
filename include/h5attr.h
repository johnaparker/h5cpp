#ifndef h5attr_GUARD_H
#define h5attr_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

#include "h5dspace.h"


class h5attr {
public:
    h5attr(std::string name, hid_t where, hid_t datatype, dataspace dspace);
    explicit h5attr(hid_t attr_id);

    void write(const void* data);

    ~h5attr();

private:
    std::string name;

    hid_t attr_id;
    hid_t dspace_id;
    dataspace dspace;
    hid_t datatype;
    herr_t status;
};

#endif
