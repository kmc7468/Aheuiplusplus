#ifndef INCLUDED_U5E_UTF32NE_STRING_GRAPHEME_ITERATOR
#define INCLUDED_U5E_UTF32NE_STRING_GRAPHEME_ITERATOR

#include <u5e/utf32ne_string_grapheme.hpp>
#include <u5e/basic_grapheme_iterator.hpp>

namespace u5e {
  /**
   * \class u5e::utf32ne_string_grapheme_iterator
   * \brief Typedef: basic_grapheme_iterator of utf32ne_string
   *
   * Although this is a typedef, it shows up in doxygen as a class for
   * better discoverability.
   *
   * \typedef u5e::utf32ne_string_grapheme_iterator
   * \brief A basic_grapheme_iterator of utf32ne_string
   */
  typedef basic_grapheme_iterator<utf32ne_string>
  utf32ne_string_grapheme_iterator;
};

#endif
