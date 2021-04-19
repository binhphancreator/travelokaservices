cmake_minimum_required(VERSION 2.9)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(traveloka)

add_executable(${PROJECT_NAME} WIN32 "src/app.cpp")

find_package(wxWidgets REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${wxWidgets_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${wxWidgets_LIBRARIES})

find_package(cpprestsdk CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE cpprestsdk::cpprest cpprestsdk::cpprestsdk_zlib_internal cpprestsdk::cpprestsdk_brotli_internal)

set(MYSQL_DIRS "C:/Library/mysql-connector-c++-8.0.23-win32")
target_include_directories(${PROJECT_NAME} PRIVATE "${MYSQL_DIRS}/include/jdbc")
target_link_directories(${PROJECT_NAME} PRIVATE "${MYSQL_DIRS}/lib/vs14")

target_link_libraries(${PROJECT_NAME} PRIVATE mysqlcppconn-static mysqlcppconn8-static mysqlcppconn8 mysqlcppconn)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different "${MYSQL_DIRS}/lib/mysqlcppconn-9-vs14.dll" $<TARGET_FILE_DIR:${PROJECT_NAME}>)