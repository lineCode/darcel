#ifndef DARCEL_BUILTIN_TRANSLATOR_HPP
#define DARCEL_BUILTIN_TRANSLATOR_HPP
#include <iostream>
#include "darcel/reactors/chain_reactor.hpp"
#include "darcel/reactors/count_reactor.hpp"
#include "darcel/reactors/enum_to_string_reactor.hpp"
#include "darcel/reactors/first_reactor.hpp"
#include "darcel/reactors/fold_reactor.hpp"
#include "darcel/reactors/last_reactor.hpp"
#include "darcel/reactors/ostream_reactor.hpp"
#include "darcel/reactors/reactor_translator.hpp"
#include "darcel/reactors/tally_reactor.hpp"
#include "darcel/semantics/scope.hpp"

namespace darcel {

  //! Adds definitions for the builtin add functions.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the function in.
  */
  inline void translate_add(ReactorTranslator& translator, const Scope& s) {
    auto f = s.find<Function>("add");
    if(f != nullptr) {
      for(auto& overload : s.get_definitions(*f)) {
        auto signature = std::static_pointer_cast<FunctionDataType>(
          overload->get_type());
        if(signature->get_parameters().size() == 2) {
          if(*signature->get_parameters()[0].m_type == IntegerDataType() &&
              *signature->get_parameters()[1].m_type == IntegerDataType()) {
            translator.add(overload, make_add_reactor_builder<int, int>());
          } else if(
              *signature->get_parameters()[0].m_type == FloatDataType() &&
              *signature->get_parameters()[1].m_type == FloatDataType()) {
            translator.add(overload,
              make_add_reactor_builder<double, double>());
          } else if(
              *signature->get_parameters()[0].m_type == TextDataType() &&
              *signature->get_parameters()[1].m_type == TextDataType()) {
            translator.add(overload,
              make_add_reactor_builder<std::string, std::string>());
          }
        }
      }
    }
  }

  //! Adds definitions for the builtin chain functions.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the function in.
  */
  inline void translate_chain(ReactorTranslator& translator, const Scope& s) {
    struct Builder {
      std::unique_ptr<ReactorBuilder> operator ()(
          const std::shared_ptr<FunctionDataType>& t) const {
        if(*t->get_parameters()[0].m_type == BoolDataType()) {
          return make_chain_reactor_builder<bool>();
        } else if(*t->get_parameters()[0].m_type == FloatDataType()) {
          return make_chain_reactor_builder<double>();
        } else if(*t->get_parameters()[0].m_type == IntegerDataType()) {
          return make_chain_reactor_builder<int>();
        } else if(*t->get_parameters()[0].m_type == TextDataType()) {
          return make_chain_reactor_builder<std::string>();
        } else if(auto e = std::dynamic_pointer_cast<EnumDataType>(
            t->get_parameters()[0].m_type)) {
          return make_chain_reactor_builder<int>();
        }

        // TODO: Chain generic values
        return nullptr;
      }
    };
    auto f = s.find<Function>("chain");
    if(f != nullptr) {
      translator.add(s.get_definitions(*f).front(), Builder());
    }
  }

  //! Adds definitions for the builtin count function.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the function in.
  */
  inline void translate_count(ReactorTranslator& translator, const Scope& s) {
    auto f = s.find<Function>("count");
    translator.add(s.get_definitions(*f).front(), make_count_builder());
  }

  //! Adds definitions for the builtin divide functions.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the function in.
  */
  inline void translate_divide(ReactorTranslator& translator,
      const Scope& s) {
    auto f = s.find<Function>("divide");
    if(f != nullptr) {
      for(auto& overload : s.get_definitions(*f)) {
        auto signature = std::static_pointer_cast<FunctionDataType>(
          overload->get_type());
        if(signature->get_parameters().size() == 2) {
          if(*signature->get_parameters()[0].m_type == IntegerDataType() &&
              *signature->get_parameters()[1].m_type == IntegerDataType()) {
            translator.add(overload, make_divide_reactor_builder<int, int>());
          } else if(
              *signature->get_parameters()[0].m_type == FloatDataType() &&
              *signature->get_parameters()[1].m_type == FloatDataType()) {
            translator.add(overload,
              make_divide_reactor_builder<double, double>());
          }
        }
      }
    }
  }

