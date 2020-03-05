project(libs)

add_subdirectory(irbis)
add_subdirectory(rfid)

if(WITH_SHARED_LIBRARIES)
    add_subdirectory(irbis_shared)
    add_subdirectory(rfid_shared)
endif()
