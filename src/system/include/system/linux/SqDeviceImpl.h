/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_linux_SqDeviceImpl_h_
#define SQ_INCLUDE_GUARD_system_linux_SqDeviceImpl_h_

#include "system/SqDevice.gen.h"

#include "system/linux/udev.h"
#include "util/typeutil.h"

namespace sq::system::linux {

class SqDeviceImpl : public SqDevice<SqDeviceImpl> {
public:
  explicit SqDeviceImpl(UdevPtr<UdevDevice> dev);

  SQ_ND Result get_sys_path() const;
  SQ_ND Result get_sys_name() const;
  SQ_ND Result get_subsystem() const;
  SQ_ND Result get_dev_node() const;
  SQ_ND Primitive to_primitive() const override;

private:
  UdevPtr<UdevDevice> dev_;
};

} // namespace sq::system::linux

#endif // SQ_INCLUDE_GUARD_system_linux_SqDeviceImpl_h_
