#include "can_filter.hpp"

namespace peripheral {
namespace can {

CanFilter::CanFilter(const unsigned int id, const unsigned int mask): id(id), mask(mask) {}

unsigned int
CanFilter::getId()
{
    return id;
}

unsigned int
CanFilter::getMask()
{
    return mask;
}

} // namespace can
} // namespace peripheral
