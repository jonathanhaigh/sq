#include "system/linux/SqDeviceImpl.h"

#include "system/linux/SqPathImpl.h"
#include "system/linux/SqStringImpl.h"

namespace sq::system::linux {

SqDeviceImpl::SqDeviceImpl(UdevPtr<UdevDevice> dev) : dev_{SQ_FWD(dev)} {
  Expects(dev_ != nullptr);
}

Result SqDeviceImpl::get_sys_path() const {
  Expects(dev_ != nullptr);
  return std::make_shared<SqPathImpl>(dev_->sys_path());
}

Result SqDeviceImpl::get_sys_name() const {
  Expects(dev_ != nullptr);
  return std::make_shared<SqStringImpl>(dev_->sys_name());
}

Result SqDeviceImpl::get_subsystem() const {
  Expects(dev_ != nullptr);
  return std::make_shared<SqStringImpl>(dev_->subsystem());
}

Result SqDeviceImpl::get_dev_node() const {
  Expects(dev_ != nullptr);
  return std::make_shared<SqPathImpl>(dev_->dev_node());
}

Primitive SqDeviceImpl::to_primitive() const {
  Expects(dev_ != nullptr);
  return PrimitiveString{dev_->sys_name()};
}

} // namespace sq::system::linux
