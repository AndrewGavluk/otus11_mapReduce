cmake_minimum_required(VERSION 3.9) 

add_executable(mapReduce main.cpp)
set_target_properties(mapReduce PROPERTIES CXX_STANDARD 14 CXX_STANDARD_REQIRED ON)
target_link_libraries(mapReduce PRIVATE pthread boost_system mapReduceLib)

if(MSVC)
	target_compile_options(mapReduce PRIVATE /W4)
else()
	target_compile_options(mapReduce PRIVATE -Wall -Wextra -Werror)
endif()

install(TARGETS mapReduce RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)
set(CPACK_PACKAGE_NAME ${PROJECT_NAME})

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

set(CPACK_PACKAGE_CONTACT gavl.andr96@gmail.com)

include(CPack)