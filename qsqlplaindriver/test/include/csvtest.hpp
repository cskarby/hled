#ifndef __csvtest_hpp
#define __csvtest_hpp

/* This file is part of QSqlPlaintextDriver.
 *
 * QSqlPlaintextDriver is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 * 
 * QSqlPlaintextDriver is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with QSqlPlaintextDriver.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtTest>
#include <QtSql>
#include <QtDebug>

#include "sqlcsvdriver.hpp"

class CsvTest : public QObject {
	Q_OBJECT

	private slots:
		void initTestCase();
		void open();
		void insertRecords();
		void commit();

	private:
		QSqlDatabase db;

};

#endif
