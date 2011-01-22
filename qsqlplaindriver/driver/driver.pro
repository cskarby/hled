# This file is part of QSqlPlaintextDriver.
# 
# QSqlPlaintextDriver is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# any later version.
# 
# QSqlPlaintextDriver is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with QSqlPlaintextDriver.  If not, see <http://www.gnu.org/licenses/>.

TEMPLATE = lib
VERSION = 0.0.1

TARGET = lib/csvdriver

DEPENDPATH += src include
INCLUDEPATH += include

OBJECTS_DIR = obj
MOC_DIR = obj

QT = core sql

SOURCES =\
		 csvresult.cpp\
		 sqlcsvdriver.cpp

HEADERS =\
		 csvresult.hpp\
		 sqlcsvdriver.hpp
