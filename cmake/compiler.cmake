include(CheckCXXCompilerFlag)

# Set C++ standard required for this project 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Macro to check if compil flag available 
MACRO(AddCXXFlag FLAG REQUIRED)
check_cxx_compiler_flag("-${FLAG}" "${FLAG}_AVAILABLE")
if(${${FLAG}_AVAILABLE})
    set(CMAKE_CXX_FLAGS "-${FLAG} ${CMAKE_CXX_FLAGS}")
    message(STATUS "flag: '-${FLAG}' enabled")
else()
    if( ${REQUIRED} )
        message(FATAL_ERROR "flag -${FLAG} is required but seems to be not supported by your compiler")
    else()
        message(STATUS "flag: -${FLAG} disabled")
    endif( ${REQUIRED} )
endif(${${FLAG}_AVAILABLE})
ENDMACRO(AddCXXFlag)

AddCXXFlag("Wall" TRUE)
AddCXXFlag("Wextra" TRUE)
AddCXXFlag("Wpedantic" TRUE) # warn if non-standard C++ is used
AddCXXFlag("Wfatal-errors" TRUE) # stop at first error 
AddCXXFlag("Wshadow" TRUE) # warn the user if a variable declaration shadows one from a parent context
AddCXXFlag("Wold-style-cast" TRUE) # warn for c-style casts
AddCXXFlag("Wunused" TRUE) 
