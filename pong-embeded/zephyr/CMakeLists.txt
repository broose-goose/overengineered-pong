cmake_minimum_required(VERSION 3.13.1)

include($ENV{ZEPHYR_BASE}/cmake/app/boilerplate.cmake NO_POLICY_SCOPE)
project(pong-embeded)


set (DTC_OVERLAY_FILE "nucleo_f429zi.overlay")

FILE(GLOB app_sources ../src/*.c*)
target_sources(app PRIVATE ${app_sources})
