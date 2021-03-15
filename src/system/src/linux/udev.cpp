/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "system/linux/udev.h"

#include <gsl/gsl>

namespace sq::system::linux {

UdevDevice::UdevDevice(udev_device *p) noexcept : p_{p} {}

udev_device *UdevDevice::create_new(udev *udev_context,
                                    const UdevSysPath &sys_path) {
  return udev_device_new_from_syspath(udev_context, sys_path.path_.c_str());
}

udev_device *UdevDevice::get_impl() const { return p_; }

void UdevDevice::add_ref() const noexcept {
  Expects(p_ != nullptr);
  udev_device_ref(p_);
}

void UdevDevice::remove_ref() const noexcept {
  Expects(p_ != nullptr);
  udev_device_unref(p_);
}

std::string UdevDevice::sys_path() const {
  Expects(p_ != nullptr);
  const gsl::czstring<> syspath = udev_device_get_syspath(p_);
  if (syspath == nullptr) {
    throw UdevError{"NULL returned from udev_device_get_syspath()"};
  }
  return std::string{syspath};
}

std::string UdevDevice::sys_name() const {
  Expects(p_ != nullptr);
  const gsl::czstring<> sysname = udev_device_get_sysname(p_);
  if (sysname == nullptr) {
    throw UdevError{"NULL returned from udev_device_get_sysname()"};
  }
  return std::string{sysname};
}

std::string UdevDevice::subsystem() const {
  Expects(p_ != nullptr);
  gsl::czstring<> subsystem = udev_device_get_subsystem(p_);
  if (subsystem == nullptr) {
    subsystem = "";
  }
  return std::string{subsystem};
}

std::string UdevDevice::dev_node() const {
  Expects(p_ != nullptr);
  gsl::czstring<> devnode = udev_device_get_devnode(p_);
  if (devnode == nullptr) {
    devnode = "";
  }
  return std::string{devnode};
}

UdevContext::UdevContext(udev *p) noexcept : p_{p} {
  Expects(p != nullptr);
  Ensures(p_ != nullptr);
}

udev *UdevContext::create_new() { return udev_new(); }

udev *UdevContext::get_impl() const { return p_; }

void UdevContext::add_ref() const noexcept {
  Expects(p_ != nullptr);
  udev_ref(p_);
}
void UdevContext::remove_ref() const noexcept {
  Expects(p_ != nullptr);
  udev_unref(p_);
}

UdevDeviceRange UdevContext::devices() const {
  auto enumerator = make_udev<UdevDeviceEnumerator>(p_);
  enumerator->scan();
  return UdevDeviceRange{UdevDeviceIterator{std::move(enumerator)},
                         UdevDeviceIterator{}};
}

UdevDeviceEnumerator::UdevDeviceEnumerator(udev_enumerate *p) noexcept
    : p_{p} {}

udev_enumerate *UdevDeviceEnumerator::create_new(udev *udev_context) {
  Expects(udev_context != nullptr);
  return udev_enumerate_new(udev_context);
}

udev_enumerate *UdevDeviceEnumerator::get_impl() const { return p_; }

void UdevDeviceEnumerator::add_ref() const noexcept {
  Expects(p_ != nullptr);
  udev_enumerate_ref(p_);
}
void UdevDeviceEnumerator::remove_ref() const noexcept {
  Expects(p_ != nullptr);
  udev_enumerate_unref(p_);
}

void UdevDeviceEnumerator::scan() const {
  Expects(p_ != nullptr);
  auto status = udev_enumerate_scan_devices(p_);
  if (status != 0) {
    throw UdevError{
        fmt::format("udev_enumerate_scan_devices returned {}", status)};
  }
}

UdevDeviceCursor::UdevDeviceCursor(UdevPtr<UdevDeviceEnumerator> enumerator)
    : enumerator_{std::move(enumerator)}, entry_{udev_enumerate_get_list_entry(
                                              enumerator_->p_)} {
  // Valid pre-conditions that don't work with the Expects() macro because
  // enumerator is moved from:
  // Expects(enumerator != nullptr);
  // Expects(enumerator->p_ != nullptr);
  Ensures(enumerator_ != nullptr);
}

UdevPtr<UdevDevice> UdevDeviceCursor::read() const {
  Expects(entry_ != nullptr);
  Expects(enumerator_->get_impl() != nullptr);

  const gsl::czstring<> syspath = udev_list_entry_get_name(entry_);
  if (syspath == nullptr) {
    throw UdevError{"NULL returned from udev_list_entry_get_name()"};
  }

  auto *udev_context = udev_enumerate_get_udev(enumerator_->get_impl());
  if (udev_context == nullptr) {
    throw UdevError{"NULL returned from udev_enumerate_get_udev()"};
  }

  return make_udev<UdevDevice>(udev_context, UdevSysPath{syspath});
}

bool UdevDeviceCursor::equal(const UdevDeviceCursor &other) const noexcept {
  return entry_ == other.entry_;
}

void UdevDeviceCursor::next() {
  Expects(entry_ != nullptr);
  entry_ = udev_list_entry_get_next(entry_);
}

} // namespace sq::system::linux
