#ifndef h5type_GUARD_H
#define h5type_GUARD_H

#include "hdf5.h"
#include <vector>
#include <string>

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

hid_t dtypeArray(dtype datatype, std::vector<hsize_t> dims);

class dtypeCompound {
public:
    dtypeCompound(size_t memsize);
    void insert(std::string name, dtype datatype);
    void insert(std::string name, hid_t h5dtype);

    hid_t memType();
    hid_t fileType();

private:
    hid_t memtype;
    herr_t status;
    size_t memDisplace;
    size_t fileSize;

    std::vector<std::string> names;
    std::vector<hid_t> types;
    std::vector<hsize_t> fileDisplacements;
};

}

#endif
