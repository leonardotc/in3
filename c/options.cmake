
# options
option(BUILD_DOC "generates the documenation with doxygen." OFF)
option(TAG_VERSION "the tagged version, which should be used" 3.0.0)
option(ETH_NANO "build minimal eth verification.(eth_getTransactionReceipt)" ON)
option(ETH_BASIC "build basic eth verification.(all rpc-calls except eth_call)" ON)
option(ETH_FULL "build full eth verification.(including eth_call)" ON)
option(IPFS "build IPFS verification" ON)
option(COLOR "Enable color codes for debug" ON)
option(BTC "if true, the bitcoin verifiers will be build" ON)
option(IN3API "build the USN-API which offer better interfaces and additional functions on top of the pure verification" ON)
option(USE_PRECOMPUTED_EC "if true the secp256k1 curve uses precompiled tables to boost performance. turning this off makes ecrecover slower, but saves about 37kb." ON)
option(LOGGING "if set logging and human readable error messages will be inculded in th executable, otherwise only the error code is used. (saves about 19kB)" ON)
option(EVM_GAS "if true the gas costs are verified when validating a eth_call. This is a optimization since most calls are only interessted in the result. EVM_GAS would be required if the contract uses gas-dependend op-codes." true)
option(IN3_LIB "if true a shared anmd static library with all in3-modules will be build." ON)
option(TEST "builds the tests and also adds special memory-management, which detects memory leaks, but will cause slower performance" OFF)
option(FAST_MATH "Math optimizations used in the EVM. This will also increase the filesize." OFF)
option(SEGGER_RTT "Use the segger real time transfer terminal as the logging mechanism" OFF)
option(CURL_BLOCKING "if true the curl-request will block until the response is received" OFF)
option(JAVA "build the java-binding (shared-lib and jar-file)" OFF)
option(JAVA_MULTI_LIBS "embedds multiple shared libs in the jar" OFF)
option(WASM "Includes the WASM-Build. In order to build it you need emscripten as toolchain. Usually you also want to turn off other builds in this case." OFF)
option(ASMJS "compiles the code as asm.js." OFF)
option(WASM_EMBED "embedds the wasm as base64-encoded into the js-file" ON)
option(WASM_EMMALLOC "use ther smaller EMSCRIPTEN Malloc, which reduces the size about 10k, but may be a bit slower" ON)
option(WASM_SYNC "intiaializes the WASM synchronisly, which allows to require and use it the same function, but this will not be supported by chrome (4k limit)" OFF)
option(CODE_COVERAGE  "Builds targets with code coverage instrumentation. (Requires GCC or Clang)"  OFF)
option(GCC_ANALYZER "GCC10 static code analyses" OFF)
option(PAY_ETH  "support for direct Eth-Payment" OFF)
option(USE_SCRYPT "integrate scrypt into the build in order to allow decrypt_key for scrypt encoded keys." ON)
option(USE_CURL "if true the curl transport will be built (with a dependency to libcurl)" ON)
option(USE_WINHTTP "if true the winhttp transport will be built (with a dependency to winhttp)" OFF)
option(LEDGER_NANO "include support for nano ledger" OFF)
option(ESP_IDF "include support for ESP-IDF microcontroller framework" OFF)
option(ASSERTIONS "includes assertions into the code, which help track errors but may cost time during runtime" OFF)
OPTION(TRANSPORTS "builds transports, which may require extra libraries." ON)
OPTION(IN3_SERVER "support for proxy server as part of the cmd-tool, which allows to start the cmd-tool with the -p option and listens to the given port for rpc-requests" OFF)
OPTION(CMD "build the comandline utils" ON)
OPTION(RECORDER "enable recording option for reproduce executions" ON)
OPTION(POA "support POA verification including validatorlist updates" OFF)
OPTION(MULTISIG "add capapbility to sign with a multig. Currrently only gnosis safe is supported" ON)
OPTION(ZKSYNC "add RPC-function to handle zksync-payments" ON)
OPTION(ZKCRYPTO_LIB "Path to the static zkcrypto-lib" OFF)
OPTION(SENTRY "Enable Sentry" OFF)
OPTION(BTC_PRE_BPI34 "Enable BTC-Verfification for blocks before BIP34 was activated" ON)
OPTION(PK_SIGNER "Enable Signing with private keys" ON)
OPTION(NODESELECT_DEF "Enable default nodeselect implementation" ON)
OPTION(NODESELECT_DEF_WL "Enable default nodeselect whitelist implementation" ON)
OPTION(PLGN_CLIENT_DATA "Enable client-data plugin" OFF)
OPTION(THREADSAFE "uses mutex to protect shared nodelist access" ON)
OPTION(SWIFT "swift API for swift bindings" OFF)


