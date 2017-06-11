#include "eigen.h" 

namespace h5cpp {

    std::unordered_map<std::type_index, dtype> type_map {
        {typeid(double), dtype::Double},
        {typeid(int), dtype::Int},
    };

    h5cpp::dspace get_eigen_dspace(std::vector<hsize_t> dims, append a) {
        h5cpp::dspace dspace;

        if (static_cast<bool>(a)) {
            dims.push_back(1);
            auto max_dims = dims;
            max_dims.back() = inf;
            auto chunk_dims = dims;
            dspace = h5cpp::dspace(dims, max_dims, chunk_dims);
        }
        else
            dspace = h5cpp::dspace(dims);
        return dspace;
    } 

}
