
// A very  C++ wrapper for (subset of) libpq C API
//
// projet 2018-03-15  (C) 2011   Copyright 2018  <laroche.jeanpierre@gmail.com>
//
// THANK YOU   MERCI BEAUCOUP
//
// le site quebecois 			https://h-deb.clg.qc.ca/		pour son travaille de divulgation
//
// https://stackoverflow.com	une mine d'or pour comprendre avec des examples
//
// https://docs.postgresql.fr/	pour tous les efforts pour la traduction 
/*
 *
 * M. laroche jean Pierre  12-10-1951    laroche.jeanpierre@gmail.com
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
*/


#ifndef LIBPQWRP_H_INCLUDED
#define LIBPQWRP_H_INCLUDED


#include <string.h>
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <ostream>
#include <sstream>
#include <iomanip>

#include <type_traits>

#include <exception>
#include <typeinfo>
#include <stdexcept>

#include <libpq-fe.h>


namespace libpqwrp
{

#ifndef   _separator_
#define   _separator_		'|'		///  remplace caractère space  de délimitation multibuffer > stringstream
#endif



///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
///				fonction stream / switch for sql  
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$

#ifndef LIBPQWRP_CPP_INCLUDED

#define NAMEOF(variable) ((void)variable, #variable)


	 std::stringstream &operator>>(std::stringstream& out ,char  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	strncpy(&Argc, Args.c_str(),1);
    return out;
}


	 std::stringstream &operator>>(std::stringstream& out ,char * Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	sprintf(Argc ,"%s",Args.c_str());
    return out;
}
	 std::stringstream &operator>>(std::stringstream& out , std::string &Args)
{
	getline(out,Args,_separator_);

    return out;
}
 std::stringstream &operator>>(std::stringstream& out , int &Argi)
{
	std::string Args ; 
	getline(out,Args,_separator_);

	Argi =atoi(Args.c_str());
    return out;
}

 std::stringstream &operator>>(std::stringstream& out , double &Argd)
{
	std::string Args ; 
	getline(out,Args,_separator_);

	const struct lconv* loc;
	loc = localeconv();
	std::string ponct=".";
	int pos = Args.find(ponct);
	if (pos >0)
	{
		if (loc->decimal_point[0]==',' && pos >= 0 ) Args.replace(pos,ponct.length(),",");
	} 
	Argd =std::stod(Args.c_str());
    return out;
}

 std::stringstream &operator>>(std::stringstream& out ,float &Argf)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	const struct lconv* loc;
	loc = localeconv();
	std::string ponct=".";
	int pos = Args.find(ponct);
	if (pos >0)
	{
		if (loc->decimal_point[0]==',' && pos >= 0 ) Args.replace(pos,ponct.length(),",");
	}
	Argf =atof(Args.c_str());
    return out;
}

 std::stringstream &operator>>(std::stringstream& out ,bool &Argb)
{
	std::string Args ; 
	getline(out,Args,_separator_);

	Argb =atoi(Args.c_str());
    return out;
}

///--------------------------------------------------------------------------

#ifdef	ZONED_H_INCLUDED
	 std::stringstream &operator>>(std::stringstream& out ,Zchar  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}

	 std::stringstream &operator>>(std::stringstream& out ,Zdcml  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}

	 std::stringstream &operator>>(std::stringstream& out ,Zdate  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}


	 std::stringstream &operator>>(std::stringstream& out ,Ztime  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}


	 std::stringstream &operator>>(std::stringstream& out ,Zbool  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}

	 std::stringstream &operator>>(std::stringstream& out ,Ztext  &Argc)
{
	std::string Args ; 
	getline(out,Args,_separator_);
	
	Argc = Args.c_str();
    return out;
}

#endif	// ZONED_H_INCLUDED


///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
/// a function to make your alphanumeric switch
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
constexpr unsigned long long int Sql_switch(const char* c_name, unsigned long long int l_hash = 0)	/// string to int for switch
{

    return (*c_name == 0) ? l_hash : 101 * Sql_switch(c_name + 1) + *c_name;
}


