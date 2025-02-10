#include "tpl/tpl.h"
#include <cstring>
#include <memory>

// - Internal structure for the handle
struct tpl_handle_t {
  int dummy_value;
};

// - Create function
tpl_handle_t *tpl_Create() { return new tpl_handle_t{0}; }

// - Destroy function
void tpl_Destroy(tpl_handle_t *handle) { delete handle; }

// - Process function
tpl_status_t tpl_Process(tpl_handle_t *handle, const tpl_data_t *data) {
  tpl_status_t status{0, "Success"};

  if (!handle || !data) {
    status.error_code = -1;
    std::strcpy(status.message, "Invalid parameters");
    return status;
  }

  try {
    if (!handle || !data) {
      throw std::invalid_argument("Invalid parameters");
    }

    handle->dummy_value += data->field1;

  } catch (const std::invalid_argument &e) {
    status.error_code = -1;
    std::strncpy(status.message, e.what(), sizeof(status.message) - 1);
  } catch (const std::exception &e) {
    status.error_code = -2;
    std::strncpy(status.message, e.what(), sizeof(status.message) - 1);
  } catch (...) {
    status.error_code = -99;
    std::strncpy(status.message, "Unknown error", sizeof(status.message) - 1);
  }

  return status;
}
