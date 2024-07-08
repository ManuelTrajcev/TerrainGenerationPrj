#!/bin/bash

CC=g++
CPPFLAGS="-I../VulkanCore/Include -I../../Include -DVULKAN -ggdb3"
LDFLAGS=`pkg-config --libs glfw3 vulkan`
LDFLAGS="$LDFLAGS"

$CC tutorial10.cpp \
    ../VulkanCore/Source/core.cpp \
    ../VulkanCore/Source/util.cpp \
    ../VulkanCore/Source/device.cpp \
    ../VulkanCore/Source/queue.cpp \
    ../VulkanCore/Source/wrapper.cpp \
    ../../Common/ogldev_util.cpp  $CPPFLAGS $LDFLAGS -o tutorial10
