#include <fstream>

#include "Parser.h"
#include "TraceMessage.h"
#include "Exceptions.h"
#include "NumStr.h"

/**
  Analysis the given file, and extracts the pairs of key and value defined in them (separated by the equal sign '=').
  The function ignores the comments, defined with '#', and blank lines.
  @param filePath is the path of the file to parse.
  @return a map of the parsed values: key and value. Note that the key is converted to lowercase.
*/
std::map<std::string, kifas::NumStr> kifas::Parser::parse(std::string filePath, const std::map<std::string, NumStr> defaults)
{
  std::ifstream file;
  //file.exceptions( std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit );
  // initiate the new keys to the defaults
  std::map<std::string, kifas::NumStr> keysMap = defaults;
  
  // read file
  file.open(filePath.c_str(), std::ios_base::in);
	
  if(!file.is_open())	
    throw KifasExpeption("[kifas::Parser::parse] : While opening \"" + filePath + "\", an exception occurred!!\n");

   
  std::string line;
  while( std::getline(file, line ) ) // Note: getline() returns a stream reference. This is automatically cast to boolean for the test. streams have a cast to bool operator that checks good()
  {         
    trim(line);
    // not empty nor commnetd line
    if(line.length() > 0 && line.compare(0,1,"#") != 0)
    {
      std::vector<std::string> tokens = split(line,"=");
      if (tokens.size() != 2)
        TraceMessage::addWarning("Wrong attribute pair: \"" + line + "\", I'm ignoring it.");
      else
      {
	    // transform to lower case the values		
		//if (tokens.at(0).compare("TrainCacheFile") != 0 && 
		//	tokens.at(0).compare("TestCacheFile") != 0 &&
		//	tokens.at(0).compare("ImageListFileTrain") != 0 &&
		//	tokens.at(0).compare("ImageListFileTest") != 0 &&
		//	tokens.at(0).compare("ImageDirTrain") != 0 &&
		//	tokens.at(0).compare("ImageDirTest") != 0 )
			std::transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), ::tolower);
			
			// transform to lower case the values
			//std::transform(tokens.at(1).begin(), tokens.at(1).end(), tokens.at(1).begin(), ::tolower);
			
			// insert or replace the values
			keysMap[tokens.at(0)] = tokens.at(1);
      }
    }
  }    
  
  if(file.bad())
	throw KifasExpeption("[kifas::Parser::parse] : While reading \"" + filePath + "\", an exception occurred!!\n");
	
  file.close();
  
  return keysMap;
}

