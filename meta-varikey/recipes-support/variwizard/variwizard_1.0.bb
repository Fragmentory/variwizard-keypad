# SPDX-FileCopyrightText: 2026 Roman Koch
# SPDX-License-Identifier: MIT

SUMMARY = "VariKey Linux USB host library and wizard CLI"
HOMEPAGE = "https://github.com/Fragmentory/variwizard-keypad"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=0f9f8afaf35271462765e2047cf1400f"

# This layer ships with its sources. Pin the enclosing repository in the distro.
FILESEXTRAPATHS:prepend := "${THISDIR}/../../..:"
SRC_URI = "file://CMakeLists.txt;subdir=${BP} \
           file://LICENSE;subdir=${BP} \
           file://src;subdir=${BP}"
S = "${WORKDIR}/${BP}"

inherit cmake

EXTRA_OECMAKE = "-DBUILD_SHARED_LIBS=ON -DBUILD_TESTING=OFF"

PACKAGES =+ "libvarikey-host"
FILES:libvarikey-host = "${libdir}/libvarikey-host.so.*"
