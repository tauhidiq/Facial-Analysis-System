/**
  @file 
  Defines the parser for configuration files
*/

#ifndef Parser_H_
#define Parser_H_

#include <string>
#include <vector>
#include <map>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>

#include "NumStr.h"

namespace kifas
{

/**
  Analyses the configuration files, and extracts the pairs of key and value from them.
  @ingroup core
*/
class Parser
{
public:
  static std::map<std::string, NumStr> parse(std::string filePath, std::map<std::string, NumStr> defaults);
  static void parseWrite(std::string filePath, std::map<std::string, NumStr> defaults);

  static inline std::string ltrim(std::string s);  
  static inline std::string rtrim(std::string s);  
  static inline std::string trim(std::string s);
  
  static inline std::vector<std::string> split(const std::string& str, const std::string& delimiters);
};


}

#endif
