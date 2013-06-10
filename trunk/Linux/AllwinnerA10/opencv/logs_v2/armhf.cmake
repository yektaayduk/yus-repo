set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(GCC_COMPILER_VERSION "4.6" CACHE STRING "GCC Compiler version")

set(FLOAT_ABI_SUFFIX "hf")

set(CMAKE_C_COMPILER    arm-linux-gnueabi${FLOAT_ABI_SUFFIX}-gcc-${GCC_COMPILER_VERSION})
set(CMAKE_CXX_COMPILER  arm-linux-gnueabi${FLOAT_ABI_SUFFIX}-g++-${GCC_COMPILER_VERSION})
set(ARM_LINUX_SYSROOT /usr CACHE PATH "ARM cross compilation system root")

set(CMAKE_CXX_FLAGS           ""                    CACHE STRING "c++ flags")
set(CMAKE_C_FLAGS             ""                    CACHE STRING "c flags")
set(CMAKE_SHARED_LINKER_FLAGS ""                    CACHE STRING "shared linker flags")
set(CMAKE_MODULE_LINKER_FLAGS ""                    CACHE STRING "module linker flags")
set(CMAKE_EXE_LINKER_FLAGS    ""                    CACHE STRING "executable linker flags")

if(USE_NEON)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=neon")
elseif(USE_VFPV3)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=vfpv3")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=vfpv3")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=vfpv3-d16")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=vfpv3-d16")
endif()


