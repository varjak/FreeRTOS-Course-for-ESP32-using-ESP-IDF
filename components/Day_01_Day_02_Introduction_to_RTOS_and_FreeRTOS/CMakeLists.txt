set(srcs "")
if(CONFIG_COURSE_DAY EQUAL 1 OR CONFIG_COURSE_DAY EQUAL 2)
    list(APPEND srcs "day1and2.c")
endif()

idf_component_register(
        SRCS ${srcs}
        INCLUDE_DIRS "include"
        PRIV_REQUIRES esp_driver_gpio
)