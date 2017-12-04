// Copyright 2017 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#pragma once

namespace sitec {
namespace can {

//! Abstraction for the diffrent states of the interface.
enum class InterfaceState {
  //! Interface is stopped
  stopped,
  //! Interface is error active
  errorActive,
  //! Interface is error passive
  errorPassive,
  //! Interface is error warning
  errorWarning,
  //! Interface is bus off
  busOff,
  //! Interface is sleeping
  sleeping,
};

} // namespace can
} // namespace sitec
