# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/se1d/simple-pgw-master/build/_deps/boost_asio-src"
  "/home/se1d/simple-pgw-master/build/_deps/boost_asio-build"
  "/home/se1d/simple-pgw-master/build/_deps/boost_asio-subbuild/boost_asio-populate-prefix"
  "/home/se1d/simple-pgw-master/build/_deps/boost_asio-subbuild/boost_asio-populate-prefix/tmp"
  "/home/se1d/simple-pgw-master/build/_deps/boost_asio-subbuild/boost_asio-populate-prefix/src/boost_asio-populate-stamp"
  "/home/se1d/simple-pgw-master/build/_deps/boost_asio-subbuild/boost_asio-populate-prefix/src"
  "/home/se1d/simple-pgw-master/build/_deps/boost_asio-subbuild/boost_asio-populate-prefix/src/boost_asio-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/se1d/simple-pgw-master/build/_deps/boost_asio-subbuild/boost_asio-populate-prefix/src/boost_asio-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/se1d/simple-pgw-master/build/_deps/boost_asio-subbuild/boost_asio-populate-prefix/src/boost_asio-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
