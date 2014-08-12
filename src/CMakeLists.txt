
# Specify source files
SET(SRC ${SRC} WFGUI.cxx)
SET(SRC ${SRC} Potentials.cxx)
SET(SRC ${SRC} Field.cxx)
SET(SRC ${SRC} Carriers.cxx)
SET(SRC ${SRC} WeightfieldClass.cpp)

# Generate main root dictionary for WFGUI
SET(Dict_HEADERS ${CMAKE_CURRENT_LIST_DIR}/WFGUI.h)
SET(Dict_LINKDEF ${CMAKE_CURRENT_LIST_DIR}/LinkDef.h )
SET(Dict_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/Dict.cxx)
ROOT_GENERATE_DICTIONARY("${Dict_HEADERS}" "${Dict_LINKDEF}" "${Dict_DICTIONARY}" "${INCLUDE_DIRECTORIES}")
# Add the dictionary to the list of source files
SET(SRC ${SRC} ${Dict_DICTIONARY})

# Generate root dictionary for Potential Class
SET(PotenDict_HEADERS ${CMAKE_CURRENT_LIST_DIR}/Potentials.h)
SET(PotenDict_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/PotenDict.cxx)
ROOT_GENERATE_DICTIONARY("${PotenDict_HEADERS}" "" "${PotenDict_DICTIONARY}" "${INCLUDE_DIRECTORIES}")

# Generate root dictionary for Field Class
SET(FieldDict_HEADERS ${CMAKE_CURRENT_LIST_DIR}/Field.h)
SET(FieldDict_DICTIONARY ${CMAKE_CURRENT_BINARY_DIR}/FieldDict.cxx)
ROOT_GENERATE_DICTIONARY("${FieldDict_HEADERS}" "" "${FieldDict_DICTIONARY}" "${INCLUDE_DIRECTORIES}")



# Specify the executable
add_executable(weightfield3-bin Weightfield.cxx ${SRC})
target_link_libraries(weightfield3-bin ${LIBRARIES})
set_target_properties(weightfield3-bin PROPERTIES OUTPUT_NAME weightfield3)

# Specify the executable
add_executable(weightfield3-cli WeightfieldCLI.cpp ${SRC})
target_link_libraries(weightfield3-cli ${LIBRARIES})
set_target_properties(weightfield3-cli PROPERTIES OUTPUT_NAME weightfield3-cli)


# Build libraries
# Potential Class
add_library(PotenDict SHARED ${PotenDict_DICTIONARY})
target_link_libraries(PotenDict ${LIBRARIES})
# Field Class
add_library(FieldDict SHARED ${FieldDict_DICTIONARY})
target_link_libraries(FieldDict ${LIBRARIES})


# vim:ft=cmake


