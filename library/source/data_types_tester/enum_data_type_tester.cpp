#include <catch.hpp>
#include "darcel/data_types/enum_data_type.hpp"

using namespace darcel;
using namespace std;

TEST_CASE("test_enum_constructor", "[enum_data_type]") {
  auto e = std::make_shared<enum_data_type>(location::global(), "e",
    vector<enum_data_type::symbol>{{"a", 0}, {"b", 1}});
  REQUIRE(e->get_name() == "e");
  REQUIRE(e->get_symbols().size() == 2);
  REQUIRE(e->get_symbols()[0].m_name == "a");
  REQUIRE(e->get_symbols()[0].m_value == 0);
  REQUIRE(e->get_symbols()[1].m_name == "b");
  REQUIRE(e->get_symbols()[1].m_value == 1);
}

TEST_CASE("test_enum_equality", "[enum_data_type]") {
  auto a = std::make_shared<enum_data_type>(location::global(), "e",
    vector<enum_data_type::symbol>{{"a", 0}, {"b", 1}});
  auto b = std::make_shared<enum_data_type>(location::global(), "e",
    vector<enum_data_type::symbol>{{"a", 0}, {"b", 1}});
  REQUIRE(*a == *a);
  REQUIRE(*b == *b);
  REQUIRE(*a != *b);
  REQUIRE(*b != *a);
}
