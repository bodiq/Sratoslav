cmake_minimum_required(VERSION 2.8.4)
project(Stratoslav)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -I /bodya/X11/include -L /opt/X11/lib -lX11")
set(Boost_USE_MULTITHREADED ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)
find_package(CURL)
find_package(X11 REQUIRED)


include_directories(${X11_INCLUDE_DIR})
link_directories(${X11_LIBRARIES})
include_directories(/usr/local/include ${OPENSSL_INCLUDE_DIR} ${Boost_INCLUDE_DIR})
if (CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIRS})
    add_definitions(-DHAVE_CURL)
endif()

add_executable(Stratoslav main.cpp Pool/ThreadPool/ThreadPool.cpp Pool/Async/Async.cpp inc/Phrases.cpp)

target_link_libraries(Stratoslav /usr/local/lib/libTgBot.a ${CMAKE_THREAD_LIBS_INIT} ${OPENSSL_LIBRARIES} ${Boost_LIBRARIES} ${CURL_LIBRARIES} ${X11_LIBRARIES})