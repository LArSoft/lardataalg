/**
 * @file   lardataalg/Utilities/MultipleChoiceSelection.h
 * @brief  Helper to select an string option among a set of allowed choices.
 * @author Gianluca Petrillo (petrillo@slac.stanford.edu)
 * @date   December 13, 2019
 *
 */

#ifndef LARDATAALG_UTILITIES_MULTIPLECHOICESELECTION_H
#define LARDATAALG_UTILITIES_MULTIPLECHOICESELECTION_H

// C/C++ standard library
#include <algorithm> // std::equal(), std::lexicographical_compare()
#include <cassert>
#include <cctype>    // std::tolower()
#include <cstddef>   // std::size_t
#include <exception> // std::exception
#include <limits>
#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <type_traits> // std::conjunction_v, std::is_convertible...
#include <vector>

// -----------------------------------------------------------------------------
namespace util::details {

  // ---------------------------------------------------------------------------
  template <typename... Strings>
  constexpr auto AllConvertibleToStrings_v =
    std::conjunction_v<std::is_convertible<Strings, std::string>...>;

  // ---------------------------------------------------------------------------
  struct CaseInsensitiveComparer {

    /// Returns whether strings `a` and `b` are equal.
    static bool equal(std::string const& a, std::string const& b);

    /// Returns whether `a` is lexicographically smaller than `b`.
    static bool less(std::string const& a, std::string const& b);

  private:
    static bool cmp_lower(unsigned char a, unsigned char b);
    static bool eq_lower(unsigned char a, unsigned char b);

  }; // struct CaseInsensitiveComparer

  // ---------------------------------------------------------------------------
  template <typename Comparer>
  struct SorterFrom {
    bool operator()(std::string const& a, std::string const& b) const;
  }; // struct SorterFrom<>

  // ---------------------------------------------------------------------------
  template <typename Value, typename = void>
  struct ValueToString;

  // ---------------------------------------------------------------------------
  /**
   * @brief Class representing one of the available options to be selected.
   *
   * An option has a value (of type `Choices_t`) and a name as a string.
   * It may also have aliases. The identity of the option is defined by the
   * value: two option objects with the same value represent the same option.
   *
   * Matching a label is encoded in this class: the option matches a label if
   * its name or any of its aliases matches the proposed label in a
   * case-insensitive comparison.
   */
  template <typename Choices>
  class MultipleChoiceSelectionOption_t {

    using Comparer_t = CaseInsensitiveComparer;

  public:
    using Choices_t = Choices;
    using Option_t = MultipleChoiceSelectionOption_t<Choices_t>;

    /// Constructor: assigns value, name and aliases.
    template <typename... Aliases>
    MultipleChoiceSelectionOption_t(Choices_t value, std::string name, Aliases... aliases);

    /// Adds aliases.
    template <typename... Aliases>
    std::enable_if_t<AllConvertibleToStrings_v<Aliases...>, Option_t&> addAlias(
      std::string alias,
      Aliases... moreAliases);

    /// Returns whether this option matches the specified label (name or alias).
    bool match(std::string const& label) const;

    /// Returns a copy of the value of the option.
    Choices_t value() const { return fValue; }

    /// Returns the name of the option (i.e. the main label).
    std::string name() const { return labels().front(); }

    /// Returns an iterable object with all the labels of the option.
    std::vector<std::string> const& labels() const { return fLabels; }

    /// Returns a string representing this option.
    operator std::string() const { return name(); }

    /// Returns the value of this option.
    operator Choices_t() const { return value(); }

    /// Returns a string representing the value of the option.
    ///
    /// That will be the `name()` of the option if the value is not convertible
    /// to a string.
    std::string value_as_string() const;

    /// Returns a string represent the value of the option, or `defValue`.
    std::string value_as_string(std::string const& defValue) const;

    /// Returns in a string the name and all the aliases.
    std::string dump() const;

    /// Returns whether the two options are the same (same value and name).
    bool operator==(Option_t const& option) const
    {
      return (value() == option.value()) && equal(name(), option.name());
    }

    /// Returns whether the two options are not the same.
    bool operator!=(Option_t const& option) const
    {
      return (value() != option.value()) || !equal(name(), option.name());
    }

