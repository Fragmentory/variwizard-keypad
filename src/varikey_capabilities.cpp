// SPDX-FileCopyrightText: 2026 Roman Koch
// SPDX-License-Identifier: MIT
#include "varikey_capabilities.hpp"

#include <cerrno>
#include <fcntl.h>
#include <linux/hidraw.h>
#include <numeric>
#include <stdexcept>
#include <system_error>
#include <sys/ioctl.h>
#include <unistd.h>

namespace varikey
{
    static uint16_t read_word(const uint8_t *bytes)
    {
        return bytes[0] | (static_cast<uint16_t>(bytes[1]) << 8);
    }

    std::optional<Capabilities> decode_capabilities(const uint8_t *bytes, std::size_t size)
    {
        if (!bytes || size != 15 || bytes[0] != 1)
            return std::nullopt;
        Capabilities caps{read_word(bytes + 1), {}};
        for (std::size_t i = 0; i < caps.counts.size(); ++i)
            caps.counts[i] = read_word(bytes + 3 + 2 * i);
        if (std::accumulate(caps.counts.begin(), caps.counts.end(), uint32_t{0}) > caps.capacity)
            return std::nullopt;
        return caps;
    }

    Capabilities read_usb_capabilities(const char *path)
    {
        const int fd = open(path, O_RDONLY | O_CLOEXEC);
        if (fd < 0)
            throw std::system_error(errno, std::generic_category(), path);
        constexpr uint8_t report_id = 0xe1;
        std::array<uint8_t, 16> report{report_id};
        const int size = ioctl(fd, HIDIOCGFEATURE(report.size()), report.data());
        const int error = errno;
        close(fd);
        if (size < 0)
            throw std::system_error(error, std::generic_category(), "HID feature report 0xe1");
        if (size != static_cast<int>(report.size()) || report[0] != report_id)
            throw std::runtime_error("invalid keypad feature report");
        auto caps = decode_capabilities(report.data() + 1, report.size() - 1);
        if (!caps)
            throw std::runtime_error("invalid keypad capabilities");
        return *caps;
    }
}
