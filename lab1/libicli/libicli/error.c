#include <libicli/error.h>

/**
 * @brief Convert error code to string representation
 * @param code Error code to convert
 * @return Human-readable string representation of the error
 */
const char* icli_error_to_string(icli_error_code code) {
  switch (code) {
    case ICLI_SUCCESS:
      return "Success";
    case ICLI_ERROR_NULL_POINTER:
      return "Null pointer provided";
    case ICLI_ERROR_INVALID_ARGS:
      return "Invalid arguments";
    case ICLI_ERROR_COMMAND_NOT_FOUND:
      return "Command not found";
    case ICLI_ERROR_MEMORY_ALLOCATION:
      return "Memory allocation failed";
    case ICLI_ERROR_COMMAND_EXISTS:
      return "Command already exists";
    case ICLI_ERROR_INVALID_COMMAND:
      return "Invalid command format";
    case ICLI_ERROR_IO:
      return "I/O error";
    case ICLI_ERROR_UNKNOWN:
      default:
          return "Unknown error";
  }
}