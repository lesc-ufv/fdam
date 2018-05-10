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

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/fam/Accelerator.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/fam/AccManagementDEFS.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/fam/AccManagement.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/fam/csr_mgr.h
        ${CMAKE_CURRENT_SOURCE_DIR}/include/fam/opae_svc_wrapper.h
        DESTINATION ${CMAKE_INSTALL_PREFIX}/include/fam
        )
