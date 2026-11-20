set(srcs "")
if(CONFIG_COURSE_DAY EQUAL 21)
    list(APPEND srcs "day21.c")
endif()

idf_component_register(
        SRCS ${srcs}
        INCLUDE_DIRS "include"
        PRIV_REQUIRES esp_driver_gpio
)