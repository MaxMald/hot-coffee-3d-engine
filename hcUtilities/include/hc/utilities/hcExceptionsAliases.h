#pragma once

#include <stdexcept>
#include <ios>

namespace hc
{
  using Exception = std::exception;
  using IOException = std::ios_base::failure;
  using OutOfRangeException = std::out_of_range;
  using InvalidArgumentException = std::invalid_argument;
  using LogicErrorException = std::logic_error;
  using RuntimeErrorException = std::runtime_error;
  using LengthErrorException = std::length_error;
  using RangeErrorException = std::range_error;
  using DomainErrorException = std::domain_error;
  using BadAllocException = std::bad_alloc;
}
