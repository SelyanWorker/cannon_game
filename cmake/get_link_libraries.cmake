function(get_link_libraries OUTPUT_LIST TARGET)
    get_target_property(IMPORTED ${TARGET} IMPORTED)
    get_target_property(TYPE ${TARGET} TYPE)
    list(APPEND VISITED_TARGETS ${TARGET})
    if (IMPORTED OR TYPE STREQUAL "INTERFACE_LIBRARY")
        get_target_property(LIBS ${TARGET} INTERFACE_LINK_LIBRARIES)
    else ()
        get_target_property(LIBS ${TARGET} LINK_LIBRARIES)
    endif ()
    set(LIB_FILES "")
    foreach(LIB ${LIBS})
        if (TARGET ${LIB})
            list(FIND VISITED_TARGETS ${LIB} VISITED)
            if (${VISITED} EQUAL -1)
                #get_target_property(LIB_FILE ${LIB} LOCATION)
                get_link_libraries(LINK_LIB_FILES ${LIB})

                # TODO: append only shared libs
                list(APPEND LIB_FILES ${LIB} ${LINK_LIB_FILES})
            endif()
        endif()
    endforeach()
    set(VISITED_TARGETS ${VISITED_TARGETS} PARENT_SCOPE)
    set(${OUTPUT_LIST} ${LIB_FILES} PARENT_SCOPE)
endfunction()

function(deploy_dlls TARGET)
    get_link_libraries(TARGET_libs ${TARGET})

    foreach(lib ${TARGET_libs})
        get_target_property(TYPE ${lib} TYPE)
        if (${TYPE} STREQUAL "SHARED_LIBRARY")
            add_custom_command(TARGET ${TARGET} PRE_LINK
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:${lib}>
                    $<TARGET_FILE_DIR:${TARGET}>
                    )
        endif()
    endforeach()
    if (WIN32)
        if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
            find_file(libgcc NAMES libgcc_s_dw2-1.dll libgcc_s_seh-1.dll)
            find_file(libstdc++-6 libstdc++-6.dll)
            find_file(libwinpthread-1 libwinpthread-1.dll)

            add_custom_command(TARGET ${TARGET} PRE_LINK
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${libgcc} $<TARGET_FILE_DIR:${TARGET}>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${libstdc++-6} $<TARGET_FILE_DIR:${TARGET}>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${libwinpthread-1} $<TARGET_FILE_DIR:${TARGET}>
                    COMMENT "Copying MinGW32 dependent libraries"
                    )

        endif()
    endif()

endfunction()