// SPDX-FileCopyrightText: 2026 Roman Koch
// SPDX-License-Identifier: MIT
#include "varikey_capabilities.hpp"
#include <algorithm>
#include <array>
#include <cstdarg>
#include <linux/hidraw.h>
#include <stdexcept>
#include <sys/ioctl.h>

static std::array<uint8_t, 16> report{0xe1,1,15,0,5,0,0,0,2,0,1,0,4,0,3,0};
static int returned_size = report.size();

// Exercise the installed Linux ioctl ABI without USB hardware.
extern "C" int ioctl(int, unsigned long request, ...) noexcept
{
    va_list args;
    va_start(args, request);
    auto *buffer = va_arg(args, uint8_t *);
    va_end(args);
    if (request != HIDIOCGFEATURE(report.size()) || buffer[0] != 0xe1)
        return -1;
    std::copy(report.begin(), report.end(), buffer);
    return returned_size;
}

int main()
{
    std::array<unsigned char, 15> sample{1,15,0,5,0,0,0,2,0,1,0,4,0,3,0};
    auto decode = [&](size_t size) { return varikey::decode_capabilities(sample.data(), size); };
    auto caps = decode(sample.size());
    if (!caps || caps->capacity != 15 || caps->counts != std::array<uint16_t,6>{5,0,2,1,4,3})
        return 1;
    if (decode(14) || decode(16))
        return 2;
    sample[0] = 2;
    if (decode(15))
        return 3;
    sample[0] = 1;
    sample[3] = 6;
    if (decode(15))
        return 4;
    sample[2] = 1;
    caps = decode(15);
    if (!caps || caps->capacity != 271)
        return 5;
    auto usb = varikey::read_usb_capabilities("/dev/null");
    if (usb.capacity != 15 || usb.counts[5] != 3)
        return 6;
    for (int test = 0; test < 3; ++test)
    {
        returned_size = test == 0 ? 15 : 16;
        report[0] = test == 1 ? 0xe0 : 0xe1;
        report[1] = test == 2 ? 2 : 1;
        try
        {
            varikey::read_usb_capabilities("/dev/null");
            return 7;
        }
        catch (const std::runtime_error &) {}
    }
    return 0;
}
