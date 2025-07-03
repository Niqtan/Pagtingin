#include "main.h"


//Move between states
void idle_state() {
    esp_wifi_stop();
    esp_bt_controller_disable();

    xTaskCreatePinnedToCore(
            write_to_tof,
            "get_measurement",
            2048,
            NULL,
            1,
            &write_handler,
            1
    );

    xTaskCreatePinnedToCore(
            read_from_tof,
            "read_measurement",
            2048,
            &distance_cm,
            1,
            &read_handler,
            1
        );
    
    vTaskDelay(pdMS_TO_TICKS(100));

    if (distance_cm <= 50) {
        current_state = STATE_ALERT;
    }
    else {
        current_state = STATE_IDLE;
    }
}

void alert_state() {
    //Handle alert behavior
    
    xTaskCreatePinnedToCore(
        i2s_write_beeps,
        "Buzzer_beeps",
        2048,
        NULL,
        1,
        &music_handler,
        1
    );
    //After:

    vTaskDelay(pdMS_TO_TICKS(100));
    current_state = STATE_MUTE;
}

void mute_state() {
    //Mute behavior: Shutdown all active tasks

    if (write_handler)  {
        vTaskDelete(write_handler);
        write_handler = NULL;
    }

    if (read_handler) {
        vTaskDelete(read_handler);
        read_handler = NULL;
    }

    if (music_handler) {
        vTaskDelete(music_handler);
        music_handler = NULL;
    }

    vTaskDelay(pdMS_TO_TICKS(100));
    //Re loop
    current_state = STATE_IDLE;
}

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
        ESP_LOGI(TAG, "GPIO Output Initiated\n");
     }
 
     // INPUT:
     io_configurations.intr_type = GPIO_INTR_DISABLE;
     io_configurations.pin_bit_mask = GPIO_INPUT_PIN_SEL;
     io_configurations.mode = GPIO_MODE_INPUT;
     io_configurations.pull_down_en = 0;
     io_configurations.pull_up_en = 0;
 
     ret = gpio_config(&io_configurations);
     if (ret == ESP_OK) {
         ESP_LOGI(TAG, "GPIO Input Initiated\n");

     }
 }

esp_err_t uart_init() {
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
    
    uart_param_config(uart_port_num, &uart_config);
    if ((ret = uart_set_pin(uart_port_num, UART_TX_PIN, UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE)) != ESP_OK) {
        ESP_LOGE(TAG, "Error occured: %s\n", esp_err_to_name(ret));
    }

    // UART Driver installation
    uart_driver_install(uart_port_num, UART_BUFFER_SIZE, UART_BUFFER_SIZE, 0, &uart_queue, EVENT_QUEUE_SIZE);

    ESP_LOGI(TAG, "UART initialized successfully!");
}

void i2c_reset() {
    gpio_set_level(GPIO_OUTPUT_IO_4, 0);
    gpio_set_level(GPIO_OUTPUT_IO_5, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(GPIO_OUTPUT_IO_4, 1);
    gpio_set_level(GPIO_OUTPUT_IO_5, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(GPIO_OUTPUT_IO_4, 0);
    gpio_set_level(GPIO_OUTPUT_IO_5, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
}
  
 void i2c_init() {
    i2c_reset();
    TAG = "I2C";
    //The bus handle is like a USB hub maanger,
    //all configurations are plugged into this hub
        //Set GPIO 02 to make I2C comms of the sensor enabled
    gpio_set_level(GPIO_OUTPUT_IO_2, 0);
    gpio_set_level(GPIO_OUTPUT_IO_18, 0);

    vTaskDelay(pdMS_TO_TICKS(100));
    
    /* TOF SENSOR 01 */
    gpio_set_level(GPIO_OUTPUT_IO_2, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    i2c_master_bus_handle_t tof_bus_handle_0;
    
    //Handler for the specific I2C device (usually a slave) on the I2C bus
    i2c_master_dev_handle_t tof_dev_handle_0;    

    // I2C  master configuration (ESP32)
    i2c_master_bus_config_t tof_mst_config_0 = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_PORT_NUM_0,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    //Used for debugging
    ret = i2c_new_master_bus(&tof_mst_config_0, &tof_bus_handle_0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "I2C Master Bus 0 did not activate! Error: %s\n", esp_err_to_name(ret));
    }
    
    i2c_device_config_t tof_dev_config_0 = {
        //Configure the slave's parameters to distinguish identity
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_TOF_DEFAULT_ADDRESS,
        .scl_speed_hz = 100000,
    };

    ret = i2c_master_bus_add_device(tof_bus_handle_0, &tof_dev_config_0, &tof_dev_handle_0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "TOF 0 Error: %s\n", esp_err_to_name(ret));
    }

    uint8_t change_address_cmd1[2] = {0x8A, I2C_TOF1_ADDRESS & 0X7F};
    i2c_master_transmit(tof_dev_handle_0, change_address_cmd1, 2, 1000);

     /* TOF SENSOR 02 */

    i2c_master_dev_handle_t tof_dev_handle_1;
    gpio_set_level(GPIO_OUTPUT_IO_18, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    i2c_device_config_t tof_dev_config_1 = {
        //Configure the slave's parameters to distinguish identity
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_TOF_DEFAULT_ADDRESS,
        .scl_speed_hz = 100000,
    };


    ret = i2c_master_bus_add_device(tof_bus_handle_0, &tof_dev_config_1, &tof_dev_handle_1);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "TOF 1 Error: %s\n", esp_err_to_name(ret));
    }

    uint8_t change_address_cmd2[2] = {0x8A, I2C_TOF2_ADDRESS & 0X7F};
    i2c_master_transmit(tof_dev_handle_0, change_address_cmd2, 2, 1000);
}

void write_to_tof(void *pvParameters) {
    TAG = "I2C";
    /* Main Purpose: Writing to turn on and off (loop this function) */ 
    uint8_t write_buffer[2];
    write_buffer[0] = 0x00;
    //Start a single measurement
    write_buffer[1] = 0x01;

    ret = i2c_master_transmit(i2c_dev, (uint8_t *)write_buffer, 2, 1000);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Slave could not receive data. Error: %s\n", esp_err_to_name(ret));
    }
    else {
        ESP_LOGI(TAG, "Slave received data");
    }

    vTaskDelay(pdMS_TO_TICKS(100));
}

