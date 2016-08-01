#ifndef h5type_GUARD_H
#define h5type_GUARD_H

#include "hdf5.h"

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

}

#endif
