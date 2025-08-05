/**
  @file Exceptions.h
  Defines the exception classes of the facial analysis system.
*/

#ifndef EXCEPTIONS_H_INCLUDED
#define EXCEPTIONS_H_INCLUDED

// OpenCV
#include <iostream>
#include <exception>

/**
  Extention of the standard exception.
  @ingroup core
  @todo Extend the functionality of the exception to produce meaningfull errors.
*/
class KifasExpeption: public std::exception
{
private:
	std::string m_errorMsg;

	
public:	
	KifasExpeption(std::string errorMsg);
	~KifasExpeption(void) throw(){}
	const char* what() const throw() { return m_errorMsg.c_str();}

	//virtual const char * what() const throw()
	//{
	//	return "KyungHee Image-processing Lab Facial Analysis System error!!!!";
	//}
};


#endif