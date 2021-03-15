/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_udev_h_
#define SQ_INCLUDE_GUARD_system_linux_udev_h_

#include <compare>
#include <libudev.h>
#include <range/v3/iterator/basic_iterator.hpp>
#include <range/v3/view/subrange.hpp>
#include <string>
#include <utility>

namespace sq::system::linux {

/**
 * Shared pointer to a udev object.
 *
 * libudev provides reference counting for its objects and UdevPtr is a C++
 * wrapper around that mechanism that resembles std::shared_ptr.
 *
 * Methods of UdevPtr all have the semantics of the shared_ptr methods with the
 * same names, with the slight exception of the constructor that takes a raw
 * libudev pointer.
 *
 * UdevPtr can be instantiated for types T that wrap raw libudev pointers to
 * provide an OO interface. The requirements on T are:
 *
 * * T must have a get_impl() const method that returns the underlying libudev
 *   raw pointer.
 *
 * * T must be regular.
 *
 * * T must have a _p member that is a raw pointer from libudev.
 *
 * * T must have add_ref() and remove_ref() methods that increment and
 *   decrement the pointer's reference count.
 *
 * * T must be nothrow-constructible from a raw libudev pointer.
 *
 * * Besides providing p_, add_ref() and remove_ref(), T must leave the
 *   management of _p up to UdevPtr.
 *
 * * In order to be created by make_udev<T, Args...>(Args&&... args)
 *   T::create_new(args...) must be valid.
 *
 * The requirements need only be satisfied from within UdevPtr and make_udev.
 * I.e. the associated methods and members need not be public if UdevPtr and
 * make_udev are friends of T. For this reason, the requirements are not
 * checked via C++ concepts because they would require the members and methods
 * to be public. It's also a bit tricky due to T being an incomplete type at
 * the point where T declares UdevPtr as a friend.
 */
template <typename T> class UdevPtr {
public:
  using RawPtr = decltype(T{}.p_);

  /**
   * Create a UdevPtr to manage the given libudev pointer.
   *
   * It is assumed that p has been freshly created using one of the libudev
   * udev*_new() functions so that it already has a reference count of 1.
   */
  explicit UdevPtr(RawPtr p) noexcept;

  UdevPtr() noexcept = default;
  UdevPtr(std::nullptr_t np) noexcept;
  UdevPtr(const UdevPtr &other) noexcept;
  UdevPtr(UdevPtr &&other) noexcept;
  UdevPtr &operator=(const UdevPtr &other) noexcept;
  UdevPtr &operator=(UdevPtr &&other) noexcept;
  ~UdevPtr() noexcept;

  void reset() noexcept;
  void swap(UdevPtr &other) noexcept;
  T *get() const noexcept;
  T &operator*() const noexcept;
  T *operator->() const noexcept;
  explicit operator bool() const noexcept;

  auto operator<=>(const UdevPtr &other) const noexcept;
  auto operator<=>(std::nullptr_t np) const noexcept;
  bool operator==(const UdevPtr &other) const noexcept;
  bool operator==(std::nullptr_t np) const noexcept;

private:
  // t_ is mutable because conceptually it is the pointee - it should be
  // modifiable from a const UdevPtr.
  //
  // Restrictions from libudev mean we can't use a UdevPtr from multiple
  // threads so the pre-C++11 "conceptually const" meaning of const methods is
  // good enough for us.
  mutable T t_;
};

/**
 * Create a UdevPtr pointing to a new libudev object.
 *
 * make_udev's first template parameter is a type that wraps a raw libudev
 * pointer. The wrapper must provide a static create_new() method to create a
 * new raw libudev object.
 */
template <typename U, typename... Args> UdevPtr<U> make_udev(Args &&...args);

/**
 * The path for a device in the /sys filesystem.
 */
struct UdevSysPath {
  std::string path_;
};

/**
 * A udev device.
 *
 * C++ wrapper of a udev_device*. Use make_udev<UdevDevice>(sys_path) to create
 * a UdevPtr<UdevDevice> rather than creating a UdevDevice directly.
 */
class UdevDevice {
public:
  /**
   * Get the path in the /sys filesystem for this device.
   *
   * Always returns a valid path or throws.
   */
  std::string sys_path() const;

