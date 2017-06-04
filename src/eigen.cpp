#include "eigen.h" 

namespace h5cpp {

    std::unordered_map<std::type_index, dtype> type_map {
        {typeid(double), dtype::Double},
        {typeid(int), dtype::Int},
    };

}

