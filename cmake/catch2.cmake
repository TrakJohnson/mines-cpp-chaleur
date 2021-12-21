find_package(Catch2 QUIET)
if (${Catch2_FOUND})       
else ()
  message(STATUS "downloading Catch2")
  
  set(CATCH2_BUILD_PATH "${THIRD_PARTY_BUILD_DIR}/catch2")
  set(CATCH2_INSTALL_PATH "${THIRD_PARTY_INSTALL_DIR}/catch2")

  ExternalProject_Add(
    ep_catch2
    PREFIX ${CATCH2_BUILD_PATH}
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG "v2.13.7"
    TIMEOUT 10
    CMAKE_ARGS ${thirdparties_forward_options} -DCMAKE_INSTALL_PREFIX=${CATCH2_INSTALL_PATH}
    LOG_DOWNLOAD ON
    LOG_DOWNLOAD  ON
    LOG_UPDATE    ON
    LOG_CONFIGURE ON
    LOG_BUILD     ON
    LOG_INSTALL   ON
  )
  include_directories(${CATCH2_INSTALL_PATH}/include)
endif ()

include(CTest)
enable_testing()