  //! Adds definitions for the builtin first functions.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the function in.
  */
  inline void translate_first(ReactorTranslator& translator, const Scope& s) {
    struct Builder {
      std::unique_ptr<ReactorBuilder> operator ()(
          const std::shared_ptr<FunctionDataType>& t) const {
        if(*t->get_parameters()[0].m_type == BoolDataType()) {
          return make_first_reactor_builder<bool>();
        } else if(*t->get_parameters()[0].m_type == FloatDataType()) {
          return make_first_reactor_builder<float>();
        } else if(*t->get_parameters()[0].m_type == IntegerDataType()) {
          return make_first_reactor_builder<int>();
        } else if(*t->get_parameters()[0].m_type == TextDataType()) {
          return make_first_reactor_builder<std::string>();
        } else if(auto e = std::dynamic_pointer_cast<EnumDataType>(
            t->get_parameters()[0].m_type)) {
          return make_first_reactor_builder<int>();
        }

        // TODO: Handle generic values.
        return nullptr;
      }
    };
    auto f = s.find<Function>("first");
    if(f != nullptr) {
      translator.add(s.get_definitions(*f).front(), Builder());
    }
  }

  //! Adds definitions for the builtin fold functions.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the function in.
  */
  inline void translate_fold(ReactorTranslator& translator, const Scope& s) {
    struct Builder {
      std::unique_ptr<ReactorBuilder> operator ()(
          const std::shared_ptr<FunctionDataType>& t) const {
        if(*t->get_parameters()[1].m_type == BoolDataType()) {
          return make_fold_reactor_builder<bool>();
        } else if(*t->get_parameters()[1].m_type == FloatDataType()) {
          return make_fold_reactor_builder<double>();
        } else if(*t->get_parameters()[1].m_type == IntegerDataType()) {
          return make_fold_reactor_builder<int>();
        } else if(*t->get_parameters()[1].m_type == TextDataType()) {
          return make_fold_reactor_builder<std::string>();
        } else if(auto e = std::dynamic_pointer_cast<EnumDataType>(
            t->get_parameters()[0].m_type)) {
          return make_fold_reactor_builder<int>();
        }

        // TODO: Fold generic values
        return nullptr;
      }
    };
    auto f = s.find<Function>("fold");
    if(f != nullptr) {
      translator.add(s.get_definitions(*f).front(), Builder());
    }
  }

  //! Adds definitions for the builtin last functions.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the function in.
  */
  inline void translate_last(ReactorTranslator& translator, const Scope& s) {
    struct Builder {
      std::unique_ptr<ReactorBuilder> operator ()(
          const std::shared_ptr<FunctionDataType>& t) const {
        if(*t->get_parameters()[0].m_type == BoolDataType()) {
          return make_last_reactor_builder<bool>();
        } else if(*t->get_parameters()[0].m_type == FloatDataType()) {
          return make_last_reactor_builder<float>();
        } else if(*t->get_parameters()[0].m_type == IntegerDataType()) {
          return make_last_reactor_builder<int>();
        } else if(*t->get_parameters()[0].m_type == TextDataType()) {
          return make_last_reactor_builder<std::string>();
        } else if(auto e = std::dynamic_pointer_cast<EnumDataType>(
            t->get_parameters()[0].m_type)) {
          return make_last_reactor_builder<int>();
        }

        // TODO: Handle generic values.
        return nullptr;
      }
    };
    auto f = s.find<Function>("last");
    if(f != nullptr) {
      translator.add(s.get_definitions(*f).front(), Builder());
    }
  }