    /// Converts a value of type `Choices_t` into a string, if possible.
    static std::optional<std::string> value_as_string(Choices_t value);

    /// Converts a value of type `Choices_t` into a string, if possible.
    static std::string value_as_string(Choices_t value, std::string const& defValue);

  private:
    Choices_t fValue;                 ///< The value associated to the option.
    std::vector<std::string> fLabels; ///< All the labels.

    static bool equal(std::string const& a, std::string const& b)
    {
      return Comparer_t::equal(a, b);
    }

  }; // MultipleChoiceSelectionOption_t

  // ---------------------------------------------------------------------------

} // namespace util::details

// -----------------------------------------------------------------------------
namespace util {
  class MultipleChoiceSelectionBase;
}

/**
 * @brief Base class of `util::MultipleChoiceSelection` with basics independent
 *        of the option type.
 */
class util::MultipleChoiceSelectionBase {

public:
  // --- BEGIN -- Exceptions ---------------------------------------------------
  /// @name Exceptions
  /// @{

  struct Exception : std::exception {
    Exception(std::string const& s) : s(s) {}
    virtual const char* what() const noexcept override { return s.c_str(); }

    std::string s;
  }; // Exception

  /// Request for unknown option.
  struct UnknownOptionError : Exception {
    UnknownOptionError() : Exception("") {}
    UnknownOptionError(std::string const& label) : Exception(label) {}

    std::string const& label() const { return s; }
  }; // UnknownOptionError

  /// Adding an option that already exists.
  struct OptionAlreadyExistsError : Exception {
    OptionAlreadyExistsError() : Exception("") {}
    OptionAlreadyExistsError(std::string const& label) : Exception(label) {}

    std::string const& label() const { return s; }
  }; // OptionAlreadyExistsError

  /// @}
  // --- END -- Exceptions -----------------------------------------------------

}; // class MultipleChoiceSelectionBase

// -----------------------------------------------------------------------------
namespace util {
  template <typename>
  class MultipleChoiceSelection;
}
/**
 * @brief Helper to select one among multiple choices via strings.
 * @tparam Choices type describing the choices
 *
 *
 * @note If the type to describe the choice is a string, its value still need
 *       to be explicitly added as an option label.
 *
 */
template <typename Choices>
class util::MultipleChoiceSelection : public util::MultipleChoiceSelectionBase {

  using Selector_t = MultipleChoiceSelection<Choices>; ///< This type.

public:
  using Choices_t = Choices; ///< Type of the underlying choice.

  using Option_t = details::MultipleChoiceSelectionOption_t<Choices_t>;

  // --- BEGIN -- Constructors -------------------------------------------------

  /// Default constructor: flags are set to `DefaultFlags`, options may be added
  /// later.
  MultipleChoiceSelection() = default;

  /**
   * @brief Constructor: adds the specified options.
   * @tparam Options a number of `Option_t` objects
   * @param options a list of options to add to the selector
   *
   * All specified options are added as with `addOption()`.
   */
  template <typename... Options>
  MultipleChoiceSelection(std::initializer_list<Option_t> options);

  // --- END -- Constructors ---------------------------------------------------

  // --- BEGIN -- Option management --------------------------------------------
  /// @name Option management
  /// @{

  /// Returns the number of available options.
  std::size_t size() const;

  /// Returns whether there is no available option.
  bool empty() const;

  /**
   * @brief Adds a new option to the selector.
   * @tparam Aliases any number of `std::string`
   * @param value the value of the new option
   * @param label the first label to associate to the option
   * @param aliases additional aliases for this option
   * @return the newly created option
   * @throw OptionAlreadyExistsError if there is already an option with `value`
   * @see `addAlias()`
   *
   * An option must always have a label; aliases are instead optional.
   * There must be no existing option with the specified `value`, otherwise
   * an exception will be thrown with the label of the existing option.
   * To add aliases to an existing option, use `addAlias()` instead.
   * Currently, it is not possible to change a label after having added it.
   */
  template <typename... Aliases>
  Option_t const& addOption(Choices_t value, std::string label, Aliases... aliases);

