
# Specify source files
SET(SRC ${SRC} WFGUI.cxx)
SET(SRC ${SRC} Potentials.cxx)
SET(SRC ${SRC} Field.cxx)
SET(SRC ${SRC} Carriers.cxx)

# Generate root dictionary
SET(Dict_HEADERS ${CMAKE_CURRENT_LIST_DIR}/WFGUI.h)
SET(Dict_LINKDEF ${CMAKE_CURRENT_LIST_DIR}/LinkDef.h )
SET(Dict_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/Dict.cxx) 
ROOT_GENERATE_DICTIONARY("${Dict_HEADERS}" "${Dict_LINKDEF}" "${Dict_DICTIONARY}" "${INCLUDE_DIRECTORIES}")
# Add the dictionary to the list of source files
SET(SRC ${SRC} ${Dict_DICTIONARY}) 


# Specify the executable
add_executable(weightfield3-bin Weightfield.cxx ${SRC})
target_link_libraries(weightfield3-bin ${LIBRARIES})
set_target_properties(weightfield3-bin PROPERTIES OUTPUT_NAME weightfield3)

# Build a library (TBD)
#add_library(weightfield3 ${SRC})
#target_link_libraries(weightfield3 ${LIBRARIES})

# vim:ft=cmake


