set (sources_list
    "src/main.cpp"
)

set (includes_list "")

idf_component_register(SRCS ${sources_list} INCLUDE_DIRS ${includes_list})

set_property(TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)
