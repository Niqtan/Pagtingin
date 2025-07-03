idf_component_register(SRCS "main.c"
                       INCLUDE_DIRS "."
                       REQUIRES bt driver esp_timer mqtt esp_wifi
                       PRIV_REQUIRES bt driver esp_timer mqtt esp_wifi)
