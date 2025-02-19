add_executable(main main.c)

target_sources(main PRIVATE main.c)

target_link_libraries(main pico_stdlib hardware_timer hardware_irq hardware_pwm hardware_gpio)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

#Creates a map/bin/hex/uf2 in addition to the ELF file (executables)
pico_add_extra_outputs(main)

