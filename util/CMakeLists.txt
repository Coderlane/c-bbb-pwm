file(GLOB UTIL_SOURCE_FILES "*.c")

foreach(CURRENT_UTIL_SOURCE_FILE ${UTIL_SOURCE_FILES})
	get_filename_component(CURRENT_UTIL_BINARY ${CURRENT_UTIL_SOURCE_FILE} NAME_WE)
	#string(REPLACE "test_" "" CURRENT_UTIL ${CURRENT_UTIL_BINARY})
	
	add_executable(${CURRENT_UTIL_BINARY} ${CURRENT_UTIL_SOURCE_FILE})
	target_include_directories(${CURRENT_UTIL_BINARY} PUBLIC ${LIBBBBPWM_INCLUDEDIR})
	target_link_libraries(${CURRENT_UTIL_BINARY} ${LIBBBBPWM_LIBRARY})
	#add_test(${CURRENT_UTIL} ${CURRENT_UTIL_BINARY})
endforeach()
