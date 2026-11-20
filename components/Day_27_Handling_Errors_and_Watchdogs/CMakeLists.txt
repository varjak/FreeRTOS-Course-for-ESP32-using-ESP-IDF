if(CONFIG_COURSE_DAY EQUAL 27)
    idf_component_register(
            SRCS "day27.c"
            INCLUDE_DIRS "include"
            PRIV_REQUIRES driver
    )
else()
    idf_component_register()   # empty component: nothing compiled or linked
endif()