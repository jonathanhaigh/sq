/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "results/Serializer.h"

#include "core/narrow.h"
#include "core/typeutil.h"
#include "results/results.h"

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

namespace rj = rapidjson;

namespace sq::results {

class JsonSerializer : public Serializer {
public:
  explicit JsonSerializer(std::ostream &os)
      : os_wrapper_{os}, writer_{os_wrapper_} {}

  void start_array() override { writer_.StartArray(); }
  void end_array() override { writer_.EndArray(); }
  void start_object() override { writer_.StartObject(); }
  void end_object() override { writer_.EndObject(); }

  void write_key(std::string_view key) override {
    writer_.Key(key.data(), narrow<rj::SizeType>(key.size()));
  }

  void write_value(const Primitive &value) override {
    std::visit([this](const auto &v) { write_value(v); }, value);
  }

  void write_value(const PrimitiveString &str) {
    writer_.String(str.c_str(), narrow<rj::SizeType>(str.size()));
  }

  void write_value(const PrimitiveInt &i) { writer_.Int64(i); }
  void write_value(const PrimitiveBool &b) { writer_.Bool(b); }
  void write_value(const PrimitiveFloat &f) { writer_.Double(f); }
  void write_value(SQ_MU const PrimitiveNull &pn) { writer_.Null(); }

private:
  rj::OStreamWrapper os_wrapper_;
  rj::Writer<rj::OStreamWrapper> writer_;
};

std::unique_ptr<Serializer> get_serializer(std::ostream &os) {
  return std::make_unique<JsonSerializer>(os);
}

} // namespace sq::results
