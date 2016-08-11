#ifndef h5attr_GUARD_H
#define h5attr_GUARD_H

#include "hdf5.h"
#include "h5type.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

#include "h5dspace.h"

namespace h5cpp {

class h5attr {
public:
    h5attr(std::string name, hid_t where, dtype datatype_, dspace dataspace);
    explicit h5attr(hid_t attr_id);

    void write(const void* data);

    void read(void* dest);

    const dspace get_dspace();

    ~h5attr();

private:
    std::string name;

    hid_t attr_id;
    hid_t dspace_id;
    dspace dataspace;
    hid_t datatype;
    herr_t status;
};

}

#endif