  /**
   * Get the name in the /sys filesystem for this device.
   *
   * Always returns a valid name or throws.
   */
  std::string sys_name() const;

  /**
   * Get the name of the kernel subsystem to which this device belongs.
   *
   * Returns an empty string if there is no such subsystem.
   */
  std::string subsystem() const;

  /**
   * Get the path for the node in /dev for this device.
   *
   * Returns an empty string if there is no such node.
   */
  std::string dev_node() const;

  /**
   * Get the raw libudev udev_device* for this device.
   */
  udev_device *get_impl() const;

  constexpr auto operator<=>(const UdevDevice &) const noexcept = default;
  constexpr bool operator==(const UdevDevice &) const noexcept = default;

private:
  // required by make_udev()

  template <typename U, typename... Args>
  friend UdevPtr<U> make_udev(Args &&...args);

  static udev_device *create_new(udev *udev_context,
                                 const UdevSysPath &sys_path);

  // Required by UdevPtr

  friend class UdevPtr<UdevDevice>;

  explicit UdevDevice(udev_device *p) noexcept;
  UdevDevice() noexcept = default;
  UdevDevice(const UdevDevice &) noexcept = default;
  UdevDevice(UdevDevice &&) noexcept = default;
  UdevDevice &operator=(const UdevDevice &) noexcept = default;
  UdevDevice &operator=(UdevDevice &&) noexcept = default;
  ~UdevDevice() noexcept = default;

  void add_ref() const noexcept;
  void remove_ref() const noexcept;

  // UdevDevice is dumb regarding this pointer - it is owned by
  // UdevPtr<UdevDevice>.
  udev_device *p_ = nullptr;
};

/**
 * A udev enumeration context.
 *
 * C++ wrapper of a udev_enumerate*. Use make_udev<UdevDeviceEnumerator>() to
 * create a UdevPtr<UdevDeviceEnumerator> rather than creating a
 * UdevDeviceEnumerator directly.
 *
 * Basic usage:
 * auto udev_context = make_udev<UdevContext>();
 * auto enumerator = make_udev<UdevDeviceEnumerator>(udev_context.get_impl());
 * enumerator->scan();
 * auto device_range =
 * UdevDeviceRange{UdevDeviceIterator{std::move(enumerator)},
 *                                     UdevDeviceIterator{}};
 *
 * Although you can just get UdevContext to do that for you:
 * auto udev_context = make_udev<UdevContext>();
 * auto device_range = udev_context->devices();
 */
class UdevDeviceEnumerator {
public:
  /**
   * Scan the system for devices so that they can then be iterated over.
   */
  void scan() const;

  /**
   * Get the uderlying libudev udev_enumerate*.
   */
  udev_enumerate *get_impl() const;

  constexpr auto
  operator<=>(const UdevDeviceEnumerator &) const noexcept = default;
  constexpr bool
  operator==(const UdevDeviceEnumerator &) const noexcept = default;

private:
  friend class UdevDeviceCursor;

  // required by make_udev()

  template <typename U, typename... Args>
  friend UdevPtr<U> make_udev(Args &&...args);

  static udev_enumerate *create_new(udev *udev_context);

  // Required by UdevPtr

  friend class UdevPtr<UdevDeviceEnumerator>;

  explicit UdevDeviceEnumerator(udev_enumerate *p) noexcept;
  UdevDeviceEnumerator() noexcept = default;
  UdevDeviceEnumerator(const UdevDeviceEnumerator &) noexcept = default;
  UdevDeviceEnumerator(UdevDeviceEnumerator &&) noexcept = default;
  UdevDeviceEnumerator &
  operator=(const UdevDeviceEnumerator &) noexcept = default;
  UdevDeviceEnumerator &operator=(UdevDeviceEnumerator &&) noexcept = default;
  ~UdevDeviceEnumerator() noexcept = default;

  void add_ref() const noexcept;
  void remove_ref() const noexcept;

