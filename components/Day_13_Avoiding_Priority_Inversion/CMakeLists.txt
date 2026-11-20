set(srcs "")
if(CONFIG_COURSE_DAY EQUAL 13)
    list(APPEND srcs "day13.c")
endif()

idf_component_register(
        SRCS ${srcs}
        INCLUDE_DIRS "include"
        PRIV_REQUIRES esp_driver_gpio
)