  /**
   * @brief Adds aliases to an existing option.
   * @tparam Args any number of `std::string`
   * @param value the value of the option to assign labels to
   * @param aliases the additional alias(es) to assign to this option
   * @return the option being changed
   * @throw UnknownOptionError if no option with `value` is registered yet
   *
   */
  template <typename... Aliases>
  std::enable_if_t<details::AllConvertibleToStrings_v<Aliases...>, Option_t const&> addAlias(
    Choices_t value,
    Aliases... aliases);

  /**
   * @brief Adds labels to an existing option.
   * @tparam Aliases any number of `std::string`
   * @param option the option to assign labels to
   * @param aliases the additional alias(es) to assign to this option
   * @return the option being changed
   * @throw UnknownOptionError if no option with `value` is registered yet
   *
   * The option with the same value as the `option` argument is assigned the
   * specified aliases.
   */
  template <typename... Aliases>
  std::enable_if_t<details::AllConvertibleToStrings_v<Aliases...>, Option_t const&> addAlias(
    Option_t const& option,
    Aliases... aliases);

  /// Returns whether the selector has an option with the specified `value`.
  bool hasOption(Choices_t value) const;

  /// Returns whether the selector has an option with the specified `label`.
  bool hasOption(std::string const& label) const;

  /// Returns if the specified option is present in the selector (by value).
  bool hasOption(Option_t const& option) const;

  /// @}
  // --- END -- Option management ----------------------------------------------

  // --- BEGIN -- Option access ------------------------------------------------
  /// @name Option access
  /// @{

  /**
   * @brief Returns the specified option.
   * @param value value of the requested option
   * @return the requested option
   * @throw UnknownOptionError if there is no available option with `value`
   *                           (the string of the exception is empty)
   */
  Option_t const& get(Choices_t value) const;

  /**
   * @brief Returns the option with the specified label.
   * @param label label of the requested option
   * @return the requested option
   * @throw UnknownOptionError if there is no available option with `label`
   *                           (the string of the exception shows `label` value)
   */
  Option_t const& get(std::string const& label) const;

  /**
   * @brief Returns the option matching the specified label.
   * @param label label of the requested option
   * @return the requested option
   * @throw UnknownOptionError if there is no available option with `label`
   *                           (the string of the exception shows `label` value)
   */
  Option_t const& parse(std::string const& label) const;

  /// @}
  // --- END -- Option access --------------------------------------------------

  /// --- BEGIN ----------------------------------------------------------------
  /// @name Presentation and dumping
  /// @{

  /// Returns a string with the (main) name of all options.
  std::string optionListString(std::string const& sep = ", ") const;

  /// Returns a string with all the options, one per line.
  std::string optionListDump(std::string const& indent, std::string const& firstIndent) const;

  /// Returns a string with all the options, one per line.
  std::string optionListDump(std::string const& indent = "") const
  {
    return optionListDump(indent, indent);
  }

  /// @}
  /// --- END ------------------------------------------------------------------

private:
  /// Type of collection of options.
  using OptionList_t = std::vector<Option_t>;

  /// Type of label index (associative container: label to option index).
  using OptionLabelMap_t =
    std::map<std::string, std::size_t, details::SorterFrom<details::CaseInsensitiveComparer>>;

  OptionList_t fOptions; ///< The list of registered objects.

  /// Map from labels to option index in `fOptions`.
  OptionLabelMap_t fLabelToOptionIndex;

  /**
   * @brief Moves the specified option into the list of registered options.
   * @param option the option to move
   * @return the newly added option
   * @throw OptionAlreadyExistsError if there is already an option with the same
   *                                 `value` or any of the labels; in that case,
   *                                 `label()` method of the exception will
   *                                 report the offending label of `option` or
   *                                 its name if it is the value to be
   *                                 duplicated
   */
  Option_t const& addOption(Option_t&& option);

  /// Associates `label` to the option at `index`.
  /// @throw OptionAlreadyExistsError if there is already an option with `label`
  void recordLabel(std::string&& label, std::size_t index);

  /// Associates all `labels` to the option at `index`.
  /// @throw OptionAlreadyExistsError if there is already an option with any of
  ///                                 the aliases
  template <typename... Aliases>
  std::enable_if_t<details::AllConvertibleToStrings_v<Aliases...>>
  recordLabels(std::size_t index, std::string alias, Aliases... moreAliases);

