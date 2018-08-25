#ifndef INCLUDED_U5E_UTF8_STRING_VIEW_GRAPHEME
#define INCLUDED_U5E_UTF8_STRING_VIEW_GRAPHEME

#include <u5e/basic_grapheme.hpp>
#include <u5e/utf8_string_view.hpp>

namespace u5e {
  /**
   * \class u5e::utf8_string_view_grapheme
   * \brief Typedef: basic_grapheme of utf8_string_view
   *
   * Although this is a typedef, it shows up in doxygen as a class for
   * better discoverability.
   *
   * \typedef u5e::utf8_string_view_grapheme
   * \brief A basic_grapheme of utf8_string_view
   */
  typedef basic_grapheme<utf8_string_view> utf8_string_view_grapheme;
};

#endif
