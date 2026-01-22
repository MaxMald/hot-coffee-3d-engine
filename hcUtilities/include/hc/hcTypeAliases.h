#pragma once

#include <cstdint>
#include <cstddef>
#include <unordered_map>
#include <memory>
#include <vector>
#include <queue>
#include <functional>
#include <filesystem>
#include <typeindex>
#include <type_traits>
#include <cmath>
#include <variant>
#include <optional>
#include <array>

namespace hc
{
  using Int8 = std::int8_t;
  using Int16 = std::int16_t;
  using Int32 = std::int32_t;
  using Int64 = std::int64_t;
  using UInt8 = std::uint8_t;
  using UInt16 = std::uint16_t;
  using UInt32 = std::uint32_t;
  using UInt64 = std::uint64_t;
  using Char = char;
  using Char16 = char16_t;
  using Char32 = char32_t;
  using UChar = unsigned char;
  using Byte = UInt8;

  using Bool = bool;
  using SizeT = std::size_t;
  using Path = std::filesystem::path;
  using TypeIndex = std::type_index;

  template<typename K, typename V>
  using UnorderedMap = std::unordered_map<K, V>;

  template<typename T>
  using UniquePtr = std::unique_ptr<T>;

  template<typename T>
  using Vector = std::vector<T>;

  template<typename T>
  using SharedPtr = std::shared_ptr<T>;

  template<typename T, std::size_t N>
  using Array = std::array<T, N>;

  template<typename T>
  using WeakPtr = std::weak_ptr<T>;

  // FIFO queue alias
  template<typename T, typename Container = std::deque<T>>
  using Queue = std::queue<T, Container>;

  // Priority queue alias (max-heap by default). Customize Container and Compare if needed.
  template<typename T, typename Container = std::vector<T>, typename Compare = std::less<T>>
  using PriorityQueue = std::priority_queue<T, Container, Compare>;

  // Variant and Optional aliases
  template<typename... Types>
  using Variant = std::variant<Types...>;

  template<typename T>
  using Optional = std::optional<T>;

  template<typename T, typename... Args>
  inline UniquePtr<T> MakeUnique(Args&&... args)
  {
    return std::make_unique<T>(std::forward<Args>(args)...);
  }

  template<typename T, typename... Args>
  inline SharedPtr<T> MakeShared(Args&&... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  template<typename Base, typename Derived>
  using IsBaseOf = std::is_base_of<Base, Derived>;
}