  /// Removes the specified label from the register.
  void unregisterLabel(std::string const& label);

  /// Returns an iterator to the option with `label`, or `npos` if none.
  typename OptionList_t::const_iterator findOption(Choices_t value) const;

  /// Returns an iterator to the option with `label`, or `npos` if none.
  typename OptionList_t::iterator findOption(Choices_t value);

  /// Returns the index of the option with `label`, or `npos` if none.
  std::size_t findOptionIndex(Choices_t value) const;

  /// Returns the index of the option with `label`, or `npos` if none.
  std::size_t findOptionIndex(std::string const& label) const;

  /// Special value.
  static constexpr auto npos = std::numeric_limits<std::size_t>::max();

}; // class util::MultipleChoiceSelection

// -----------------------------------------------------------------------------
namespace util::details {

  /// --- BEGIN -- Comparison operators ----------------------------------------
  /// @name Option comparison operators
  /// @{

  //@{
  /// Returns whether `option` has the specified `value`.
  template <typename Choices>
  bool operator==(MultipleChoiceSelectionOption_t<Choices> const& option, Choices const value);
  template <typename Choices>
  bool operator==(Choices const value, MultipleChoiceSelectionOption_t<Choices> const& option);
  //@}

  //@{
  /// Returns whether `option` does not have the specified `value`.
  template <typename Choices>
  bool operator!=(MultipleChoiceSelectionOption_t<Choices> const& option, Choices const value);
  template <typename Choices>
  bool operator!=(Choices const value, MultipleChoiceSelectionOption_t<Choices> const& option);
  //@}

  //@{
  /// Returns whether `option` has the specified name or alias.
  template <typename Choices>
  bool operator==(MultipleChoiceSelectionOption_t<Choices> const& option, std::string const& label);
  template <typename Choices>
  bool operator==(std::string const& label, MultipleChoiceSelectionOption_t<Choices> const& option);
  //@}

  //@{
  /// Returns whether `option` does not have the specified name or alias.
  template <typename Choices>
  bool operator!=(MultipleChoiceSelectionOption_t<Choices> const& option, std::string const& label);
  template <typename Choices>
  bool operator!=(std::string const& label, MultipleChoiceSelectionOption_t<Choices> const& option);
  //@}

  /// @}
  /// --- END -- Comparison operators ------------------------------------------

  /// Prints an option into a stream.
  template <typename Choices>
  std::ostream& operator<<(std::ostream& out,
                           MultipleChoiceSelectionOption_t<Choices> const& option);

} // namespace util

// =============================================================================
// ---  template implementation
// =============================================================================
// -----------------------------------------------------------------------------
// ---  (details)
// -----------------------------------------------------------------------------
template <typename B1, typename E1, typename B2, typename E2, typename Comp>
bool my_lexicographical_compare(B1 b1, E1 e1, B2 b2, E2 e2, Comp less)
{

  while (b1 != e1) {
    if (b2 == e2) return false; // shorter is less

    if (less(*b1, *b2)) return true;
    if (less(*b2, *b1)) return false;
    // equal so far...
    ++b1;
    ++b2;
  }            // while
  return true; // 1 is shorter
} // my_lexicographical_compare()

inline bool util::details::CaseInsensitiveComparer::equal(std::string const& a,
                                                          std::string const& b)
{
  return std::equal(a.begin(), a.end(), b.begin(), b.end(), eq_lower);
}

// -----------------------------------------------------------------------------
inline bool util::details::CaseInsensitiveComparer::less(std::string const& a, std::string const& b)
{
  return std::lexicographical_compare(a.begin(), a.end(), b.begin(), b.end(), cmp_lower);
} // util::details::CaseInsensitiveComparer::less()

// -----------------------------------------------------------------------------
inline bool util::details::CaseInsensitiveComparer::cmp_lower(unsigned char a, unsigned char b)
{
  return std::tolower(a) < std::tolower(b);
}

// -----------------------------------------------------------------------------
inline bool util::details::CaseInsensitiveComparer::eq_lower(unsigned char a, unsigned char b)
{
  return std::tolower(a) == std::tolower(b);
}

// -----------------------------------------------------------------------------
template <typename Comparer>
bool util::details::SorterFrom<Comparer>::operator()(std::string const& a,
                                                     std::string const& b) const
{
  return Comparer::less(a, b);
}

