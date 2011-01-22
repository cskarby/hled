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

#include "csvtest.hpp"

void CsvTest::initTestCase() {
	QSqlDatabase::registerSqlDriver( "CSV", new QSqlDriverCreator<CSVDriver> );
	db = QSqlDatabase::addDatabase( "CSV" );
	QCOMPARE( db.isValid(), true );
}

void CsvTest::open() {
	db.setDatabaseName( "new.csv" );
	QCOMPARE( db.open(), true );
}

void CsvTest::insertRecords() {
	QSqlQuery query;
	QCOMPARE( query.exec( "insert Intercooler-B;4500;MCO;0.8" ), true );
}

void CsvTest::commit() {
	QSqlQuery query;
	QCOMPARE( query.exec( "commit" ), true );
}

QTEST_MAIN( CsvTest );
