#include "main.h"

static void gpio_init(void) {
     // GPIO PINS
     gpio_config_t io_configurations = {};

     // OUTPUT:
     io_configurations.intr_type = GPIO_INTR_DISABLE;
     io_configurations.pin_bit_mask = GPIO_OUTPUT_PIN_SEL;
     io_configurations.mode = GPIO_MODE_OUTPUT;
     io_configurations.pull_down_en = 0;
     io_configurations.pull_up_en = 0;
 
     ret = gpio_config(&io_configurations);
     if (ret == ESP_OK) {
         printf("GPIO Output Initiated\n");
     } else {
         printf("GPIO Output dead\n");
     }
 
     // INPUT:
     io_configurations.intr_type = GPIO_INTR_DISABLE;
     io_configurations.pin_bit_mask = GPIO_INPUT_PIN_SEL;
     io_configurations.mode = GPIO_MODE_INPUT;
     io_configurations.pull_down_en = 0;
     io_configurations.pull_up_en = 0;
 
     ret = gpio_config(&io_configurations);
     if (ret == ESP_OK) {
         printf("GPIO Input Initiated\n");
     } else {
         printf("GPIO Input dead\n");
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

    ret = uart_param_config(uart_port_num, &uart_config);
    if (ret == ESP_OK) {
        printf("UART Channel Activated!\n");

        ret = uart_set_pin(uart_port_num, 1, 3);
        if (ret == ESP_OK) {
            printf("UART Pins Activated!\n");
        } else {
            printf("UART Pins Failed To Activate\n");
            ESP_ERROR_CHECK(ret);
        }

    } else {
        printf("UART Channel Failed To Activate...\n");
        ESP_ERROR_CHECK(ret);
    }

    // UART Driver installation
    ret = uart_driver_install(uart_port_num, UART_BUFFER_SIZE, UART_BUFFER_SIZE, 0, &uart_queue, EVENT_QUEUE_SIZE);
    if (ret == ESP_OK) {
        printf("UART Drivers Successfully Activated!\n");
    } else {
        printf("UART Drivers did not activate!\n");
        ESP_ERROR_CHECK(ret);
    }
}

static void i2c_master_init() {
    // I2C configuration
    i2c_master_bus_config_t tof_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_PORT_NUM,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    i2c_master_bus_handle_t tof_bus_handle;

    ret = i2c_new_master_bus(&tof_mst_config, &tof_bus_handle);
    if (ret == ESP_OK) {
        printf("I2C Master Initialized!\n");
    } else {
        printf("I2C Master did not activate!\n");
        ESP_ERROR_CHECK(ret);
    }

    i2c_device_config_t tof_dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x52,
        .scl_speed_hz = 100000,
    };

    i2c_master_dev_handle_t tof_dev_handle;

    ret = i2c_master_bus_add_device(tof_bus_handle, &tof_dev_config, &tof_dev_handle);
    if (ret == ESP_OK) {
        printf("I2C Master bus ok!\n");
    } else {
        printf("No I2C Master bus bruh!\n");
        ESP_ERROR_CHECK(ret);
    }
}

int map(int distance, int starting_low, int starting_high, int ending_low, int ending_high) {
    return (distance - starting_low) * (ending_high - ending_low) / (starting_high - starting_low) + ending_low;
  }

/*
Just some to do list on some functionality to build:
- Build up on the I2C communication protocol to be able to use the TOF sensor
- Bulild up on the I2S communication protocol for the audio system ( this will include the voice message now)
    - But better to research first on what type of speaker can we drive with this
- Use interrupts 
- Implement the buzzer functionality algorithm
- Build up the bluetooth
  - Make sure to test it individually first before going in and integrating it with the project in order to know how this works with any earphones

*/

void app_main(void)
{
    //Setup the pins 
    gpio_init();
    uart_init();
    i2c_master_init();


    xTaskCreate 


}