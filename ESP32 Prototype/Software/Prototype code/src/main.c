#include "main.h"

esp_err_t gpio_init(void) {
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

 esp_err_t uart_init() {
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

 esp_err_t mqtt_start(void) {
    esp_mqtt_client_config_t mqtt_cfg = {
        //Address of the broker (localhost I think)
        .broker.address.url = 
    }
 }

 esp_err_t i2c_master_init() {
    // I2C configuration
    i2c_master_bus_config_t tof_mst_config = {
        //Configuration parameters of the ESP32
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
        //Configuration parameters of the Time of Flight Sensor
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

esp_err_t mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {

}

esp_err_t mqtt_init(void) {
    const mqtt_client_config_t mqtt_cfg = {
        .broker = {
            .broker.address.url =
        };

    }
}

esp_err_t wifi_event_handler


esp_err_t connect_to_wifi(void) {

    /* INTIAILIZE */
    ESP_ERROR_CHECK(esp_netif_init());
    
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    //EVENT Loop
    wifi_event_group = xEventGroupCreate();

    esp_event_handler_instance_t wifi_handler_event_instance;
    ESP_ERROR_CHECK(esp_event_handler_instance_register())

    //Configure wifi again
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "xxx",
            .password = "yyyy",
            .threshold.authmode = WIFI_AUTH HERE,
            .pmf_cfg = {
                .capable = true,
                .required = false   
            },
        },
    };


}

int map(int distance, int starting_low, int starting_high, int ending_low, int ending_high) {
    return (distance - starting_low) * (ending_high - ending_low) / (starting_high - starting_low) + ending_low;
  }

void app_main(void)
{
    //Setup the pins 
    gpio_init();
    uart_init();
    i2c_master_init();

    ESP_ERROR_CHECK(example)

    mqtt_init();

}