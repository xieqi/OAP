#include <arrow/array.h>
#include <arrow/buffer.h>
#include <arrow/ipc/json_simple.h>
#include <arrow/memory_pool.h>
#include <arrow/pretty_print.h>
#include <arrow/record_batch.h>
#include <arrow/status.h>
#include <arrow/type.h>
#include <gandiva/node.h>
#include <gandiva/tree_expr_builder.h>
#include <iostream>
#include <memory>
#include <sstream>
using namespace arrow;

using TreeExprBuilder = gandiva::TreeExprBuilder;
using FunctionNode = gandiva::FunctionNode;

#define ASSERT_NOT_OK(status)                  \
  do {                                         \
    ::arrow::Status __s = (status);            \
    if (!__s.ok()) {                           \
      throw std::runtime_error(__s.message()); \
    }                                          \
  } while (false);

template <typename T>
Status Equals(const T& expected, const T& actual) {
  if (expected.Equals(actual)) {
    return arrow::Status::OK();
  }
  std::stringstream pp_expected;
  std::stringstream pp_actual;
  ::arrow::PrettyPrintOptions options(/*indent=*/2);
  options.window = 50;
  ASSERT_NOT_OK(PrettyPrint(expected, options, &pp_expected));
  ASSERT_NOT_OK(PrettyPrint(actual, options, &pp_actual));
  if (pp_expected.str() == pp_actual.str()) {
    return arrow::Status::OK();
  }
  return Status::Invalid("Expected RecordBatch is ", pp_expected.str(), " with schema ",
                         expected.schema()->ToString(), ", while actual is ",
                         pp_actual.str(), " with schema ", actual.schema()->ToString());
}
