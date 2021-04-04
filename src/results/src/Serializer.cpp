/* -----------------------------------------------------------------------------
 * Copyright 2021 Jonathan Haigh
 * SPDX-License-Identifier: MIT
 * ---------------------------------------------------------------------------*/

#include "results/Serializer.h"

#include "core/ASSERT.h"
#include "core/typeutil.h"
#include "results/results.h"

#include <iostream>
#include <stack>
#include <vector>

namespace sq::results {

class JsonSerializer : public Serializer {
private:
  enum class State {
    EmptyArray,
    NonemptyArray,
    EmptyObject,
    NonemptyObject,
    Done
  };
  using StateStack = std::stack<State, std::vector<State>>;

public:
  explicit JsonSerializer(std::ostream &os) : os_{os} {}

  void start_array() override {
    prepare_for_value();
    state_stack_.push(State::EmptyArray);
    os_ << '[';
  }

  void end_array() override {
    Expects(in_array());
    os_ << ']';
    state_stack_.pop();
  }

  void start_object() override {
    prepare_for_value();
    state_stack_.push(State::EmptyObject);
    os_ << '{';
  }

  void end_object() override {
    Expects(in_object());
    os_ << '}';
    state_stack_.pop();
  }

  void write_key(std::string_view key) override {
    prepare_for_key();
    write_string(key);
    os_ << ':';
  }

  void write_value(const Primitive &value) override {
    std::visit([this](const auto &v) { write_value(v); }, value);
  }

  void write_value(const PrimitiveString &str) {
    prepare_for_value();
    write_string(str);
  }

  void write_value(const PrimitiveInt &i) {
    prepare_for_value();
    fmt::print(os_, "{}", i);
  }

  void write_value(const PrimitiveBool &b) {
    prepare_for_value();
    os_ << (b ? "true" : "false");
  }

  void write_value(const PrimitiveFloat &f) {
    prepare_for_value();
    auto str = fmt::format("{}", f);
    os_ << str;
    // Make sure we always either:
    // * use scientific notation; or
    // * have at least one digit after the decimal point.
    // This hints to consumers that the number should be interpreted as a
    // real number rather than an integer.
    if (str.find_first_of("e.") == std::string::npos) {
      os_ << ".0";
    }
  }

  void write_value(SQ_MU const PrimitiveNull &pn) {
    prepare_for_value();
    os_ << "null";
  }

private:
  void prepare_for_value() {
    Expects(!done());
    if (state_stack_.empty()) {
      state_stack_.push(State::Done);
      return;
    }
    auto &state = state_stack_.top();

    switch (state) {
    case State::EmptyArray:
      state = State::NonemptyArray;
      return;

    case State::EmptyObject:
      // There should be a key before a value
      ASSERT(false);
      return;

    case State::NonemptyArray:
      os_ << ',';
      return;

    case State::NonemptyObject:
      return;

    case State::Done:
      ASSERT(false);
      return;
    }
  }

  void prepare_for_key() {
    Expects(in_object());
    auto &state = state_stack_.top();

    switch (state) {
    case State::NonemptyObject:
      os_ << ',';
      return;

    case State::EmptyObject:
      state = State::NonemptyObject;
      return;

    case State::EmptyArray:
    case State::NonemptyArray:
    case State::Done:
      ASSERT(false);
    }
  }

  SQ_ND bool in_array() const {
    return !state_stack_.empty() &&
           (state_stack_.top() == State::EmptyArray ||
            state_stack_.top() == State::NonemptyArray);
  }

  SQ_ND bool in_object() const {
    return !state_stack_.empty() &&
           (state_stack_.top() == State::EmptyObject ||
            state_stack_.top() == State::NonemptyObject);
  }

  SQ_ND bool done() const {
    return !state_stack_.empty() && state_stack_.top() == State::Done;
  }

  void write_string(std::string_view str) {
    os_ << '\"';
    for (const char c : str) {
      switch (c) {
      case '"':
        os_ << "\\\"";
        break;
      case '\\':
        os_ << "\\\\";
        break;
      case '\b':
        os_ << "\\b";
        break;
      case '\f':
        os_ << "\\f";
        break;
      case '\n':
        os_ << "\\n";
        break;
      case '\r':
        os_ << "\\r";
        break;
      case '\t':
        os_ << "\\t";
        break;
      default:
        static constexpr char first_control_char = '\x00';
        static constexpr char last_control_char = '\x1F';
        if (first_control_char <= c && c <= last_control_char) {
          fmt::print(os_, "\\u{:04x}", c);
        } else {
          os_ << c;
        }
      }
    }
    os_ << '\"';
  }

  std::ostream &os_;
  StateStack state_stack_;
};

std::unique_ptr<Serializer> get_serializer(std::ostream &os) {
  return std::make_unique<JsonSerializer>(os);
}

} // namespace sq::results
