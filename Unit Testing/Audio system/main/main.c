#include "main.h"

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
            .mclk = I2S_GPIO_UNUSED,
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

    return ESP_OK;
}

void i2s_write_beeps(void *param) {
    /* Write bytes in order to sound beeps*/

    TAG = "I2S";

    //Buffer for storing samples
    size_t written_bytes = 0;
    int16_t NUM_SAMPLES = ((int)(SAMPLE_RATE * DURATION));

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

    if (stereo_buf == NULL) {
    ESP_LOGE(TAG, "Buffer allocation failed!");
    return;
    }

    while (1) {
    //Write to existing channel
    ESP_ERROR_CHECK(i2s_channel_write(tx_handler, stereo_buf, NUM_SAMPLES * 2 * sizeof(int16_t), &written_bytes, portMAX_DELAY));
    
    ESP_LOGI(TAG, "Wrote %d bytes to I2S", written_bytes);        
    ESP_LOGI(TAG, "You received a beep!");
    }
}

void app_main(void)
{

    TAG = "MALLOC";

    //Initialization
    uart_init();

    TaskHandle_t music_handler = NULL;
    if (i2s_init() == ESP_OK) {
        xTaskCreatePinnedToCore(
            i2s_write_beeps,
            "Buzzer_beeps",
            4096,
            NULL,
            1,
            &music_handler,
            1
        );
    }

    
}