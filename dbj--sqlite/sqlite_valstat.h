#pragma once
#ifndef _DBJ_SQLITE_STATUS_INC_
#define _DBJ_SQLITE_STATUS_INC_
/*
(c) 2019 -2021 by dbj.org   -- https://dbj.org/license_dbj
*
* for the background start from here https://github.com/DBJDBJ/metastate
*/


#include "dbj++sql.h"
#include <dbj/dbj_buffer.h>
#include <dbj/dbj_valstat.h>

#include <string_view>

namespace dbj::sql
{
	// value is T*
	// status is const char * aka std::string_view
	// thus be carefull not to have it as dangling
	template<typename T>
	using dbj_sql_valstat = typename dbj::standard::valstat<T, std::string_view >;

	// make and return error or ok valstat instance
#define DBJ_SQL_VALSTAT_ERR(C_) {{}, std::string_view{ err_message_sql(C_).data()} }
#define DBJ_SQL_VALSTAT_OK(V_) {V_ , {}}


	// buffer_type::value_type is vector<char>
	// reminder: std::string is not ment to be char buffer
	using buffer_type = typename dbj::buffer<char>;

#define DBJ_SQL_INNER_NS

	// redundant: constexpr const char* category_name() { return "sqlite3"; }

	enum class sqlite_status_code : int
	{
		/*
			https://sqlite.org/c3ref/c_abort.html
			note: this must match with sqlite3 # defines
		*/
		sqlite_ok = SQLITE_OK,	/* successful result */
		sqlite_error = SQLITE_ERROR, /* generic error */
		sqlite_internal = SQLITE_INTERNAL, /* internal logic error in sqlite */
		sqlite_perm = SQLITE_PERM, /* access permission denied */
		sqlite_abort = SQLITE_ABORT, /* callback routine requested an abort */
		sqlite_busy = SQLITE_BUSY, /* the database file is locked */
		sqlite_locked = SQLITE_LOCKED, /* a table in the database is locked */
		sqlite_nomem = SQLITE_NOMEM, /* a malloc() failed */
		sqlite_readonly = SQLITE_READONLY, /* attempt to write a readonly database */
		sqlite_interrupt = SQLITE_INTERRUPT, /* operation terminated by sqlite= 3 _interrupt()*/
		sqlite_ioerr = SQLITE_IOERR, /* some kind of disk i/o error occurred */
		sqlite_corrupt = SQLITE_CORRUPT, /* the database disk image is malformed */
		sqlite_notfound = SQLITE_NOTFOUND, /* unknown opcode in sqlite= 3 _file_control() */
		sqlite_full = SQLITE_FULL, /* insertion failed because database is full */
		sqlite_cantopen = SQLITE_CANTOPEN, /* unable to open the database file */
		sqlite_protocol = SQLITE_PROTOCOL, /* database lock protocol error */
		sqlite_empty = SQLITE_EMPTY, /* internal use only */
		sqlite_schema = SQLITE_SCHEMA, /* the database schema changed */
		sqlite_toobig = SQLITE_TOOBIG, /* string or blob exceeds size limit */
		sqlite_constraint = SQLITE_CONSTRAINT, /* abort due to constraint violation */
		sqlite_mismatch = SQLITE_MISMATCH, /* data type mismatch */
		sqlite_misuse = SQLITE_MISUSE, /* library used incorrectly */
		sqlite_nolfs = SQLITE_NOLFS, /* uses os features not supported on host */
		sqlite_auth = SQLITE_AUTH, /* authorization denied */
		sqlite_format = SQLITE_FORMAT, /* not used */
		sqlite_range = SQLITE_RANGE, /* = 2 nd parameter to sqlite= 3 _bind out of range */
		sqlite_notadb = SQLITE_NOTADB, /* file opened that is not a database file */
		sqlite_notice = SQLITE_NOTICE, /* notifications from sqlite= 3 _log() */
		sqlite_warning = SQLITE_WARNING, /* warnings from sqlite= 3 _log() */
		sqlite_row = SQLITE_ROW, /* sqlite= 3 _step() has another row ready */
		sqlite_done = SQLITE_DONE        /* sqlite= 3 _step() has finished executing */
	};                          // dbj_status_code/*

		/*
		The sqlite3_errstr() returns the English-language text that
		describes the result code, as UTF-8. Memory to hold the error message
		string is managed internally and must not be freed by the application.

		function argument is int so that native sqlite3 return values can be
		used also
		*/
	inline buffer_type::value_type err_message_sql(int sqlite_return_value)  noexcept
	{
		static auto unknown_code_message_ = buffer_type::make("Unknown SQLITE status code");

		if (const char* mp_ = ::sqlite::sqlite3_errstr(sqlite_return_value); mp_ != nullptr)
			return buffer_type::make(mp_);
		else
			return unknown_code_message_;
	}

	// the overload 
	inline buffer_type::value_type err_message_sql(sqlite_status_code code_)  noexcept
	{
		return err_message_sql(int(code_));
	}

	inline int code_to_int(sqlite_status_code code_)  noexcept
	{
		return int(code_);
	}

	/*
	this is the sqlite3 logic: not all codes are errors
	*/
	inline constexpr bool is_sqlite_error(sqlite_status_code sc_) noexcept
	{
		switch (sc_) {
		case sqlite_status_code::sqlite_ok:
		case sqlite_status_code::sqlite_row:
		case sqlite_status_code::sqlite_done:
			return false;
		default:
			return true;
		};
	}

	// sqlite api returns int status codes basically
	inline constexpr bool is_sqlite_error(int sqlite3_result_) noexcept
	{
		return is_sqlite_error(sqlite_status_code(sqlite3_result_));
	}

	// is valstat value representing SQLite error
	inline auto is_error_valstat = [](auto const& sqlite3_valstat) noexcept -> bool
	{
		// no value at all so def. an error
		if (!sqlite3_valstat.value) return true;
		// this requires value to be valid int value
		// invalid int value is -1 in here
		// because sqlite3 has no -1 return value
		static int invalid_sqlite_retval = -1;
		int int_value = invalid_sqlite_retval;
		if (int_value = sqlite3_valstat.value.value_or(invalid_sqlite_retval); int_value == invalid_sqlite_retval)
		{   // int value is inside but is not valid sqlite return value
			return true;
		}

		// value is present and OK
		// but check is it an error by sqlite3 logic
		return is_sqlite_error(int_value);
	};

	/* do not go overboard with macros */
	inline auto print_on_sql_error = [](auto const& S_)
	{
		if (S_.status)
		{
			DBJ_PRINT("ERROR Status %s ", S_.status->data());
		}
	};

#define DBJ_RETURN_ON_SQL_ERROR(S_) dbj::sql::print_on_sql_error(S_); return 

#define DBJ_PRINT_STATUS(S_) DBJ_PRINT("%s", S_->data() )


} // namespace dbj::sql

#endif // !_DBJ_SQLITE_STATUS_INC_
