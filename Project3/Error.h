#pragma once

//ten class viet in _H
#ifndef ERROR_H
#define ERROR_H

#include <exception>
#include <string>


//quy uoc doi voi namespace TenClass+NS
namespace ErrorNS
{
	const int FATAL_ERROR = -1;
	const int WARNING = 0;
}


//ghi ro std::
//khong using namespace std;
class Error : public std::exception
{
	//ten bien _tenbien
	int _errorCode;
	std::string _msg;
public:
	Error() throw () : _errorCode(ErrorNS::FATAL_ERROR), _msg("Khong biet bi loi gi"){}
	Error(int errorCode, std::string msg) : _errorCode(errorCode), _msg(msg){}
	Error(const Error& e) throw () : _errorCode(e._errorCode), _msg(e._msg){}
	
	Error& operator = (const Error& e)
	{
		std::exception::operator=(e);
		_errorCode = e._errorCode;
		_msg = e._msg;
	}
	//lay ve noi dung cua loi
	int getErrorCode() { return _errorCode; }
	const char* getMsg()  const  throw() { return _msg.c_str(); }
};

#endif // !ERROR_H