#include <catch.hpp>
#include "darcel/semantics/builtin_scope.hpp"
#include "darcel/syntax/syntax_builders.hpp"
#include "darcel/type_checks/constraints.hpp"

using namespace darcel;

TEST_CASE("test_empty_constraints", "[constraints]") {
  constraints s;
  type_checker t;
  REQUIRE(s.is_satisfied(t));
}

TEST_CASE("test_single_constraint", "[constraints]") {
  auto top_scope = make_builtin_scope();
  auto binding = bind_variable(*top_scope, "x", make_literal(5));
  type_checker t(*top_scope);
  t.check(*binding);
  auto e1 = find_term("x", *top_scope);
  SECTION("Test matching types.") {
    constraints s;
    s.add(*e1, integer_data_type::get_instance());
    REQUIRE(s.is_satisfied(t));
  }
  SECTION("Test mismatched types.") {
    constraints s;
    s.add(*e1, bool_data_type::get_instance());
    REQUIRE(!s.is_satisfied(t));
  }
}

TEST_CASE("test_two_constraints", "[constraints]") {
  auto top_scope = make_builtin_scope();
  auto x_binding = bind_variable(*top_scope, "x", make_literal(5));
  auto y_binding = bind_variable(*top_scope, "y", make_literal(true));
  type_checker t(*top_scope);
  t.check(*x_binding);
  t.check(*y_binding);
  auto e1 = find_term("x", *top_scope);
  auto e2 = find_term("y", *top_scope);
  SECTION("Test satisfying neither c1, c2.") {
    constraints s;
    s.add(*e1, text_data_type::get_instance());
    s.add(*e2, text_data_type::get_instance());
    REQUIRE(!s.is_satisfied(t));
  }
  SECTION("Test satisfying c1 but not c2.") {
    constraints s;
    s.add(*e1, integer_data_type::get_instance());
    s.add(*e2, text_data_type::get_instance());
    REQUIRE(!s.is_satisfied(t));
  }
  SECTION("Test satisfying c2 but not c1.") {
    constraints s;
    s.add(*e1, text_data_type::get_instance());
    s.add(*e2, bool_data_type::get_instance());
    REQUIRE(!s.is_satisfied(t));
  }
  SECTION("Test satisfying both c1, c2.") {
    constraints s;
    s.add(*e1, integer_data_type::get_instance());
    s.add(*e2, bool_data_type::get_instance());
    REQUIRE(s.is_satisfied(t));
  }
}
