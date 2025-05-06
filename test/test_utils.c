#include <criterion/assert.h>  // For cr_assert, cr_assert_eq, etc.
#include <criterion/criterion.h>
#include <criterion/internal/test.h>  // For Test macro
#include <microhttpd.h>

#include "utils.h"

// Dummy callback to capture added headers
struct HeaderCapture {
  int count;
  const char* last_key;
  const char* last_value;
};

static int header_iterator(void* cls, enum MHD_ValueKind kind, const char* key,
                           const char* value) {
  struct HeaderCapture* capture = (struct HeaderCapture*)cls;
  capture->count++;
  capture->last_key = key;
  capture->last_value = value;
  return MHD_YES;
}

Test(utils, add_cors_headers_adds_headers) {
  struct MHD_Response* resp =
      MHD_create_response_from_buffer(0, NULL, MHD_RESPMEM_PERSISTENT);
  cr_assert_not_null(resp);

  add_cors_headers(resp);

  struct HeaderCapture capture = {0};
  MHD_get_response_headers(resp, header_iterator, &capture);

  cr_assert(capture.count > 0, "No headers were added by add_cors_headers");

  MHD_destroy_response(resp);
}