// -----------------------------------------------------------------------------
namespace util::details {

  template <typename Value, typename /* = void */>
  struct ValueToString {
    static constexpr bool can_convert = false;

    template <typename T>
    static std::optional<std::string> convert(T const&)
    {
      return {};
    }
  }; // struct ValueToString

  // enumerators
  template <typename Value>
  struct ValueToString<Value, std::enable_if_t<std::is_enum_v<Value>>> {
    static constexpr bool can_convert = true;

    template <typename T>
    static std::optional<std::string> convert(T const& value)
    {
      return {std::to_string(static_cast<std::underlying_type_t<T>>(value))};
    }
  }; // ValueToString(enum)

  // whatever converts to `std::to_string`
  template <typename Value>
  struct ValueToString<Value,
                       std::enable_if_t<std::is_convertible_v<Value, std::string> ||
                                        std::is_constructible_v<std::string, Value>>> {
    static constexpr bool can_convert = true;
    template <typename T>
    static std::optional<std::string> convert(T const& value)
    {
      return {std::string{value}};
    }
  }; // ValueToString(string)

  // whatever supports `std::to_string`
  template <typename Value>
  struct ValueToString<Value, std::void_t<decltype(std::to_string(std::declval<Value>()))>> {
    static constexpr bool can_convert = true;
    template <typename T>
    static std::optional<std::string> convert(T const& value)
    {
      return {std::to_string(value)};
    }
  }; // ValueToString(to_string)

} // namespace util::details

// -----------------------------------------------------------------------------
// --- util::details::MultipleChoiceSelectionOption_t
// -----------------------------------------------------------------------------
template <typename Choices>
template <typename... Aliases>
util::details::MultipleChoiceSelectionOption_t<Choices>::MultipleChoiceSelectionOption_t(
  Choices_t value,
  std::string name,
  Aliases... aliases)
  : fValue(value)
{
  fLabels.reserve(1U + sizeof...(aliases));
  addAlias(std::move(name), std::move(aliases)...);
} // util::details::MultipleChoiceSelectionOption_t<>::MultipleChoiceSelectionOption_t

// -----------------------------------------------------------------------------
template <typename Choices>
template <typename... Aliases>
auto util::details::MultipleChoiceSelectionOption_t<Choices>::addAlias(std::string alias,
                                                                       Aliases... moreAliases)
  -> std::enable_if_t<AllConvertibleToStrings_v<Aliases...>, Option_t&>
{
  fLabels.push_back(std::move(alias));
  if constexpr (sizeof...(moreAliases) > 0)
    return addAlias(std::move(moreAliases)...);
  else
    return *this;
} // util::details::MultipleChoiceSelectionOption_t<>::addAlias()

// -----------------------------------------------------------------------------
template <typename Choices>
bool util::details::MultipleChoiceSelectionOption_t<Choices>::match(std::string const& label) const
{
  return std::find_if(fLabels.begin(), fLabels.end(), [&label](std::string const& alias) {
           return equal(label, alias);
         }) != fLabels.end();
} // util::details::MultipleChoiceSelectionOption_t<>::match()

// -----------------------------------------------------------------------------
template <typename Choices>
std::string util::details::MultipleChoiceSelectionOption_t<Choices>::value_as_string(
  std::string const& defValue) const
{
  return value_as_string(value(), defValue);
}

// -----------------------------------------------------------------------------
template <typename Choices>
std::string util::details::MultipleChoiceSelectionOption_t<Choices>::value_as_string() const
{
  return value_as_string(name());
}

// -----------------------------------------------------------------------------
template <typename Choices>
std::optional<std::string> util::details::MultipleChoiceSelectionOption_t<Choices>::value_as_string(
  Choices_t value)
{
  return details::ValueToString<std::decay_t<Choices_t>>::convert(value);
} // util::details::MultipleChoiceSelectionOption_t<>::value_as_string()

// -----------------------------------------------------------------------------
template <typename Choices>
std::string util::details::MultipleChoiceSelectionOption_t<Choices>::value_as_string(
  Choices_t value,
  std::string const& defValue)
{
  return value_as_string(value).value_or(defValue);
} // util::details::MultipleChoiceSelectionOption_t<>::value_as_string()