IF (DEFINED ANDROID_ABI)
  set(TRANSPORTS,false)
  set(IN3_LIB,false)
  set(USE_CURL,off)
  set(CMD,false)
  set(JAVA,true)
  set(RECORDER,false)
ENDIF()
IF (SWIFT)
  ADD_DEFINITIONS(-DSWIFT)
  set(TRANSPORTS,false)
  set(IN3_LIB,true)
  set(USE_CURL, false)
  set(CMD,false)
  set(JAVA,false)
  set(RECORDER,false)
ENDIF()

IF (BTC_PRE_BPI34)
  ADD_DEFINITIONS(-DBTC_PRE_BPI34)
ENDIF (BTC_PRE_BPI34)

IF (POA)
  ADD_DEFINITIONS(-DPOA)
ENDIF (POA)

IF (PK_SIGNER)
  ADD_DEFINITIONS(-DPK_SIGNER)
  set(IN3_API ${IN3_API} pk_signer)
ENDIF (PK_SIGNER)

if (USE_PRECOMPUTED_EC)
  ADD_DEFINITIONS(-DUSE_PRECOMPUTED_CP=1)
else()
  ADD_DEFINITIONS(-DUSE_PRECOMPUTED_CP=0)
endif()

if (LOGGING)
  ADD_DEFINITIONS(-DLOGGING)
endif()

if (MULTISIG)
  ADD_DEFINITIONS(-DMULTISIG)
endif()

if (ZKSYNC)
  ADD_DEFINITIONS(-DZKSYNC)
  set(WASM_MODULES ${WASM_MODULES} zksync)
  set(IN3_API ${IN3_API} zksync)
endif()


if(ETH_FULL)
    ADD_DEFINITIONS(-DETH_FULL)
    set(IN3_VERIFIER eth_full)
    set(ETH_BASIC true)
    set(ETH_NANO true)
elseif(ETH_BASIC)
    ADD_DEFINITIONS(-DETH_BASIC)
    set(IN3_VERIFIER eth_basic)
    set(ETH_NANO true)
elseif(ETH_NANO)
    ADD_DEFINITIONS(-DETH_NANO)
    set(IN3_VERIFIER eth_nano)
endif()

if (ETH_NANO)
  set(WASM_MODULES ${WASM_MODULES} eth)
endif()

if(IN3API)
    ADD_DEFINITIONS(-DETH_API)
    set(IN3_API ${IN3_API} eth_api)
endif()

if (ESP_IDF)
    ADD_DEFINITIONS(-DESP_IDF)
endif()

if(PAY_ETH)
  ADD_DEFINITIONS(-DPAY_ETH -DPAY)
  set(IN3_API ${IN3_API} pay_eth)
endif()

if(IPFS)
    ADD_DEFINITIONS(-DIPFS)
    set(IN3_VERIFIER ${IN3_VERIFIER} ipfs)
    set(WASM_MODULES ${WASM_MODULES} ipfs)

    if(IN3API)
       set(IN3_API ${IN3_API} ipfs_api)
    endif()
endif()

if(BTC)
    ADD_DEFINITIONS(-DBTC)
    set(IN3_VERIFIER ${IN3_VERIFIER} btc)
    set(WASM_MODULES ${WASM_MODULES} btc)
    if(IN3API)
       set(IN3_API ${IN3_API} btc_api)
    endif()