  // UdevDeviceEnumerator is dumb regarding this pointer - it is owned by
  // UdevPtr<UdevDeviceEnumerator>.
  udev_enumerate *p_ = nullptr;
};

/**
 * Cursor for iterating over udev devices.
 *
 * Implementation of basic iterator functionality that can be used by
 * ranges::basic_iterator to create an input_iterator for iterating over udev
 * devices in a udev list.
 */
class UdevDeviceCursor {
public:
  /**
   * Create a cursor from a UdevDeviceEnumerator.
   *
   * Note: UdevDeviceEnumerator::scan() must have already been run.
   */
  UdevDeviceCursor(UdevPtr<UdevDeviceEnumerator> enumerator);
  UdevDeviceCursor() noexcept = default;
  UdevDeviceCursor(const UdevDeviceCursor &) noexcept = default;
  UdevDeviceCursor(UdevDeviceCursor &&) noexcept = default;
  UdevDeviceCursor &operator=(const UdevDeviceCursor &) noexcept = default;
  UdevDeviceCursor &operator=(UdevDeviceCursor &&) noexcept = default;
  ~UdevDeviceCursor() noexcept = default;

  UdevPtr<UdevDevice> read() const;
  bool equal(const UdevDeviceCursor &other) const noexcept;
  void next();

private:
  UdevPtr<UdevDeviceEnumerator> enumerator_ = nullptr;
  udev_list_entry *entry_ = nullptr;
};

/**
 * Iterator for iterating over udev devices.
 *
 * Create from a UdevDeviceEnumerator:
 *
 * auto udev_context = make_udev<UdevContext>();
 * auto enumerator = make_udev<UdevDeviceEnumerator>(udev_context.get_impl());
 * enumerator->scan();
 * auto devices_begin = UdevDeviceIterator{std::move(enumerator)};
 * auto devices_end = UdevDeviceIterator{};
 *
 * Note that a default constructed UdevDeviceIterator is used as the "end"
 * iterator.
 *
 * Alternatively, just use UdevContext::devices() to get a device range.
 */
using UdevDeviceIterator = ranges::basic_iterator<UdevDeviceCursor>;

/**
 * Range of udev devices.
 *
 * Create using a begin/end pair of UdevDeviceIterators:
 * auto device_range = UdevDeviceRange{UdevDeviceIterator{enumerator},
 *                                     UdevDeviceIterator{}};
 *
 * Alternatively, just call UdevContext::devices().
 */
using UdevDeviceRange =
    ranges::subrange<UdevDeviceIterator, UdevDeviceIterator>;

/**
 * A udev context.
 *
 * C++ wrapper of a libudev udev*. Use make_udev<UdevContext>() to create a
 * UdevPtr<UdevContext> rather than creating a UdevContext directly.
 *
 * IMPORTANT: According to the libudev docs, struct udevs, and therefore
 * UdevContext objects must not be shared between threads, even if
 * synchronization mechanisms are used to ensure that only one thread accesses
 * at any one time.
 *
 * Basic usage:
 * auto udev_context = make_udev<UdevContext>();
 * auto device_range = udev_context->devices();
 * for (auto device : device_range) {
 *   std::cout << fmt::format("sys_path: {}; dev_path: {}\n",
 *                            device->sys_path(),
 *                            device->dev_path);
 * }
 */
class UdevContext {
public:
  /**
   * Get the devices in the system as an input_range of UdevPtr<UdevDevice>s.
   */
  UdevDeviceRange devices() const;

  /**
   * Get the underlying raw libudev udev*.
   */
  udev *get_impl() const;

  constexpr auto operator<=>(const UdevContext &) const noexcept = default;
  constexpr bool operator==(const UdevContext &) const noexcept = default;

private:
  // required by make_udev()

  template <typename U, typename... Args>
  friend UdevPtr<U> make_udev(Args &&...args);

  static udev *create_new();

  // Required by UdevPtr

  friend class UdevPtr<UdevContext>;

  explicit UdevContext(udev *p) noexcept;
  UdevContext() noexcept = default;
  UdevContext(const UdevContext &) noexcept = default;
  UdevContext(UdevContext &&) noexcept = default;
  UdevContext &operator=(const UdevContext &) noexcept = default;
  UdevContext &operator=(UdevContext &&) noexcept = default;
  ~UdevContext() noexcept = default;

  void add_ref() const noexcept;
  void remove_ref() const noexcept;

  // UdevContext is dumb regarding this pointer - it is owned by
  // UdevPtr<UdevContext>.
  udev *p_ = nullptr;
};

} // namespace sq::system::linux

#include "udev.inl.h"

#endif // SQ_INCLUDE_GUARD_system_linux_udev_h_
