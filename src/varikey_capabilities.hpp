// SPDX-FileCopyrightText: 2026 Roman Koch
// SPDX-License-Identifier: MIT
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>

namespace varikey
{
    struct Capabilities
    {
        uint16_t capacity;
        // Soft, hard, function, control left, control center, control right.
        std::array<uint16_t, 6> counts;
    };

    std::optional<Capabilities> decode_capabilities(const uint8_t *bytes, std::size_t size);
    // Reads a Linux hidraw node. Throws on I/O failure or an invalid reply.
    Capabilities read_usb_capabilities(const char *path);
}