// -----------------------------------------------------------------------------
template <typename Choices>
std::string util::details::MultipleChoiceSelectionOption_t<Choices>::dump() const
{
  auto iLabel = fLabels.begin();
  auto const lend = fLabels.end();
  std::string s{'"'};
  s += *iLabel;
  s += '"';
  auto const valueStr = value_as_string();
  if (valueStr != *iLabel) {
    s += " [=";
    s += valueStr;
    s += "]";
  }
  if (++iLabel != lend) {
    s += " (aliases: \"";
    s += *iLabel;
    s += '"';
    while (++iLabel != lend) {
      s += " \"";
      s += *iLabel;
      s += '"';
    } // while
    s += ')';
  } // if aliases

  return s;
} // util::details::MultipleChoiceSelectionOption_t<>::dump()

// -----------------------------------------------------------------------------
template <typename Choices>
bool util::details::operator==(MultipleChoiceSelectionOption_t<Choices> const& option,
                               Choices const value)
{
  return option.value() == value;
}

template <typename Choices>
bool util::details::operator==(Choices const value,
                               MultipleChoiceSelectionOption_t<Choices> const& option)
{
  return option == value;
}

// -----------------------------------------------------------------------------
template <typename Choices>
bool util::details::operator!=(MultipleChoiceSelectionOption_t<Choices> const& option,
                               Choices const value)
{
  return option.value() != value;
}

template <typename Choices>
bool util::details::operator!=(Choices const value,
                               MultipleChoiceSelectionOption_t<Choices> const& option)
{
  return option != value;
}

// -----------------------------------------------------------------------------
template <typename Choices>
bool util::details::operator==(MultipleChoiceSelectionOption_t<Choices> const& option,
                               std::string const& label)
{
  return option.match(label);
}

template <typename Choices>
bool util::details::operator==(std::string const& label,
                               MultipleChoiceSelectionOption_t<Choices> const& option)
{
  return option == label;
}

// -----------------------------------------------------------------------------
template <typename Choices>
bool util::details::operator!=(MultipleChoiceSelectionOption_t<Choices> const& option,
                               std::string const& label)
{
  return !option.match(label);
}

template <typename Choices>
bool util::details::operator!=(std::string const& label,
                               MultipleChoiceSelectionOption_t<Choices> const& option)
{
  return option != label;
}

// -----------------------------------------------------------------------------
template <typename Choices>
std::ostream& util::details::operator<<(std::ostream& out,
                                        MultipleChoiceSelectionOption_t<Choices> const& option)
{
  out << option.name();
  return out;
}

// -----------------------------------------------------------------------------
// --- util::MultipleChoiceSelection
// -----------------------------------------------------------------------------
template <typename Choices>
template <typename... Options>
util::MultipleChoiceSelection<Choices>::MultipleChoiceSelection(
  std::initializer_list<Option_t> options)
{
  for (Option_t option : options)
    addOption(std::move(option));
} // util::MultipleChoiceSelection<>::MultipleChoiceSelection()

// -----------------------------------------------------------------------------
template <typename Choices>
std::size_t util::MultipleChoiceSelection<Choices>::size() const
{
  return fOptions.size();
}

// -----------------------------------------------------------------------------
template <typename Choices>
bool util::MultipleChoiceSelection<Choices>::empty() const
{
  return fOptions.empty();
}

// -----------------------------------------------------------------------------
template <typename Choices>
template <typename... Aliases>
auto util::MultipleChoiceSelection<Choices>::addOption(Choices_t value,
                                                       std::string label,
                                                       Aliases... aliases) -> Option_t const&
{
  return addOption({value, std::move(label), std::move(aliases)...});
} // util::MultipleChoiceSelection<>::addOption()

// -----------------------------------------------------------------------------
template <typename Choices>
template <typename... Aliases>
auto util::MultipleChoiceSelection<Choices>::addAlias(Choices_t value, Aliases... aliases)
  -> std::enable_if_t<details::AllConvertibleToStrings_v<Aliases...>, Option_t const&>
{
  std::size_t const index = findOptionIndex(value);
  if (index >= fOptions.size()) throw UnknownOptionError(Option_t::value_as_string(value, ""));
  recordLabels(index, aliases...);
  return fOptions[index].addAlias(std::move(aliases)...);
} // util::MultipleChoiceSelection<>::addAlias()

