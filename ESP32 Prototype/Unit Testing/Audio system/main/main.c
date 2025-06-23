#include "main.h"

static void gpio_init(void) {
     // GPIO PINS
     gpio_config_t io_configurations = {};

     // OUTPUT:
     io_configurations.intr_type = GPIO_INTR_DISABLE;
     io_configurations.pin_bit_mask = (1ULL << GPIO_OUTPUT_IO_22) | (1ULL << GPIO_OUTPUT_IO_23);
     io_configurations.mode = GPIO_MODE_OUTPUT;
     io_configurations.pull_down_en = 0;
     io_configurations.pull_up_en = 0;
 
     ret = gpio_config(&io_configurations);
     if (ret != ESP_OK) {
         printf("GPIO Output Error: %s", esp_err_to_name(ret));
     }
 }

 static void uart_init() {
    // UART Configuration
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    
    // UART Driver installation
    uart_driver_install(uart_port_num, UART_BUFFER_SIZE, UART_BUFFER_SIZE, 0, &uart_queue, EVENT_QUEUE_SIZE);
    uart_param_config(uart_port_num, &uart_config);
    if ((ret = uart_set_pin(uart_port_num, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE)) != ESP_OK) {
        printf("Error occured: %s\n", esp_err_to_name(ret));
    }
}

static void i2s_init() {
    
}


void app_main(void)
{
    //Initialization
    uart_init();
    gpio_init();
    i2s_init();

        xTaskCreatePinnedToCore(
        )

}