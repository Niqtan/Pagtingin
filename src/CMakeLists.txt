add_executable(main main.c)

pico_enable_stdio_usb(main 1)
pico_enable_stdio_uart(main 0)

target_include_directories(main PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_SOURCE_DIR}/FreeRTOS-Kernel/include
    ${CMAKE_SOURCE_DIR}/FreeRTOS-Kernel/portable/GCC/ARM_CM0
)

target_sources(main PRIVATE main.c)

target_link_libraries(main pico_stdlib hardware_timer hardware_irq hardware_pwm hardware_sync pico_multicore freertos_kernel)


#Creates a map/bin/hex/uf2 in addition to the ELF file (executables)
pico_add_extra_outputs(main)

