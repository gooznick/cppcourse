#include <iostream>
#include "tpl/tpl.h"

int main() {
    tpl_handle_t* handle = tpl_Create();
    if (!handle) {
        std::cerr << "Failed to create handle" << std::endl;
        return 1;
    }

    tpl_data_t data = {10, 20, 30};
    tpl_status_t status = tpl_Process(handle, &data);
    
    std::cout << "Status: " << status.message << " (Code: " << status.error_code << ")" << std::endl;

    tpl_Destroy(handle);
    return 0;
}
