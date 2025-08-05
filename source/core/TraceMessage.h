/**
  @file TraceMessage.h
  This files defines the TraceMessage class of the facial analysis system.
*/

#ifndef TRACE_MESSAGE_H_INCLUDED
#define TRACE_MESSAGE_H_INCLUDED

#include <iostream>
#include <fstream>

/**
* Handles the messages (normal, warning, or errors) of the application, and output it to the user.
* It may not display the messages if the verbose option is turned off.
* @ingroup core
*/
class TraceMessage
{

public:
	TraceMessage() {};
	~TraceMessage() {};

	static void setMessageFlag(bool flag){
		mVerbose = flag;
	};
	
    static void setFileWriteFlage(bool flag);
    
  /**
  * Displays a message depending on the verbose flag, while adding a line break at the end.
  * @param msg is the message to display.
  */
	static void addMessageLine(std::string msg){
		if(mVerbose)
			std::cout << msg.c_str() << std::endl;

        if(mLogOut)
			mFileOut << msg.c_str() << std::endl;

	};  

  /**
  * Displays a message depending on the verbose flag. It does not add a line break.
  * @param msg is the message to display.
  */
	static void addMessage(std::string msg){
		if(mVerbose)
			std::cout << msg.c_str();

        if(mLogOut)
			mFileOut << msg.c_str();
	};

	static void addMessage(int number) {
		if(mVerbose)
			std::cout << number;

        if(mLogOut)
			mFileOut << number;
	}

	static void addMessageLine(int number) {
		if(mVerbose)
			std::cout << number << std::endl;

        if(mLogOut)
			mFileOut << number << std::endl;
	}

	static void addMessage(double number) {
		if(mVerbose)
			std::cout << number;

        if(mLogOut)
			mFileOut << number;
	}

	static void addMessageLine(double number) {
		if(mVerbose)
			std::cout << number << std::endl;

        if(mLogOut)
			mFileOut << number << std::endl;
	}

  /**
  * Display a message regardless of the verbose flag, while adding a line break.
  * @param msg is the message to display.
  */
  static void displayMessageLine(std::string msg) {
    if(mVerbose)
      std::cout << msg.c_str();

    if(mLogOut)
      mFileOut << msg.c_str();
  }

  /**
  * Display a message regardless of the verbose flag. It does not add a line break.
  * @param msg is the message to display. 
  */
  static void displayMessage(std::string msg) {
    if(mVerbose)
      std::cout << msg.c_str();

    if(mLogOut)
      mFileOut << msg.c_str();
  }

  /**
  * Displays the error message regardless of the verbose flag.
  * @param msg is the message to display.
  */
  static void addError(std::string msg) {
    if(mVerbose)
      std::cout << "Error: " << msg.c_str() << std::endl;

    if(mLogOut)
      mFileOut << "Error: " << msg.c_str() << std::endl;
  }

  /**
  * Displays the warning message regardless of the verbose flag.
  * @todo Set a flag to display the warnings or not. This may give a fine tuning to the user to display certain messages.
  */
  static void addWarning(std::string msg) {
    if(mVerbose)
      std::cout << "Warning: " << msg.c_str() << std::endl;

    if(mLogOut)
      mFileOut << "Warning: " << msg.c_str() << std::endl;
  }

protected:
  /** A flag handling whehter to show the messages */
	static bool mVerbose;

    static bool mLogOut;
    static std::ofstream mFileOut;
};



#endif