// -----------------------------------------------------------------------------
template <typename Choices>
template <typename... Aliases>
auto util::MultipleChoiceSelection<Choices>::addAlias(Option_t const& option, Aliases... aliases)
  -> std::enable_if_t<details::AllConvertibleToStrings_v<Aliases...>, Option_t const&>
{
  return addAlias(option.value(), std::move(aliases)...);
} // util::MultipleChoiceSelection<>::addAlias()

// -----------------------------------------------------------------------------
template <typename Choices>
bool util::MultipleChoiceSelection<Choices>::hasOption(Choices_t value) const
{
  return findOption(value) != fOptions.end();
}

// -----------------------------------------------------------------------------
template <typename Choices>
bool util::MultipleChoiceSelection<Choices>::hasOption(std::string const& label) const
{
  return fLabelToOptionIndex.find(label) != fLabelToOptionIndex.end();
} // util::MultipleChoiceSelection<>::hasOption(string)

// -----------------------------------------------------------------------------
template <typename Choices>
bool util::MultipleChoiceSelection<Choices>::hasOption(Option_t const& option) const
{
  return hasOption(option.value());
}

// -----------------------------------------------------------------------------
template <typename Choices>
auto util::MultipleChoiceSelection<Choices>::get(Choices_t value) const -> Option_t const&
{
  auto const iOption = findOption(value);
  if (iOption == fOptions.end())
    throw UnknownOptionError(Option_t::value_as_string(value).value_or(""));
  return *iOption;
} // util::MultipleChoiceSelection<>::get(value)

// -----------------------------------------------------------------------------
template <typename Choices>
auto util::MultipleChoiceSelection<Choices>::get(std::string const& label) const -> Option_t const&
{
  auto const iLabelIndexPair = fLabelToOptionIndex.find(label);
  if (iLabelIndexPair == fLabelToOptionIndex.end()) { throw UnknownOptionError(label); }
  assert(iLabelIndexPair->second < fOptions.size());
  return fOptions[iLabelIndexPair->second];
} // util::MultipleChoiceSelection<>::get(string)

// -----------------------------------------------------------------------------
template <typename Choices>
auto util::MultipleChoiceSelection<Choices>::parse(std::string const& label) const
  -> Option_t const&
{
  return get(label);
} // util::MultipleChoiceSelection<>::parse()

// -----------------------------------------------------------------------------
template <typename Choices>
std::string util::MultipleChoiceSelection<Choices>::optionListString(
  std::string const& sep /* = ", " */) const
{
  using namespace std::string_literals;

  auto iOption = fOptions.begin();
  auto const oend = fOptions.end();

  if (iOption == oend) return "<no options>"s;

  std::string s{*iOption};
  while (++iOption != oend) {
    s += sep;
    s += *iOption;
  } // while
  return s;
} // util::MultipleChoiceSelection<>::optionListString()

// -----------------------------------------------------------------------------
template <typename Choices>
std::string util::MultipleChoiceSelection<Choices>::optionListDump(
  std::string const& indent,
  std::string const& firstIndent) const
{
  using namespace std::string_literals;

  auto iOption = fOptions.begin();
  auto const oend = fOptions.end();

  if (iOption == oend) return firstIndent + "<no options>\n"s;

  std::string s{firstIndent};
  s += iOption->dump();
  s += '\n';
  while (++iOption != oend) {
    s += indent;
    s += iOption->dump();
    s += '\n';
  } // while
  return s;
} // util::MultipleChoiceSelection<>::optionListDump()

