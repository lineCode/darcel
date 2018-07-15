#include <catch.hpp>
#include "darcel/syntax/syntax_builders.hpp"
#include "darcel/type_checks/constraints.hpp"

using namespace darcel;

TEST_CASE("test_empty_constraints", "[constraints]") {
  constraints s;
  type_map t;
  REQUIRE(s.is_satisfied(t, scope()));
}

TEST_CASE("test_single_constraint", "[constraints]") {
  scope s;
  auto binding = bind_variable(s, "x", make_literal(5));
  type_map t;
  t.add(*binding->get_variable(), std::make_shared<integer_data_type>());
  auto e1 = find_term("x", s);
  SECTION("Test matching types.") {
    constraints c;
    c.add(*e1, integer_data_type::get_instance());
    REQUIRE(c.is_satisfied(t, s));
  }
  SECTION("Test mismatched types.") {
    constraints c;
    c.add(*e1, bool_data_type::get_instance());
    REQUIRE(!c.is_satisfied(t, s));
  }
}

TEST_CASE("test_two_constraints", "[constraints]") {
  scope s;
  auto x_binding = bind_variable(s, "x", make_literal(5));
  auto y_binding = bind_variable(s, "y", make_literal(true));
  type_map t;
  t.add(*x_binding->get_variable(), std::make_shared<integer_data_type>());
  t.add(*y_binding->get_variable(), std::make_shared<bool_data_type>());
  auto e1 = find_term("x", s);
  auto e2 = find_term("y", s);
  SECTION("Test satisfying neither c1, c2.") {
    constraints c;
    c.add(*e1, text_data_type::get_instance());
    c.add(*e2, text_data_type::get_instance());
    REQUIRE(!c.is_satisfied(t, s));
  }
  SECTION("Test satisfying c1 but not c2.") {
    constraints c;
    c.add(*e1, integer_data_type::get_instance());
    c.add(*e2, text_data_type::get_instance());
    REQUIRE(!c.is_satisfied(t, s));
  }
  SECTION("Test satisfying c2 but not c1.") {
    constraints c;
    c.add(*e1, text_data_type::get_instance());
    c.add(*e2, bool_data_type::get_instance());
    REQUIRE(!c.is_satisfied(t, s));
  }
  SECTION("Test satisfying both c1, c2.") {
    constraints c;
    c.add(*e1, integer_data_type::get_instance());
    c.add(*e2, bool_data_type::get_instance());
    REQUIRE(c.is_satisfied(t, s));
  }
}
