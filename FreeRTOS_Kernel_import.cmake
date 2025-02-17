if (NOT FreeRTOS_Kernel_FOUND)
    include(FetchContent)
    
    set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/FreeRTOS-Kernel CACHE STRING "Path to FreeRTOS kernel")
    
    add_library(FreeRTOS-Kernel STATIC
        ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040/port.c        ${FREERTOS_KERNEL_PATH}/tasks.c
        ${FREERTOS_KERNEL_PATH}/list.c
        ${FREERTOS_KERNEL_PATH}/queue.c
        ${FREERTOS_KERNEL_PATH}/timers.c
        ${FREERTOS_KERNEL_PATH}/event_groups.c
        ${FREERTOS_KERNEL_PATH}/stream_buffer.c
    )
    
    target_include_directories(FreeRTOS-Kernel PUBLIC
        ${FREERTOS_KERNEL_PATH}/include
        ${FREERTOS_KERNEL_PATH}/portable/ThirdParty/GCC/RP2040
    )
    
    target_link_libraries(FreeRTOS-Kernel
        pico_stdlib
    )
    
    set(FreeRTOS_Kernel_FOUND TRUE)
endif()