set (SOURCES
  callback_manager.cpp
  connection.cpp
  connection_impl.cpp
  default_http_client.cpp
  default_websocket_client.cpp
  handshake_protocol.cpp
  hub_connection.cpp
  hub_connection_builder.cpp
  hub_connection_impl.cpp
  json_helpers.cpp
  json_hub_protocol.cpp
  logger.cpp
  negotiate.cpp
  signalr_client_config.cpp
  signalr_value.cpp
  stdafx.cpp
  trace_log_writer.cpp
  transport.cpp
  transport_factory.cpp
  url_builder.cpp
  websocket_transport.cpp
  ../../third_party_code/cpprestsdk/uri.cpp
  ../../third_party_code/cpprestsdk/uri_builder.cpp
  ../../third_party_code/jsoncpp/jsoncpp.cpp
)

include_directories(
    ../../third_party_code/cpprestsdk
    ../../third_party_code/jsoncpp)

add_library (signalrclient ${SOURCES})

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    if(WERROR)
        target_compile_options(signalrclient PRIVATE /WX)
    endif()
    if(WALL)
        target_compile_options(signalrclient PRIVATE /Wall)
    endif()
    target_compile_options(signalrclient PRIVATE
        /wd4820 # padding added after data member
        /wd4514 # unreferenced inline function removed
        /wd5045 # compiler will insert Spectre mitigation if /Qspectre switch is added
        /wd4464 # relative include paths
        /wd4711 # function 'x' selected for automatic inline expansion
        /wd4710 # function not inlined
        /experimental:external /external:anglebrackets /external:templates- /external:W0
        /GR- # disable run-time type information
        /guard:cf # enable control-flow guard
        /EHa # enable C++ EH (w/ SEH exceptions)
    )
else()
    if(WERROR)
        target_compile_options(signalrclient PRIVATE -Werror)
    endif()
    if(WALL)
        target_compile_options(signalrclient PRIVATE -Wall)
    endif()

    # GCC on OSX has a bug with exceptions and no-rtti that can cause crashes
    if(NOT APPLE)
        target_compile_options(signalrclient PRIVATE -fno-rtti)
    endif()
    target_compile_options(signalrclient PRIVATE -Wextra -Wpedantic -Wno-unknown-pragmas)
endif()

if(NOT USE_CPPRESTSDK)
  target_link_libraries(signalrclient)
else()
  if(APPLE)
    target_link_libraries(signalrclient
      PUBLIC ${CPPREST_LIB}
      PRIVATE OpenSSL::SSL Boost::boost Boost::system Boost::chrono Boost::thread
    )
  elseif(NOT WIN32)
    target_link_libraries(signalrclient
      PUBLIC ${CPPREST_LIB} Boost::system
      PRIVATE OpenSSL::SSL
    )
  else()
    target_link_libraries(signalrclient
      PUBLIC ${CPPREST_LIB}
    )
  endif()
endif() # USE_CPPRESTSDK

include(GNUInstallDirs)

install(TARGETS signalrclient
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)