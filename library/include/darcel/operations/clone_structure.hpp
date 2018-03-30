#ifndef DARCEL_CLONE_STRUCTURE_HPP
#define DARCEL_CLONE_STRUCTURE_HPP
#include "darcel/syntax/syntax_nodes.hpp"
#include "darcel/syntax/syntax_node_visitor.hpp"

namespace darcel {

  //! Copies a node structurally, that is, the AST nodes are cloned but the
  //! elements they refer to are not.
  /*!
    \param node The node to clone.
    \return A structural clone of the node.
  */
  template<typename T>
  inline std::unique_ptr<T> clone_structure(const T& node) {
    struct clone_visitor : syntax_node_visitor {
      std::unique_ptr<syntax_node> m_clone;

      std::unique_ptr<T> operator ()(const T& node) {
        node.apply(*this);
        return std::unique_ptr<T>(static_cast<T*>(m_clone.release()));
      }

      void visit(const bind_function_statement& node) override final {
        m_clone = std::make_unique<bind_function_statement>(node.get_location(),
          node.get_function(), node.get_overload(), node.get_parameters(),
          clone_structure(node.get_expression()));
      }

      void visit(const bind_variable_statement& node) override final {
        m_clone = std::make_unique<bind_variable_statement>(node.get_location(),
          node.get_variable(), clone_structure(node.get_expression()));
      }

      void visit(const call_expression& node) override final {
        m_clone = std::make_unique<call_expression>(node.get_location(),
          clone_structure(node.get_callable()),
          clone_structure(node.get_parameters()));
      }

      void visit(const function_expression& node) override final {
        m_clone = std::make_unique<function_expression>(node.get_location(),
          node.get_function());
      }

      void visit(const literal_expression& node) override final {
        m_clone = std::make_unique<literal_expression>(node.get_location(),
          node.get_literal());
      }

      void visit(const variable_expression& node) override final {
        m_clone = std::make_unique<variable_expression>(node.get_location(),
          node.get_variable());
      }
    };
    return clone_visitor()(node);
  }

  template<typename T>
  inline std::vector<std::unique_ptr<T>> clone_structure(
      const std::vector<std::unique_ptr<T>>& nodes) {
    std::vector<std::unique_ptr<T>> clones;
    for(auto& node : nodes) {
      clones.push_back(clone_structure(*node));
    }
    return clones;
  }
}

#endif
