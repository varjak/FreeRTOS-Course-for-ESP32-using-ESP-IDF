set(srcs "")
if(CONFIG_COURSE_DAY EQUAL 12)
    list(APPEND srcs "day12.c")
endif()

idf_component_register(
        SRCS ${srcs}
        INCLUDE_DIRS "include"
        PRIV_REQUIRES esp_driver_gpio
)