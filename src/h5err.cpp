#include "h5err.h"

error_lock::error_lock() {
    H5Eget_auto(H5E_DEFAULT, &func, &client_data);
    H5Eset_auto(H5E_DEFAULT, nullptr, nullptr);
}

error_lock::~error_lock() {
    release();
}

void error_lock::release() {
    H5Eset_auto(H5E_DEFAULT, func, client_data);
}

