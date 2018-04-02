#ifndef DARCEL_FUNCTION_HPP
#define DARCEL_FUNCTION_HPP
#include <vector>
#include "darcel/data_types/data_type_compatibility.hpp"
#include "darcel/data_types/function_data_type.hpp"
#include "darcel/data_types/generic_data_type.hpp"
#include "darcel/semantics/element.hpp"
#include "darcel/semantics/semantics.hpp"
#include "darcel/semantics/variable.hpp"

namespace darcel {

  //! Represents a potentially overloaded, named function.
  class function : public element {
    public:

      //! Constructs a function.
      /*!
        \param initial The initial overload.
      */
      function(std::shared_ptr<variable> initial);

      //! Returns the parent of this function.
      const std::shared_ptr<function>& get_parent() const;

      //! Returns the list of overloads.
      const std::vector<std::shared_ptr<variable>>& get_overloads() const;

      //! Returns the list of instantiations of a generic function.
      const std::vector<std::shared_ptr<variable>>& get_instantiations(
        const std::shared_ptr<variable>& f) const;

      //! Returns an instantiation's definition.
      std::shared_ptr<variable> get_definition(
        const std::shared_ptr<variable>& instance) const;

      //! Adds an overload to this function.
      /*!
        \param overload The overload to add.
        \return true iff the overload was added, otherwise the overload clashes
                with an existing overload.
      */
      bool add(std::shared_ptr<variable> overload);

      //! Adds an instantiation of a generic to this function.
      /*!
        \param definition The overload representing the generic definition.
        \param instance The instantiation of the definition.
        \return true iff the instantiation was added, otherwise the instance
                clashes with an existing overload.
      */
      bool add(std::shared_ptr<variable> definition,
        std::shared_ptr<variable> instance);

      const location& get_location() const override final;

      const std::string& get_name() const override final;

    private:
      std::shared_ptr<function> m_parent;
      std::vector<std::shared_ptr<variable>> m_overloads;
      std::unordered_map<std::shared_ptr<variable>,
        std::vector<std::shared_ptr<variable>>> m_instantiations;
      std::unordered_map<std::shared_ptr<variable>,
        std::shared_ptr<variable>> m_definitions;
  };

  //! Finds the function overload matching a set of parameters.
  /*!
    \param f The overloaded function to search over.
    \param parameters The parameter's to search.
    \return The variable representing the specific function overload.
  */
  inline std::shared_ptr<variable> find_overload(function& f,
      const std::vector<function_data_type::parameter>& parameters) {
    for(auto& overload : f.get_overloads()) {
      auto type = std::static_pointer_cast<function_data_type>(
        overload->get_data_type());
      if(type->get_parameters().size() != parameters.size()) {
        continue;
      }
      auto compatibility = [&] {
        auto compatibility = data_type_compatibility::EQUAL;
        for(std::size_t i = 0; i < parameters.size(); ++i) {
          auto parameter_compatibility = get_compatibility(
            *parameters[i].m_type, *type->get_parameters()[i].m_type);
          if(parameter_compatibility == data_type_compatibility::NONE) {
            return data_type_compatibility::NONE;
          } else if(parameter_compatibility ==
              data_type_compatibility::GENERIC) {
            compatibility = data_type_compatibility::GENERIC;
          }
        }
        return compatibility;
      }();
      if(compatibility == data_type_compatibility::EQUAL) {
        return overload;
      } else if(compatibility == data_type_compatibility::GENERIC) {
        auto t = instantiate(*type, parameters);
        auto instantiation = std::make_shared<variable>(f.get_location(),
          f.get_name(), std::move(t));
        f.add(overload, instantiation);
        return instantiation;
      }
    }
    if(f.get_parent() != nullptr) {
      return find_overload(*f.get_parent(), parameters);
    }
    return nullptr;
  }

  inline function::function(std::shared_ptr<variable> initial) {
    m_overloads.push_back(std::move(initial));
  }

  inline const std::shared_ptr<function>& function::get_parent() const {
    return m_parent;
  }

  inline const std::vector<std::shared_ptr<variable>>&
      function::get_overloads() const {
    return m_overloads;
  }

  inline const std::vector<std::shared_ptr<variable>>&
      function::get_instantiations(const std::shared_ptr<variable>& f) const {
    static std::vector<std::shared_ptr<variable>> NONE;
    auto i = m_instantiations.find(f);
    if(i == m_instantiations.end()) {
      return NONE;
    }
    return i->second;
  }

  inline std::shared_ptr<variable> function::get_definition(
      const std::shared_ptr<variable>& instance) const {
    auto i = m_definitions.find(instance);
    if(i == m_definitions.end()) {
      return nullptr;
    }
    return i->second;
  }

  inline bool function::add(std::shared_ptr<variable> overload) {

    // TODO: Overloads need to be added according to a match order.
    //       Concrete types come first, generics come at the end.
    auto overload_type = std::dynamic_pointer_cast<function_data_type>(
      overload->get_data_type());
    if(overload_type == nullptr) {
      return false;
    }
    auto& overload_parameters = overload_type->get_parameters();
    for(auto& instance : m_overloads) {
      auto instance_type = std::static_pointer_cast<function_data_type>(
        instance->get_data_type());
      auto& instance_parameters = instance_type->get_parameters();
      if(overload_parameters.size() != instance_parameters.size()) {
        continue;
      }
      auto parameters_match = true;
      for(std::size_t i = 0; i < overload_parameters.size(); ++i) {
        if(*overload_parameters[i].m_type != *instance_parameters[i].m_type) {
          parameters_match = false;
          break;
        }
      }
      if(parameters_match) {
        return false;
      }
    }
    m_overloads.insert(m_overloads.begin(), std::move(overload));
    return true;
  }

  inline bool function::add(std::shared_ptr<variable> definition,
      std::shared_ptr<variable> instance) {
    if(!add(instance)) {
      return false;
    }
    m_instantiations[definition].push_back(instance);
    m_definitions.insert(std::make_pair(std::move(instance),
      std::move(definition)));
    return true;
  }

  inline const location& function::get_location() const {
    return m_overloads.front()->get_location();
  }

  inline const std::string& function::get_name() const {
    return m_overloads.front()->get_name();
  }
}

#endif
