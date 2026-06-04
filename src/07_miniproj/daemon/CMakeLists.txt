cmake_minimum_required(VERSION 3.28)
project(fanctl-daemon)

add_executable(fanctl-daemon
    daemon.c
    buttons.c
    comm.c
    oled_view.c
    fanctl_sysfs.c
    ../oled/ssd1306.c
)

target_include_directories(fanctl-daemon PRIVATE ../oled)
target_compile_definitions(fanctl-daemon PRIVATE _GNU_SOURCE)