// -----------------------------------------------------------------------------
template <typename Choices>
auto util::MultipleChoiceSelection<Choices>::addOption(Option_t&& option) -> Option_t const&
{
  std::size_t const newOptionIndex = size();

  fOptions.push_back(std::move(option));
  Option_t const& newOption = fOptions.back();
  auto const& labels = newOption.labels();
  for (auto iLabel = labels.begin(); iLabel != labels.end(); ++iLabel) {
    try {
      recordLabel(std::string{*iLabel}, newOptionIndex);
    }
    catch (OptionAlreadyExistsError const&) {
      // we attempt to offer a strong guarantee here,
      // that the object is restored in the state it was before the call

      // remove the new entries from the index
      // (*iLabel was not inserted, and all labels before it were new)
      for (auto iNewLabel = labels.begin(); iNewLabel != iLabel; ++iNewLabel)
        unregisterLabel(*iNewLabel);

      // remove the new option from the list
      fOptions.pop_back();

      // let the caller handle the rest
      throw;
    } // try ... catch
  }   // for labels

  return newOption;
} // util::MultipleChoiceSelection<>::addOption()

// -----------------------------------------------------------------------------
template <typename Choices>
void util::MultipleChoiceSelection<Choices>::recordLabel(std::string&& label, std::size_t index)
{

  auto const iOption = fLabelToOptionIndex.lower_bound(label);

  // check for duplicate entries: if iOption is not `end()`, it points to an
  // element whose key is either greater or equal (equivalent) to `label`;
  // if the key is greater than `label`, we are good:
  // we check it by requiring that `label` is smaller than the key;
  // `key_compare(a, b)` comparison is equivalent to `a < b`,
  if ((iOption != fLabelToOptionIndex.end()) &&
      !OptionLabelMap_t::key_compare()(label, iOption->first)) {
    throw OptionAlreadyExistsError(label); // maybe too terse?
  }

  // the actual recording, at last:
  fLabelToOptionIndex.emplace_hint(iOption, std::move(label), index);

} // util::MultipleChoiceSelection<>::recordLabel()

// -----------------------------------------------------------------------------
template <typename Choices>
template <typename... Aliases>
auto util::MultipleChoiceSelection<Choices>::recordLabels(std::size_t index,
                                                          std::string alias,
                                                          Aliases... moreAliases)
  -> std::enable_if_t<details::AllConvertibleToStrings_v<Aliases...>>
{
  try {
    recordLabel(std::move(alias), index);
    if constexpr (sizeof...(moreAliases) > 0U) recordLabels(index, std::move(moreAliases)...);
  }
  catch (OptionAlreadyExistsError const&) {
    unregisterLabel(alias); // if recordLabel() call threw alias is still intact
    throw;
  }
} // util::MultipleChoiceSelection<>::recordLabels()

// -----------------------------------------------------------------------------
template <typename Choices>
void util::MultipleChoiceSelection<Choices>::unregisterLabel(std::string const& label)
{
  fLabelToOptionIndex.erase(label);
}

// -----------------------------------------------------------------------------
template <typename Choices>
auto util::MultipleChoiceSelection<Choices>::findOption(Choices_t value) const ->
  typename OptionList_t::const_iterator
{
  auto const matchValue = [value](Option_t const& option) { return option.value() == value; };
  return std::find_if(fOptions.begin(), fOptions.end(), matchValue);
} // util::MultipleChoiceSelection<>::findOption(value) const

// -----------------------------------------------------------------------------
template <typename Choices>
auto util::MultipleChoiceSelection<Choices>::findOption(Choices_t value) ->
  typename OptionList_t::iterator
{
  auto const matchValue = [value](Option_t const& option) { return option.value() == value; };
  return std::find_if(fOptions.begin(), fOptions.end(), matchValue);
} // util::MultipleChoiceSelection<>::findOption(value)

// -----------------------------------------------------------------------------
template <typename Choices>
std::size_t util::MultipleChoiceSelection<Choices>::findOptionIndex(Choices_t value) const
{
  auto const d = static_cast<std::size_t>(std::distance(fOptions.begin(), findOption(value)));
  return (d >= size()) ? npos : d;
} // util::MultipleChoiceSelection<>::findOptionIndex(value) const

// -----------------------------------------------------------------------------
template <typename Choices>
std::size_t util::MultipleChoiceSelection<Choices>::findOptionIndex(std::string const& label) const
{
  auto const iOption = fLabelToOptionIndex.find(label);
  return (iOption == fLabelToOptionIndex.end()) ? npos : iOption->second;
} // util::MultipleChoiceSelection<>::findOptionIndex(string) const

// -----------------------------------------------------------------------------

// =============================================================================

#endif // LARDATAALG_UTILITIES_MULTIPLECHOICESELECTION_H
