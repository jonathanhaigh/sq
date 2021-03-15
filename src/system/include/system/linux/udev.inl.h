/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_udev_inl_h_
#define SQ_INCLUDE_GUARD_system_linux_udev_inl_h_

#include "common_types/errors.h"
#include "util/typeutil.h"

#include <fmt/format.h>
#include <gsl/gsl>

namespace sq::system::linux {

template <typename T> UdevPtr<T>::UdevPtr(SQ_MU std::nullptr_t np) noexcept {}

template <typename T>
UdevPtr<T>::UdevPtr(const UdevPtr &other) noexcept : t_{other.t_.p_} {
  if (t_.p_ != nullptr) {
    t_.add_ref();
  }
}

template <typename T>
UdevPtr<T>::UdevPtr(UdevPtr &&other) noexcept : t_{other.t_.p_} {
  other.t_.p_ = nullptr;
}

// When a raw pointer is first obtained from libudev it already has a reference
// count of 1 so we don't need to adjust it here.
template <typename T> UdevPtr<T>::UdevPtr(RawPtr p) noexcept : t_{p} {
  Expects(p != nullptr);
  Ensures(t_.p_ != nullptr);
}

template <typename T>
UdevPtr<T> &UdevPtr<T>::operator=(const UdevPtr &other) noexcept {
  // Note: must call add_ref before remove_ref to deal with the case where
  // *this == other
  if (other.t_.p_ != nullptr) {
    other.t_.add_ref();
  }
  if (t_.p_ != nullptr) {
    t_.remove_ref();
  }
  t_.p_ = other.t_.p_;
  return *this;
}

template <typename T> UdevPtr<T>::~UdevPtr() noexcept {
  if (t_.p_ != nullptr) {
    t_.remove_ref();
  }
}

template <typename T>
UdevPtr<T> &UdevPtr<T>::operator=(UdevPtr &&other) noexcept {
  std::swap(t_.p_, other.t_.p_);
  return *this;
}

template <typename T> void UdevPtr<T>::reset() noexcept {
  UdevPtr<T>{}.swap(*this);
}

template <typename T> void UdevPtr<T>::swap(UdevPtr &other) noexcept {
  std::swap(t_.p_, other.t_.p_);
}

template <typename T> T *UdevPtr<T>::get() const noexcept {
  if (t_.p_ == nullptr) {
    return nullptr;
  }
  return &t_;
}

template <typename T> T &UdevPtr<T>::operator*() const noexcept {
  Expects(t_.p_ != nullptr);
  return t_;
}

template <typename T> T *UdevPtr<T>::operator->() const noexcept {
  return get();
}

template <typename T> UdevPtr<T>::operator bool() const noexcept {
  return t_.p_ != nullptr;
}

template <typename T>
auto UdevPtr<T>::operator<=>(const UdevPtr<T> &other) const noexcept {
  return std::compare_three_way{}(t_.p_, other.t_.p_);
}

template <typename T>
bool UdevPtr<T>::operator==(const UdevPtr<T> &other) const noexcept {
  return t_.p_ == other.t_.p_;
}

template <typename T>
auto UdevPtr<T>::operator<=>(std::nullptr_t np) const noexcept {
  return std::compare_three_way{}(t_.p_, static_cast<decltype(t_.p_)>(np));
}

template <typename T>
bool UdevPtr<T>::operator==(std::nullptr_t np) const noexcept {
  return t_.p_ == static_cast<decltype(t_.p_)>(np);
}

template <typename U, typename... Args> UdevPtr<U> make_udev(Args &&...args) {
  auto *const p = U::create_new(SQ_FWD(args)...);
  if (p == nullptr) {
    throw UdevError{
        fmt::format("Failed to create new {}", util::base_type_name(p))};
  }
  return UdevPtr<U>{p};
}

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_udev_inl_h_
