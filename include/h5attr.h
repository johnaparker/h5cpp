#ifndef h5attr_GUARD_H
#define h5attr_GUARD_H

#include "hdf5.h"
#include <memory>
#include <string>
#include <map>
#include <vector>

#include "h5dspace.h"

namespace h5cpp {

enum class dtype {
    Int, UInt,
    Short, UShort,
    Long, ULong,
    LLong, ULLong,
    Float, Double,
    String,
};

hid_t getDtype(dtype datatype);

class h5attr {
public:
    h5attr(std::string name, hid_t where, dtype datatype_, dataspace dspace);
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

}

#endif