#endif	// LIBPQWRP_CPP_INCLUDED
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
///			 prepare sql  formatage avec template variadique  etc........
///$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$



class libPQwrp																		/// gestion parametre sql 
{
	private:

	PGconn* conn;

	
	static constexpr char FORMAT_SPECIFIER = '?';
	
	std::stringstream sqlstream;
	
	bool first = true ;
	
	std::string fin_stage ;

	bool clean ;																/// contrôle  PGresult 

    
	protected :
	
	unsigned count_format_specifiers(std::string const& format);				/// count the number of parameters in the format
	
	void prepare(std::string const& format);									/// format without parameters causes an error

	public:
	
	bool errorSQL ;				/// fatal_error

	bool fetchEOF ;				/// fin de traitenent get reccords 

	int			rows;			/// initialise par les requetes
	int			cols;			/// initialise par les requetes

	int			rown;			/// variable de traitement
	int			coln;			/// variable de traitement

	
	PGresult* res;

	libPQwrp(){
	errorSQL =false ;

	fetchEOF =false ;

		};

	~libPQwrp(){};

	void connectDB(std::string info);												/// connect to the database
	
	void qexec( std::string sql);													/// PQexec

	void query(std::string sql, bool binary = true );								/// PQquery
	
	void fetchAll( std::string , std::string cursor = "mycursor" );					/// fetch ALL include requete

	void opensql( std::string sql, std::string cursor = "mycursor" );				/// query for fetchsql record / record

	void fetchsql( std::string cursor = "mycursor");								/// fetch record use openSQL

	void openupd( std::string sql);													/// query select for update

	char* rtvCh(int row, int column);												/// get the value of the row and the column
	
	double rtvDbl(int row, int column);												/// get the value of the row and the column

	int rtvInt(int row, int column);												/// get the value of the row and the column

	int nfield(std::string field);													/// number column of the field

	const char* cfield(int nfield);													/// name of the field

	int countfield();																/// number of columns

	int countrow();																	/// number of rows

	long int countqry(std::string sql);												/// count select .....

	bool is_Table(const char* table);												/// if exist table of the database

	void closeDB();																	/// close to the database
	
	void autocommitON();															/// set auotcommit ON
	
	void autocommitOFF();															/// set auotcommit OFF
		
	void begin();																	/// start transction for commit

	void commit();																	/// commit for transaction init

	void rollback();																/// rollbck for transaction init

	void end();																		/// end transaction for init

	void clearRes();																/// clear PGresult

	void savpoint();																/// point de sauvegarde

	void savpointRollback();														/// roolback base savepoint

	void savpointRelease();															/// delete savepoint






	template <typename Head, typename... Args>
	std::string prepare(std::string const& format, Head&& head, Args&&... args)		/// formatting the query with parameters
	{

		if (count_format_specifiers(format) > 0 && first == true)
		{
			fin_stage = format ;  first = false ;  sqlstream.str("");
			auto last_format_pos = format.find_last_of(FORMAT_SPECIFIER);
			fin_stage  =fin_stage.erase(0,  last_format_pos +1);
		}

		if (count_format_specifiers(format) != sizeof...(Args) + 1)
		throw std::invalid_argument("number of arguments does not match the format string");

		// TODO: take care of escaped format specifiers
		sqlstream <<"";
		auto first_format_pos = format.find_first_of(FORMAT_SPECIFIER);
  
		sqlstream << format.substr(0, first_format_pos);  
		sqlstream << head;
  
		if ( count_format_specifiers(format) == 1 )
		{
			sqlstream<<fin_stage; first =true ;
		}
  
		prepare(format.substr(first_format_pos+1), std::forward<Args>(args)...);
		return  sqlstream.str();
 
	}




		
	std::stringstream result();														/// out buffer;
	char* DoubleToChar(double _X_ ,unsigned _precision_ = 0 );						/// Double to char





};




} // namespace libpqwrp
using namespace libpqwrp ;
#endif // LIBPQWRP_H_INCLUDED