void kifas::Parser::parseWrite(std::string filePath, std::map<std::string, NumStr> defaults)
{
	std::ifstream file;
	std::ofstream ofile;
	//file.exceptions( std::ifstream::failbit | std::ifstream::badbit | std::ifstream::eofbit );
	ofile.exceptions( std::ofstream::failbit | std::ofstream::badbit | std::ofstream::eofbit );
	// initiate the new keys to the defaults
	std::map<std::string, kifas::NumStr> keysMap = defaults;
  
	std::string fileContents;

	// read file
	file.open(filePath.c_str(), std::ios_base::in);
	
	if(!file.is_open())	
		throw KifasExpeption("[kifas::Parser::parseWrite] : While opening \"" + filePath + "\", an exception occurred!!\n");
  
	std::string oriLine, line;
	while( std::getline(file, oriLine ) ) // Note: getline() returns a stream reference. This is automatically cast to boolean for the test. streams have a cast to bool operator that checks good()
	{        
		//std::getline(file, oriLine );
		line = oriLine;
			
		// trim
		trim(line);
			
		// not empty nor commnetd line
		if(line.length() > 0 && line.compare(0,1,"#") != 0)
		{
			std::vector<std::string> tokens = split(line,"=");
			if (tokens.size() != 2)
			{
				TraceMessage::addWarning("Wrong attribute pair: \"" + line + "\", I'm ignoring it.");
			}
			else
			{
				// transform to lower case the values		
				//if (tokens.at(0).compare("TrainCacheFile") != 0 && 
				//tokens.at(0).compare("TestCacheFile") != 0 &&
				//tokens.at(0).compare("ImageListFileTrain") != 0 &&
				//tokens.at(0).compare("ImageListFileTest") != 0 &&
				//tokens.at(0).compare("ImageDirTrain") != 0 &&
				//tokens.at(0).compare("ImageDirTest") != 0 )
				std::transform(tokens.at(0).begin(), tokens.at(0).end(), tokens.at(0).begin(), ::tolower);
		
				// transform to lower case the values
				std::transform(tokens.at(1).begin(), tokens.at(1).end(), tokens.at(1).begin(), ::tolower);
		
				// finding the same token
				std::map<std::string, NumStr>::iterator it = keysMap.find(tokens.at(0));
				
				// config file has the same token as config map
				if(it != keysMap.end())
				{
					std::string tstr = it->second;
					oriLine = tokens.at(0) + "=" + tstr;
					keysMap.erase(it);
				}

					//	keysMap[tokens.at(0)] = tokens.at(1);
			}
		}

		fileContents = fileContents + "\n"+oriLine;
	}

	if(file.bad())
		throw KifasExpeption("[kifas::Parser::parseWrite] : While reading \"" + filePath + "\", an exception occurred!!\n");
	
	file.close();
	

	std::map<std::string, NumStr>::iterator it;
	for (it=keysMap.begin(); it!=keysMap.end(); ++it)
	{
		std::string additionalLine = it->first + " = " + std::string(it->second);
		fileContents = fileContents + "\n"+additionalLine;
	}

	// read file
	try
	{
		ofile.open(filePath.c_str(), std::ofstream::out | std::ofstream::trunc);
	}
	catch(std::ofstream::failure e)
	{
		//TraceMessage::addError("While opening \"" + filePath + "\", an exception occurred!!\n"+e.what());    
		throw KifasExpeption("[kifas::Parser::parseWrite] : While opening \"" + filePath + "\", an exception occurred!!\n"+e.what());
	}

	try
	{
		fileContents.erase(0,1);
		ofile.write(fileContents.c_str(),fileContents.size());
	}
	catch(std::ofstream::failure e)
	{
		ofile.close();
		throw KifasExpeption("[kifas::Parser::parseWrite] : While wrting \"" + filePath + "\", an exception occurred!!\n"+e.what());

	}

	ofile.close();
	return;

}

/**
  Trim from start
  @param s string to trim from.
  @return the left trimmed string.
*/
inline std::string kifas::Parser::ltrim(std::string s) 
{
  s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
  return s;
}

/**
  Trim from end
  @param s string to trim from.
  @return the right trimmed string.
*/
inline std::string kifas::Parser::rtrim(std::string s) 
{
  s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
  return s;
}

/**
  Trim from both ends
  @param s string to trim from.
  @return the trimmed string.
*/
inline std::string kifas::Parser::trim(std::string s) 
{
  return ltrim(rtrim(s));
}

/**
  Split the given string str by the given delimiters, and stores all the parts in tokens.
  @param str is the string to split.
  @param delimiters is a string containing the delimiters to split the string with.
  @return tokens is the resultant vector with the splited parts.
*/
std::vector<std::string> kifas::Parser::split(const std::string& str, const std::string& delimiters)
{
  std::vector<std::string> tokens;
  // Skip delimiters at beginning
  std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter"
  std::string::size_type pos = str.find_first_of(delimiters, lastPos);

  while (std::string::npos != pos || std::string::npos != lastPos)
  {
    // Found a token, add it to the vector
    tokens.push_back(trim(str.substr(lastPos, pos - lastPos)));
    // Skip delimiters.  Note the "not_of"
    lastPos = str.find_first_not_of(delimiters, pos);
    // Find next "non-delimiter"
    pos = str.find_first_of(delimiters, lastPos);
  }
  
  // ", " processing
  while(1)
  {
	  int pos = tokens[1].find("\"",0);

	  if(pos == std::string::npos)
		  break;

	  tokens[1].replace(pos,1,"");

  }
  
  return tokens;
}
