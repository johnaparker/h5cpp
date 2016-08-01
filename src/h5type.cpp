#include "h5type.h"
#include <stdexcept>

namespace h5cpp {

hid_t getDtype(dtype datatype) {
    switch(datatype) {
        case dtype::Int: return H5T_NATIVE_INT; break;
        case dtype::UInt: return H5T_NATIVE_UINT; break;
        case dtype::Short: return H5T_NATIVE_SHORT; break;
        case dtype::UShort: return H5T_NATIVE_USHORT; break;
        case dtype::Long: return H5T_NATIVE_LONG; break;
        case dtype::ULong: return H5T_NATIVE_ULONG; break;
        case dtype::LLong: return H5T_NATIVE_LLONG; break;
        case dtype::ULLong: return H5T_NATIVE_ULLONG; break;
        case dtype::Float: return H5T_NATIVE_FLOAT; break;
        case dtype::Double: return H5T_NATIVE_DOUBLE; break;

        case dtype::String: 
            return H5Tcreate(H5T_STRING, H5T_VARIABLE); break;

        default: throw std::invalid_argument("No corresponding H5datatype to your datatype");
    }
}

}

