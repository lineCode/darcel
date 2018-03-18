#ifndef DARCEL_CALL_EXPRESSION_HPP
#define DARCEL_CALL_EXPRESSION_HPP
#include <memory>
#include <utility>
#include <vector>
#include "darcel/data_types/function_data_type.hpp"
#include "darcel/syntax/expression.hpp"
#include "darcel/syntax/syntax.hpp"
#include "darcel/syntax/syntax_node_visitor.hpp"

namespace darcel {

  //! Represents applying a callable expression to its parameters,
  //! ie. a function call.
  class call_expression : public expression {
    public:

      //! Constructs a call_expression.
      /*!
        \param l The location of the opening bracket.
        \param callable The expression to call.
        \param parameters The parameters to apply to the <i>callable</i>.
      */
      call_expression(location l, std::unique_ptr<expression> callable,
        std::vector<std::unique_ptr<expression>> parameters);

      //! Returns the callable expression.
      const expression& get_callable() const;

      //! Returns the parameters to apply to the callable.
      const std::vector<std::unique_ptr<expression>>& get_parameters() const;

      const std::shared_ptr<data_type>& get_data_type() const override final;

      void apply(syntax_node_visitor& visitor) const override final;

    private:
      std::unique_ptr<expression> m_callable;
      std::vector<std::unique_ptr<expression>> m_parameters;
  };

  inline call_expression::call_expression(location l,
      std::unique_ptr<expression> callable,
      std::vector<std::unique_ptr<expression>> parameters)
      : expression(std::move(l)),
        m_callable(std::move(callable)),
        m_parameters(std::move(parameters)) {}

  inline const expression& call_expression::get_callable() const {
    return *m_callable;
  }

  inline const std::vector<std::unique_ptr<expression>>&
      call_expression::get_parameters() const {
    return m_parameters;
  }

  inline const std::shared_ptr<data_type>& call_expression::
      get_data_type() const {
    auto function_type = std::static_pointer_cast<function_data_type>(
      m_callable->get_data_type());
    return function_type->get_return_type();
  }

  inline void call_expression::apply(syntax_node_visitor& visitor) const {
    visitor.visit(*this);
  }

  inline void syntax_node_visitor::visit(const call_expression& node) {
    visit(static_cast<const expression&>(node));
  }
}

#endif