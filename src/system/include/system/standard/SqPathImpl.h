/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#ifndef SQ_INCLUDE_GUARD_system_standard_SqPathImpl_h_
#define SQ_INCLUDE_GUARD_system_standard_SqPathImpl_h_

#include "system/SqPath.gen.h"

#include <filesystem>

namespace sq::system::standard {

class SqPathImpl
    : public SqPath<SqPathImpl>
{
public:
    explicit SqPathImpl(const std::filesystem::path& value);
    explicit SqPathImpl(std::filesystem::path&& value);

    [[nodiscard]] Result get_string() const;
    [[nodiscard]] Result get_parent() const;
    [[nodiscard]] Result get_filename() const;
    [[nodiscard]] Result get_extension() const;
    [[nodiscard]] Result get_stem() const;
    [[nodiscard]] Result get_children() const;
    [[nodiscard]] Result get_parts() const;
    [[nodiscard]] Result get_absolute() const;
    [[nodiscard]] Result get_canonical() const;
    [[nodiscard]] Result get_is_absolute() const;
    [[nodiscard]] Result get_size() const;
    [[nodiscard]] Primitive to_primitive() const override;

private:
    std::filesystem::path value_;
};


} // namespace sq::system::standard

#endif // SQ_INCLUDE_GUARD_system_standard_SqPathImpl_h_
