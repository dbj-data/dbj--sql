#pragma once
/*
(c) 2019 by dbj@dbj.org -- CC BY-SA 4.0 -- https://creativecommons.org/licenses/by-sa/4.0/
*/
#include "test_db.h"


namespace dbj_sql_user
{
	/*
	no exceptions are thrown
	we return the db_valstat, and
	we make sure it is not discarded
	*/
	[[nodiscard]] inline sql::dbj_sql_valstat<int> test_wrong_insert(sql::database const& db) noexcept
	{

		/*
		 here we make sure we insert utf8 encoded ANSI string literals
		 please read here about u8 and execution_character_set
		 https://docs.microsoft.com/en-gb/cpp/preprocessor/execution-character-set?view=vs-2017

		 note: db.exec return the status too

		 this SQL is wrong because we already have 4,5,6 as primary keys in that table in the demo db
		 thus the caller will be notified and will decide what to do
		 */
		constexpr auto BAD_SQL = u8"INSERT INTO entries (Id, Name) "
			u8"values (4, 'Krčedin'), (5, 'Čačak'), (6, 'Kruševac')";

		DBJ_WARN("Attempting BAD SQL: %s", BAD_SQL);
		return db.exec(BAD_SQL);
	}
	/*
	callback to be called per each row of
	the result set made by:
		SELECT Id,Name FROM demo_table
	Singature of the callback function is always the same
	*/
	int sample_callback(
		const size_t row_id,
		sql::cursor_iterator result_row_
	)
	{
		// get the int value of the first column
		int   id_ = *(result_row_.to_int32(0));
		// get the string value of the second column
		buffer_type::value_type   name_ = *(result_row_.to_text(1));
		// print what we got

		DBJ_PRINT(" %zu  %s = %d  %s = %s",
			row_id, result_row_.name(0), id_, result_row_.name(1), name_.data());

		return SQLITE_OK;
		// we have to use SQLITE_OK macro
		// otherwise sqlite3 will stop the 
		// result set traversal
	}


	/*
	use the universal callback provided by dbj++sqlite
	*/
	[[nodiscard]] inline sql::dbj_sql_valstat<int> test_table_info(sql::database const& db) noexcept
	{
		DBJ_PRINT("meta data for columns of the table 'entries'");
		/*
		execute the table_info and pass the status out
		*/
		return sql::table_info(db, "entries", sql::universal_callback);
	}

	[[nodiscard]] inline sql::dbj_sql_valstat<int> test_select(sql::database const& db) noexcept
	{
		DBJ_PRINT("execute: 'SELECT Id, Name FROM entries'");
		return db.query("SELECT Id,Name FROM entries", sample_callback);
	}

	/*
	This is how dbj++sqlite helps you consume the sqlite3 query results
	Remember: this is called once per each row
	SQL statement result set this callback is processing is:
	"select word, definition from entries where word like 'zyga%'"
	*/
	int example_callback(
		const size_t row_id,
		dbj::sql::cursor_iterator row_
	)
	{
		// 'automagic' transform to the buffer type
		// of the column 0 value for this row
		// auto can not be used here
		// compiler would not know what type you want
		buffer_type::value_type  word_ = *(row_.to_text(0));
		buffer_type::value_type  definition_ = *(row_.to_text(1));
		DBJ_PRINT("%3zd: %12s | %s", row_id, word_.data(), definition_.data());

		//// all these should provoke exception
		//// TODO: but they don't -- currently
		//long   DBJ_MAYBE(number_) = row_(0);
		//double DBJ_MAYBE(real_) = row_(0);

		return SQLITE_OK;
	}

	/* here we use the external database
	the DB_FILE_PATH is set to its full path
	*/
	UTEST(dbjsqlite, simple_select)
	{
		sql::dbj_sql_valstat<int> status_{};
		// we do not need to make the database just obtain its instance
		// the we do not use sql::db_initor here
		sql::database db(DICTIONARY_DB_FILE_PATH, status_);
		// some kind of error has happened
		if (sql::is_error_valstat(status_)) {
			DBJ_ERR("ERROR Status : %s, While opening the database: %s",
				status_.status->data(), DICTIONARY_DB_FILE_PATH
			);
			return;
		}

		constexpr auto SQL = "SELECT word, definition FROM entries WHERE word LIKE 'zyga%'";
		DBJ_PRINT("External database: %s, testing query: %s", db.db_name(), SQL);
		// do not return the status
		sql::print_on_sql_error(db.query(SQL, example_callback));
	}

	/*
	Test Unit registration
	*/
	UTEST(dbjsqlite, error_handling)
	{
		auto [database, status] = demo_db();
		if (!database) {
			DBJ_SQL_LOG("%s", status->data());
			return; // error
		}
		sql::print_on_sql_error(test_wrong_insert(*database));
		sql::print_on_sql_error(test_table_info(*database));
		sql::print_on_sql_error(test_select(*database));

		/*
		NOTE: above we just perform "print-and-proceed"
		usually callers will use status_ to develop more involved logic
		*/
	}

	UTEST(dbjsqlite, Creating_and_querying_rezults_database)
	{

		DBJ_PRINT("Creating and querying rezults database. Rezults of C++ runtime buffer types.");

		auto [db, status] = rezults_db();
		if (!db) {
			DBJ_PRINT("%s", status->data());
			return; // error
		}

		// db type is optional<reference_wrapper< sql::database >>
		sql::database const& database = *db;

		DBJ_PRINT("Database: %s, meta data for columns of the table 'rezults'", database.db_name());
		sql::print_on_sql_error(sql::table_info(database, "rezults", sql::universal_callback));

		const char* const QRY[]{ "SELECT rank,size, rezult,comment FROM rezults GROUP BY size ORDER BY rank" };

		DBJ_PRINT("%s", QRY[0]);
		sql::print_on_sql_error(database.query(QRY[0], sql::universal_callback));

	}

} // eof two_tests namespace