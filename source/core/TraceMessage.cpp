#include "TraceMessage.h"
#include "ConfigFileManager.h"
#include <ctime>

bool TraceMessage::mVerbose=true;
bool TraceMessage::mLogOut=false;
std::ofstream TraceMessage::mFileOut;

void TraceMessage::setFileWriteFlage(bool flag)
{
    if(flag == true)
    {
        // Make Log File name
        // Open Log File as append
        std::string LogFileName = kifas::ConfigFileManager::getConfigFilesDir();
        LogFileName = LogFileName.substr(0,LogFileName.size()-1);
        std::size_t found = LogFileName.rfind("/");
        if(found == std::string::npos)
        {
            found = LogFileName.rfind("\\");
        }
        LogFileName = LogFileName.substr(0, found);
        LogFileName.append("/kifas_log.txt");


        mFileOut.open(LogFileName.c_str(),std::ofstream::out | std::ofstream::app);
        
        std::string LogTime;
        time_t rawtime;
        time (&rawtime);
        LogTime = ctime (&rawtime);
        mFileOut<<std::endl<<std::endl;
        mFileOut<<"Program is runing at "<<LogTime;
        mLogOut  = true;
    }
    else
    {
        mLogOut = false;
    }
}
