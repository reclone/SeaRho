set (sources_list
    "src/main.cpp"
)

set (include_dirs_list "")

set (requires_list "")

idf_component_register (SRCS ${sources_list} INCLUDE_DIRS ${include_dirs_list} REQUIRES ${requires_list})

set_property (TARGET ${COMPONENT_LIB} PROPERTY CXX_STANDARD 20)
