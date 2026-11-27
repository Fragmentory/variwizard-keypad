// SPDX-FileCopyrightText: 2026 Roman Koch
// SPDX-License-Identifier: MIT

#include <varikey_gadget_usb.hpp>

int main()
{
    varikey::gadget::usb device;
    return device.is_open() ? 1 : 0;
}
