include(GNUInstallDirs)


add_library(
        fam SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/AccManagement.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Accelerator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opae_svc_wrapper.cpp
)
target_link_libraries(fam ${LIB_OPAE} ${LIB_UUID} ${LIB_MPF})

add_library(
        fam-ase SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/AccManagement.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Accelerator.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/opae_svc_wrapper.cpp
)
target_link_libraries(fam-ase ${LIB_OPAE_ASE} ${LIB_UUID} ${LIB_MPF})

install(TARGETS fam
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/fam
        )

install(TARGETS fam-ase
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/lib
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_PREFIX}/include/fam
        )

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/Accelerator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/AccManagementDEFS.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/AccManagement.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/csr_mgr.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/opae_svc_wrapper.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/fam
        )
