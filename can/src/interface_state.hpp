#pragma once

namespace peripheral {
namespace can {

enum class InterfaceState {
    stopped,
    errorActive,
    errorPassive,
    errorWarning,
    busOff,
    sleeping,
};

}
} // namespace periperhal