endif()

if(LEDGER_NANO)
    add_definitions(-DLEDGER_NANO)
endif()

if(COLOR AND NOT (MSVC OR MSYS OR MINGW))
   ADD_DEFINITIONS(-DLOG_USE_COLOR)
endif()


if(CMAKE_BUILD_TYPE MATCHES Debug)
    ADD_DEFINITIONS(-DDEBUG)
endif(CMAKE_BUILD_TYPE MATCHES Debug)

if(EVM_GAS)
    ADD_DEFINITIONS(-DEVM_GAS)
endif(EVM_GAS)

if(FAST_MATH)
    ADD_DEFINITIONS(-DIN3_MATH_FAST)
else()
    ADD_DEFINITIONS(-DIN3_MATH_LITE)
endif(FAST_MATH)

if(SEGGER_RTT)
    ADD_DEFINITIONS(-DSEGGER_RTT)
endif(SEGGER_RTT)

if(RECORDER)
    ADD_DEFINITIONS(-DRECORDER)
endif(RECORDER)

if (SENTRY)
  ADD_DEFINITIONS(-DSENTRY)
  set(IN3_API ${IN3_API} in3_sentry)
endif()

if (NODESELECT_DEF)
  ADD_DEFINITIONS(-DNODESELECT_DEF)
  if (NODESELECT_DEF_WL)
    ADD_DEFINITIONS(-DNODESELECT_DEF_WL)
  endif()
  set(IN3_NODESELECT ${IN3_NODESELECT} nodeselect_def)
endif()

# handle version
if (TAG_VERSION)
   set(PROJECT_VERSION "${TAG_VERSION}")
else(TAG_VERSION)
   set(PROJECT_VERSION "3.0.0-local")
endif(TAG_VERSION)

MESSAGE(STATUS "Building version ${PROJECT_VERSION}")

string(REPLACE "." ";" VERSION_LIST ${PROJECT_VERSION})
list(GET VERSION_LIST 0 PROJECT_VERSION_MAJOR)
list(GET VERSION_LIST 1 PROJECT_VERSION_MINOR)
list(GET VERSION_LIST 2 PROJECT_VERSION_PATCH)

ADD_DEFINITIONS("-DIN3_VERSION=\"${PROJECT_VERSION}\"")
ADD_DEFINITIONS(-DIN3_VERSION_MAJOR=${PROJECT_VERSION_MINOR})
ADD_DEFINITIONS(-DIN3_VERSION_MINOR=${PROJECT_VERSION_MINOR})
ADD_DEFINITIONS(-DIN3_VERSION_PATCH=${PROJECT_VERSION_PATCH})


# define output dir structure
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


IF (WASM)
  set(TEST false)
  set(RECORDER false)
  set(TRANSPORTS false)
  set(BUILD_DOC false)
  set(IN3_LIB false)
  set(CMD false)
  set(USE_CURL false)
  set(USE_WINHTTP false)
  set(THREADSAFE false)
  ADD_DEFINITIONS(-DWASM)
ENDIF (WASM)

if (THREADSAFE)
   ADD_DEFINITIONS(-DTHREADSAFE)
ENDIF()


# build tests
if(TEST)
    ADD_DEFINITIONS(-DTEST)
    ADD_DEFINITIONS(-DIN3_EXPORT_TEST=)
    ADD_DEFINITIONS(-DIN3_IMPORT_TEST=extern)
    ADD_DEFINITIONS(-DDEBUG)
    SET(CMAKE_BUILD_TYPE Debug)
    enable_testing()
    add_custom_target(ptest COMMAND ${CMAKE_CTEST_COMMAND} -j 8)
    add_custom_target(rtest COMMAND ${CMAKE_CTEST_COMMAND} -V )
else(TEST)
    ADD_DEFINITIONS(-DIN3_EXPORT_TEST=static)
    ADD_DEFINITIONS(-DIN3_IMPORT_TEST=)
endif(TEST)
