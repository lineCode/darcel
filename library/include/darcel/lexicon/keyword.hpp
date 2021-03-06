#ifndef DARCEL_KEYWORD_HPP
#define DARCEL_KEYWORD_HPP
#include <algorithm>
#include <cctype>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string_view>
#include "darcel/lexicon/lexical_iterator.hpp"
#include "darcel/lexicon/lexicon.hpp"

namespace darcel {

  //! Stores a keyword token.
  class Keyword {
    public:

      //! The list of keywords.
      enum class Word {

        //! enum
        ENUM,

        //! let
        LET
      };

      //! Constructs a keyword.
      /*!
        \param word The word to represent.
      */
      Keyword(Word word);

      //! Returns the word represented.
      Word get_word() const;

    private:
      Word m_word;
  };

  //! Checks if a string prefix matches a string literal.
  /*!
    \param lhs An iterator to the first character to match.
    \param rhs The string prefix to match against.
    \return <code>true</code> iff the iterator contains <i>rhs</i> as a prefix.
  */
  template<std::size_t N>
  bool prefix_match(LexicalIterator& lhs, const char (&rhs)[N]) {
    if(lhs.get_size_remaining() >= N &&
        std::equal(&*lhs, &*lhs + (N - 1), rhs) &&
        !std::isalnum(*(lhs + (N - 1))) && *(lhs + (N - 1)) != '_') {
      lhs += N - 1;
      return true;
    }
    return false;
  }

  //! Parses a keyword.
  /*!
    \param cursor An iterator to the first character to parse, this iterator
           will be adjusted to one past the last character that was parsed.
    \return The keyword that was parsed.
  */
  inline std::optional<Keyword> parse_keyword(LexicalIterator& cursor) {
    if(prefix_match(cursor, "enum")) {
      return Keyword::Word::ENUM;
    } else if(prefix_match(cursor, "let")) {
      return Keyword::Word::LET;
    }
    return std::nullopt;
  }

  //! Parses a keyword from a string.
  /*!
    \param source The string to parse.
    \return The keyword that was parsed.
  */
  inline auto parse_keyword(const std::string_view& source) {
    return darcel::parse_keyword(
      LexicalIterator(source.data(), source.size() + 1));
  }

  inline std::ostream& operator <<(std::ostream& out, const Keyword& value) {
    switch(value.get_word()) {
      case Keyword::Word::ENUM:
        return out << "enum";
      case Keyword::Word::LET:
        return out << "let";
      default:
        throw std::runtime_error("Invalid keyword.");
    }
  }

  inline bool operator ==(const Keyword& lhs, const Keyword& rhs) {
    return lhs.get_word() == rhs.get_word();
  }

  inline bool operator !=(const Keyword& lhs, const Keyword& rhs) {
    return !(lhs == rhs);
  }

  inline Keyword::Keyword(Word word)
      : m_word(word) {}

  inline Keyword::Word Keyword::get_word() const {
    return m_word;
  }
}

#endif
