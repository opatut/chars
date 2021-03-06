# Copyright (c) 2009, Whispersoft s.r.l.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
# * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
# * Neither the name of Whispersoft s.r.l. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Find mysqlclient C++
# Find the native MySQL for C++ includes and library
#
#  MySQLpp_INCLUDE_DIR - where to find mysql.h, etc.
#  MySQLpp_LIBRARIES   - List of libraries when using MySQL.
#  MySQLpp_FOUND       - True if MySQL found.

if (MySQLpp_INCLUDE_DIR)
  # Already in cache, be silent
  set (MySQLpp_FIND_QUIETLY TRUE)
endif (MySQLpp_INCLUDE_DIR)

find_path(MySQLpp_INCLUDE_DIR mysql++.h
  /opt/local/include/mysql++
  /usr/local/include/mysql++
  /usr/include/mysql++
)

set(MySQLpp_NAMES mysqlpp)
find_library(MySQLpp_LIBRARY
  NAMES ${MySQLpp_NAMES}
  PATHS /opt/local/lib /usr/local/lib /usr/lib
  PATH_SUFFIXES mysql
)

if (MySQLpp_INCLUDE_DIR AND MySQLpp_LIBRARY)
  set(MySQLpp_FOUND TRUE)
  set( MySQLpp_LIBRARIES ${MySQLpp_LIBRARY} )
else (MySQLpp_INCLUDE_DIR AND MySQLpp_LIBRARY)
  set(MySQLpp_FOUND FALSE)
  set( MySQLpp_LIBRARIES )
endif (MySQLpp_INCLUDE_DIR AND MySQLpp_LIBRARY)

if (MySQLpp_FOUND)
  if (NOT MySQLpp_FIND_QUIETLY)
    message(STATUS "Found MySQL: ${MySQLpp_LIBRARY}")
  endif (NOT MySQLpp_FIND_QUIETLY)
else (MySQLpp_FOUND)
  if (MySQLpp_FIND_REQUIRED)
    message(STATUS "Looked for MySQL libraries named ${MySQLpp_NAMES}.")
    message(FATAL_ERROR "Could NOT find MySQL library")
  endif (MySQLpp_FIND_REQUIRED)
endif (MySQLpp_FOUND)

mark_as_advanced(
  MySQLpp_LIBRARY
  MySQLpp_INCLUDE_DIR
  )
