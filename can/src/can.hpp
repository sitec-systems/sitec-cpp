#pragma once

#include <string>
#include <vector>

#include "can_filter.hpp"
#include "can_frame.hpp"

namespace peripheral {
namespace can {

class Can
{
public:
    Can(const char* networkInterface);
    ~Can();
    void openInterface();
    void setFilter(const std::vector<CanFilter>& filter);
    void setFilter(const CanFilter& filter);
    void disableFilter();
    CanFrame receiveFrame();
    void sendFrame(CanFrame& frame);
    void closeInterface();

private:
    void configureInterface();

    std::string networkInterface; 
    unsigned int bitrate;
    int sock = 0;
};

} // namespace can
} // namespace peripheral