  //! Adds definitions for the builtin multiply functions.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the function in.
  */
  inline void translate_multiply(ReactorTranslator& translator,
      const Scope& s) {
    auto f = s.find<Function>("multiply");
    if(f != nullptr) {
      for(auto& overload : s.get_definitions(*f)) {
        auto signature = std::static_pointer_cast<FunctionDataType>(
          overload->get_type());
        if(signature->get_parameters().size() == 2) {
          if(*signature->get_parameters()[0].m_type == IntegerDataType() &&
              *signature->get_parameters()[1].m_type == IntegerDataType()) {
            translator.add(overload, make_multiply_reactor_builder<int, int>());
          } else if(
              *signature->get_parameters()[0].m_type == FloatDataType() &&
              *signature->get_parameters()[1].m_type == FloatDataType()) {
            translator.add(overload,
              make_multiply_reactor_builder<double, double>());
          }
        }
      }
    }
  }

  //! Adds definitions for the builtin print functions.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the function in.
  */
  inline void translate_print(ReactorTranslator& translator, const Scope& s) {
    struct Builder {
      std::unique_ptr<ReactorBuilder> operator ()(
          const std::shared_ptr<FunctionDataType>& t) const {
        if(*t->get_parameters()[0].m_type == BoolDataType()) {
          return make_ostream_reactor_builder<bool>(std::cout);
        } else if(*t->get_parameters()[0].m_type == FloatDataType()) {
          return make_ostream_reactor_builder<double>(std::cout);
        } else if(*t->get_parameters()[0].m_type == IntegerDataType()) {
          return make_ostream_reactor_builder<int>(std::cout);
        } else if(*t->get_parameters()[0].m_type == TextDataType()) {
          return make_ostream_reactor_builder<std::string>(std::cout);
        } else if(auto e = std::dynamic_pointer_cast<EnumDataType>(
            t->get_parameters()[0].m_type)) {
          return std::make_unique<FunctionReactorBuilder>(
            [=] (auto& parameters, auto& t) {
              return make_ostream_reactor(std::cout,
                std::static_pointer_cast<Reactor<std::string>>(
                make_enum_to_string_reactor(e,
                std::static_pointer_cast<Reactor<int>>(
                parameters.front()->build(t)))));
            });
        }

        // TODO: Print generic values
        return nullptr;
      }
    };
    auto f = s.find<Function>("print");
    if(f != nullptr) {
      translator.add(s.get_definitions(*f).front(), Builder());
    }
  }

  //! Adds definitions for the builtin subtract functions.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the function in.
  */
  inline void translate_subtract(ReactorTranslator& translator,
      const Scope& s) {
    auto f = s.find<Function>("subtract");
    if(f != nullptr) {
      for(auto& overload : s.get_definitions(*f)) {
        auto signature = std::static_pointer_cast<FunctionDataType>(
          overload->get_type());
        if(signature->get_parameters().size() == 2) {
          if(*signature->get_parameters()[0].m_type == IntegerDataType() &&
              *signature->get_parameters()[1].m_type == IntegerDataType()) {
            translator.add(overload, make_subtract_reactor_builder<int, int>());
          } else if(
              *signature->get_parameters()[0].m_type == FloatDataType() &&
              *signature->get_parameters()[1].m_type == FloatDataType()) {
            translator.add(overload,
              make_subtract_reactor_builder<double, double>());
          }
        }
      }
    }
  }

  //! Adds definitions for the builtin tally functions.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the function in.
  */
  inline void translate_tally(ReactorTranslator& translator, const Scope& s) {
    struct Builder {
      std::unique_ptr<ReactorBuilder> operator ()(
          const std::shared_ptr<FunctionDataType>& t) const {
        return make_tally_reactor_builder();
      }
    };
    auto f = s.find<Function>("tally");
    if(f != nullptr) {
      translator.add(s.get_definitions(*f).front(), Builder());
    }
  }

  //! Adds definitions for all the builtin functions.
  /*!
    \param translator The translator to add the definitions to.
    \param s The scope to find the functions in.
  */
  inline void translate_builtins(ReactorTranslator& translator,
      const Scope& s) {
    translate_add(translator, s);
    translate_chain(translator, s);
    translate_count(translator, s);
    translate_divide(translator, s);
    translate_first(translator, s);
    translate_fold(translator, s);
    translate_last(translator, s);
    translate_multiply(translator, s);
    translate_print(translator, s);
    translate_subtract(translator, s);
    translate_tally(translator, s);
  }
}

#endif
