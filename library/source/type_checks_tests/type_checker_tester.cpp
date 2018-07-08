#include <catch.hpp>
#include "darcel/semantics/builtin_scope.hpp"
#include "darcel/syntax/syntax_builders.hpp"
#include "darcel/type_checks/type_checker.hpp"

using namespace darcel;

TEST_CASE("test_bind_variable_type_checker", "[type_checker]") {
  auto top_scope = make_builtin_scope();
  auto binding = bind_variable(*top_scope, "x", make_literal(123));
  type_checker checker(*top_scope);
  REQUIRE_NOTHROW(checker.check(*binding));
  REQUIRE(*checker.get_type(*binding->get_variable()) ==
    integer_data_type());
}

TEST_CASE("test_bind_function_type_checker", "[type_checker]") {
  auto top_scope = make_builtin_scope();
  auto binding = bind_function(*top_scope, "f",
    {{"x", integer_data_type::get_instance()}},
    [&] (scope& s) {
      return find_term("x", s);
    });
  type_checker checker(*top_scope);
  REQUIRE_NOTHROW(checker.check(*binding));
}
