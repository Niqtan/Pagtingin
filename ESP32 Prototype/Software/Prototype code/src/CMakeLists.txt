idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "."
                       REQUIRES bt esp_driver_gpio esp_timer mqtt esp_wifi
                       PRIV_REQUIRES bt esp_driver_gpio esp_timer mqtt esp_wifi)
