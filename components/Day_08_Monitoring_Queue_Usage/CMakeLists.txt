set(srcs "")
if(CONFIG_COURSE_DAY EQUAL 8)
    list(APPEND srcs "day8.c")
endif()

idf_component_register(
        SRCS ${srcs}
        INCLUDE_DIRS "include"
        PRIV_REQUIRES esp_driver_gpio
)