#pragma once

namespace peripheral {
namespace can {
class CanFilter {
public:
    CanFilter(const unsigned int id, const unsigned int mask);
    unsigned int getId();
    unsigned int getMask();

private:
    unsigned int id;
    unsigned int mask;
};

} // namespace can
} // namespace peripheral
