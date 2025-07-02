#include "main.h"

static void gpio_init(void) {

    TAG = "GPIO";
     // GPIO PINS
     gpio_config_t io_configurations = {};

     // OUTPUT:
     io_configurations.intr_type = GPIO_INTR_DISABLE;
     io_configurations.pin_bit_mask = (1ULL << GPIO_OUTPUT_IO_23);
     io_configurations.mode = GPIO_MODE_OUTPUT;
     io_configurations.pull_down_en = 0;
     io_configurations.pull_up_en = 0;
 
     ret = gpio_config(&io_configurations);
     if (ret != ESP_OK) {
         ESP_LOGE(TAG, "GPIO Output Error: %s", esp_err_to_name(ret));
     }
 }

  static void uart_init() {
    TAG = "UART";
    // UART Configuration
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    
    uart_param_config(UART_NUM_0, &uart_config);
    uart_set_pin(UART_NUM_0, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

    // UART Driver installation
    if ((ret = (uart_driver_install(UART_NUM_0, UART_TX_SIZE, 0, 0, NULL, 0)) != ESP_OK)) {
        ESP_LOGE(TAG, "Error occured: %s\n", esp_err_to_name(ret));
    }
    ESP_LOGI(TAG, "UART initialized successfully!");
}

void i2c_init() {
    TAG = "I2C";
    // I2C  master configuration (ESP32)
    i2c_master_bus_config_t tof_mst_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_PORT_NUM,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    //Used for debugging
    ret = i2c_new_master_bus(&tof_mst_config, &tof_bus_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C Master did not activate! Error: %s\n", esp_err_to_name(ret));
    }

    i2c_device_config_t tof_dev_config = {
        //Configure the slave's parameters to distinguish identity
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_TOF_ADDRESS,
        .scl_speed_hz = 100000,
    };

    ret = i2c_master_bus_add_device(tof_bus_handle, &tof_dev_config, &tof_dev_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "No I2C Master! Error: %s\n", esp_err_to_name(ret));
    }
}

void write_to_tof(void *pvParameters) {
    while (1) {
        TAG = "I2C";
        /* Main Purpose: Writing to turn on and off (loop this function) */ 
        uint8_t write_buffer[2];
        write_buffer[0] = 0x00;
        //Start a single measurement
        write_buffer[1] = 0x01;

        ret = i2c_master_transmit(tof_dev_handle, (uint8_t *)write_buffer, 2, 1000);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Slave could not receive data. Error: %s\n", esp_err_to_name(ret));
        }
        else {
            ESP_LOGI(TAG, "Slave received data");
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void read_from_tof(void *pvParameters) {
    while (1) {
        TAG = "I2C";
        /* Main Purpose: Converting and getting distance measurements */
        
        //Register for getting result of the sensor
        uint8_t read_buffer;

        //Keep looping until the the last three bits become 1
        while ((read_buffer & 0x07) == 0) {
            uint8_t reg_addr = 0x13;
                
            i2c_master_transmit(tof_dev_handle, &reg_addr, 1, 1000);

            i2c_master_receive(tof_dev_handle, &read_buffer, 1, 1000);
        }

        //Once the bits become 1, we want to get that measurement (read)
        uint8_t reg_addr = 0x13;
        uint8_t raw_distance[2];

        i2c_master_transmit(tof_dev_handle, &reg_addr, 1, 1000);
        i2c_master_receive(tof_dev_handle, raw_distance, 2, 1000);

        //Convert to the distance to cm
        uint16_t distance_mm = (raw_distance[0] << 8) | raw_distance[1];
        distance_cm = distance_mm / 10;
        ESP_LOGI(TAG, "Distance: %dcm", distance_cm);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    //Initialization
    uart_init();
    gpio_init();
    i2c_init();

    /* Free RTOS Notes
    Unlike vanilla FreeRTOS,
    FreeRTOS in the ESP-IDF must never call vTaskStartScheduler or vTaskEndScheduler

    */

    TaskHandle_t handler = NULL;
    

    xTaskCreatePinnedToCore(
            write_to_tof,
            "get_measurement",
            2048,
            NULL,
            1,
            &handler,
            1
        );

    xTaskCreatePinnedToCore(
            read_from_tof,
            "read_measurement",
            2048,
            NULL,
            1,
            NULL,
            1
        );


}