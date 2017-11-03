// Copyright 2017 sitec systems GmbH. All rights reserved
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include "can_filter.hpp"

namespace sitec {
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
} // namespace sitec