uint16_t read_from_tof(void *pvParameters) {
    TAG = "I2C";
    distance_cm = (uint16_t *)pvParameters;
    /* Main Purpose: Converting and getting distance measurements */
    
    //Register for getting result of the sensor
    uint8_t read_buffer;

    //Keep looping until the the last three bits become 1
    while ((read_buffer & 0x07) == 0) {
        uint8_t reg_addr = 0x13;
            
        i2c_master_transmit(i2c_dev, &reg_addr, 1, 1000);

        i2c_master_receive(i2c_dev, &read_buffer, 1, 1000);
    }

    //Once the bits become 1, we want to get that measurement (read)
    uint8_t reg_addr = 0x1E;
    uint8_t raw_distance[2];

    i2c_master_transmit(i2c_dev, &reg_addr, 1, 1000);
    i2c_master_receive(i2c_dev, raw_distance, 2, 1000);

    //Convert to the distance to cm
    uint16_t distance_mm = (raw_distance[0] << 8) | raw_distance[1];
    distance_cm = distance_mm / 10;
    ESP_LOGI(TAG, "Distance: %dcm", distance_cm);
    return distance_cm;
}


esp_err_t i2s_init() {
    TAG = "I2S";
    
    //READY PHASE

    //I2S struct for initializing
    i2s_chan_config_t i2s_channel_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    //Allow for tx handler only since we're only transmitting audio data
    ESP_ERROR_CHECK(i2s_new_channel(&i2s_channel_cfg, &tx_handler, NULL));
    
    i2s_std_config_t i2s_cfg = {
        .slot_cfg = I2S_STD_PHILIPS_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT , I2S_SLOT_MODE_STEREO),
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(SAMPLE_RATE),
        .gpio_cfg = {
            .mclk = SCK_IO,
            .bclk = BCK_IO,
            .ws = LRCK_IO,
            .dout = DOUT_IO,
            .din = I2S_GPIO_UNUSED,
            .invert_flags = {
                .mclk_inv = false,
                .bclk_inv = false,
                .ws_inv = false,
            },
        },
    };


    //REGISTERED PHASE
    ESP_ERROR_CHECK(i2s_channel_init_std_mode(tx_handler, &i2s_cfg));
    ESP_ERROR_CHECK(i2s_channel_enable(tx_handler));
    
    gpio_set_level(GPIO_OUTPUT_IO_25, 1);
    gpio_set_level(GPIO_OUTPUT_IO_23, 1);

    return ESP_OK;
}

//Think of a new tone for the I2S

static void i2s_write_beeps() {
    /* Write bytes in order to sound beeps*/
    TAG = "I2S";

    //Buffer for storing samples
    size_t written_bytes = 0;
    int16_t NUM_SAMPLES = ((int)(SAMPLE_RATE * DURATION));

    //Stereo buffer for storing samples
    int16_t *stereo_buf = malloc(NUM_SAMPLES * 2 * sizeof(int16_t));

    int samples_a_cycle = SAMPLE_RATE / BEEP_FREQ;
        
    /*Since we're making a square wave for a harsh buffer sound,
    we would need to alternate between the highest and lowest amplitude
    */
    for (int i = 0; i < NUM_SAMPLES; i++)  {
        int amplitude = ((int)(i / samples_a_cycle) % 2 == 0) ? 30000: -30000;

        stereo_buf[2 * i] = amplitude;
        stereo_buf[2 * i + 1] = amplitude;
    }

    while (1) {
    //Write to existing channel
    beep_delay = map(distance_cm, 0, THRESHOLD, BUZZER_MIN, BUZZER_MAX);
    ESP_ERROR_CHECK(i2s_channel_write(tx_handler, &stereo_buf, sizeof(&stereo_buf), &written_bytes, 1000));
    vTaskDelay(pdMS_TO_TICKS(beep_delay));
    ESP_ERROR_CHECK(i2s_channel_write(tx_handler, &stereo_buf, sizeof(&stereo_buf), &written_bytes, 1000));
    vTaskDelay(pdMS_TO_TICKS(beep_delay));

    ESP_LOGI(TAG, "You received a beep!");
    vTaskDelay(pdMS_TO_TICKS(beep_delay));
    }
}

int map(int distance, int starting_low, int starting_high, int ending_low, int ending_high) {
    return (distance - starting_low) * (ending_high - ending_low) / (starting_high - starting_low) + ending_low;
}

void app_main(void)
{   
    //Initialize the protocols
    gpio_init();
    uart_init();
    i2c_master_init();
    i2s_init();

    while (1) {
        switch(current_state) {
            case STATE_IDLE:
                idle_state();
                break;
            case STATE_ALERT:
                alert_state();
                break;
            case STATE_MUTE:
                mute_state();
                break;    
        }
    }

}