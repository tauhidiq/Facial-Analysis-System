#include "ImageFileManager.h"
#include <sstream>
#include <set>

kifas::ImageFileManager::ImageFileManager()
{
	mImageFileNumber = 0;
	mImageFileDir = "";
	mFilePathList ="";

	mColorder = 1;

    mClassNum = 0;

    mSequenceNum = 0;

    mSequenceSize = 1;
}

/**
* Creates the file manager with the imageDir.
* @param imageDir is the image directory to read the images from.
* @param fileListName is the path to the file that contains the list of images
*/
kifas::ImageFileManager::ImageFileManager(std::string imageDir, std::string fileListName)
{
	mImageFileNumber = 0;
	mImageFileDir = "";
	mFilePathList ="";
  setImageFileDir(imageDir);
  readFileNames(fileListName);
}

kifas::ImageFileManager::~ImageFileManager()
{
}

/**
* Read the file names and truth values given in the fileListName file. 
* This function populates the member variables mImageFilePaths and mImageFileTruthTable.
* At the same time, it counts the number of images, and store it on mImagefileNumber.
* @param fileListName is the path to the file that contains the list of files and their truth values.
*/
void kifas::ImageFileManager::readFileNames(std::string fileListName)
{
    // for calculating the number of classes
    std::set<int> classSet;
    std::set<int> sequenceSet;

	mImageFilePaths.clear();
	mImageFileTruthTable.clear();
	mImageFilePersonID.clear();

	std::ifstream file;	
	file.exceptions ( std::ifstream::failbit | std::ifstream::badbit |std::ifstream::eofbit);
	std::string strFileName;
	char fileName[256];
	int truth_id;
	
	try
	{
        // file open
		file.open(fileListName.c_str(), std::ios_base::in);
	}
	catch(std::ifstream::failure e)
	{
    std::stringstream msg;
    msg << "Opening \"" << fileListName << "\" created an exception!";
    //TraceMessage::addError( msg.str() );
    //TraceMessage::addError( e.what() );
		throw KifasExpeption("[ImageFileManager::readFileNames] :Opening \""+fileListName+"\" created an exception! "+e.what());				
	}

	try
	{
        /// filename classID
		if(mColorder == 1)    // name anyID (%s %d)
		{
			while(!file.eof())
			{
				file >> fileName;
				file >> truth_id;

				strFileName = mImageFileDir;
				strFileName.append(fileName);

				// add list
				mImageFilePaths.push_back(strFileName);
				mImageFileTruthTable.push_back(truth_id);

                // add class id to set
                classSet.insert(truth_id);

				mImageFileNumber++;
			}
		} /// filename personID classID
		else if(mColorder == 0) // name perID anyID
		{
			int person_id;

			while(!file.eof())
			{
				file >> fileName;
				file >> person_id;
				file >> truth_id;

				strFileName = mImageFileDir;
				strFileName.append(fileName);

				// add list
				mImageFilePaths.push_back(strFileName);
				mImageFileTruthTable.push_back(truth_id);
				mImageFilePersonID.push_back(person_id);

                // add class id to set
                classSet.insert(truth_id);

				mImageFileNumber++;
			}
		}
        else if(mColorder == 3) // name seqID perID anyID (%s %d %d %d)
		{
			int person_id;
            int seq_id;

			while(!file.eof())
			{
				file >> fileName;
                file >> seq_id;
				file >> person_id;
				file >> truth_id;

				strFileName = mImageFileDir;
				strFileName.append(fileName);

				// add list
				mImageFilePaths.push_back(strFileName);
                mImageFileSeqID.push_back(seq_id);
				mImageFileTruthTable.push_back(truth_id);
				mImageFilePersonID.push_back(person_id);

                // add class id to set
                classSet.insert(truth_id);
                sequenceSet.insert(seq_id);
				mImageFileNumber++;
			}
		}
        else if(mColorder == 4) // name perID anyID
		{
			int seq_id;

			while(!file.eof())
			{
				file >> fileName;
				file >> seq_id;
				file >> truth_id;

				strFileName = mImageFileDir;
				strFileName.append(fileName);

				// add list
				mImageFilePaths.push_back(strFileName);
				mImageFileTruthTable.push_back(truth_id);
				mImageFileSeqID.push_back(seq_id);

                // add class id to set
                classSet.insert(truth_id);
                sequenceSet.insert(seq_id);

				mImageFileNumber++;
			}
		}

	}
	catch(std::ifstream::failure e)
	{
		if(file.eof() && file.fail() )
		{
			file.close();
			mIterator = mImageFilePaths.begin();
			
			TraceMessage::addMessage("The number of all images is ");
			TraceMessage::addMessageLine(mImageFileNumber);

            mClassNum = (int)(classSet.size());
            TraceMessage::addMessage("The number of classes is ");
            TraceMessage::addMessageLine(mClassNum);

            if((mColorder == 4) || (mColorder == 3))
            {
                mSequenceNum  = sequenceSet.size();

                if(vaildateSequenceNumber() == false)
                {
                    //TraceMessage::addMessage("");
                    throw KifasExpeption("[ImageFileManager::readFileNames] :When this program read \""+fileListName+"\", All sequence number are not same!!");
                }
                mIteratorSeq = mImageFileSeqID.begin();
            }
			return;
		}
    std::stringstream msg;
		//msg << "When this program read \"" << fileListName << "\", a fail exception occurred before end of file!!";
		//TraceMessage::addError( msg.str() );
		throw KifasExpeption("[ImageFileManager::readFileNames] :When this program read \""+fileListName+"\", a fail exception occurred before end of file!! "+e.what());
	}

	file.close();
}

bool kifas::ImageFileManager::resetFileIndex()
{	
	mIterator = mImageFilePaths.begin();
	return true;
}

/**
* Sets the image directory, and checks for the ending slash (this function adds the slash if it is not present).
* @param imageDir is the image directory to read the images from.
*/
void kifas::ImageFileManager::setImageFileDir(std::string imageDir)
{
	mImageFileDir = imageDir;

	char checkstr = mImageFileDir.at(mImageFileDir.size()-1);

	// Check whether the directory ends with a slash, and add it if it's not the case
	if( checkstr != '/' || checkstr != '\\')
		mImageFileDir.append("/");   // Linux , Window are different
}

std::string kifas::ImageFileManager::getNextImageFilePath()		// ���� ���� �н� �������� �Լ�
{
	if(mIterator != mImageFilePaths.end())
	{
		std::list<std::string>::iterator tempIterator = mIterator;	
		mIterator++;
		return *tempIterator;
	}
	else
	{
		return "0";
	}

}

std::vector<std::string> kifas::ImageFileManager::getNextImageFilePaths() // next path
{
    // a static image
    if((mColorder == 0) || (mColorder== 1))
    {
        if(mIterator != mImageFilePaths.end())
	    {
		    std::list<std::string>::iterator tempIterator = mIterator;	
		    mIterator++;
            std::vector<std::string> imagePathList;
            imagePathList.push_back(*tempIterator);
		    return imagePathList;
	    }
        else
        {
            std::vector<std::string> imagePathList;
            return imagePathList;
        }
    }
    else if((mColorder == 3) || (mColorder== 4))
    {
        if(mIterator != mImageFilePaths.end())
	    {
            std::vector<std::string> imagePathList;

            int curSeqID = *mIteratorSeq;

            while((mIteratorSeq != mImageFileSeqID.end()) && (*mIteratorSeq == curSeqID))
            {
                imagePathList.push_back(*mIterator);
                mIterator++;
                mIteratorSeq++;
            }
            
		    return imagePathList;
	    }
        else
        {
            std::vector<std::string> imagePathList;
            return imagePathList;
        }
    }
}

int  kifas::ImageFileManager::getImageFileNumber()
{
    if((mColorder == 0) || (mColorder== 1))
        return mImageFileNumber;
    else
        return mSequenceNum;
}

bool kifas::ImageFileManager::vaildateSequenceNumber()
{
    // 1. set mIteratorSeq start
    // 2. calculate the number of the first sequnce
    // 3. check all sequences have the same number of the first sequnce
    
    // 1. set mIteratorSeq start
    mIteratorSeq = mImageFileSeqID.begin();
    int curSeqID = *mIteratorSeq;

    // 2. calculate the number of the first sequnce
    int seqNum = 0;
    while(*mIteratorSeq == curSeqID)
    {
        mIteratorSeq++;
        seqNum++;
    }

    // 3. check all sequences have the same number of the first sequnce
    int otherSeqNum;
    while(mIteratorSeq != mImageFileSeqID.end())
    {
        otherSeqNum = 0;
        int curID = *mIteratorSeq;
        while( (mIteratorSeq != mImageFileSeqID.end()) && (*mIteratorSeq == curID))
        {
            mIteratorSeq++;
            otherSeqNum++;
        }

        if(otherSeqNum != seqNum)
            return false;
    }

    mSequenceSize = seqNum;

    // make label id for sequence
    int i;
    std::vector<int> newTruthTable;
    for(i=0; i< mImageFileTruthTable.size(); i= i+ mSequenceSize)
        newTruthTable.push_back(mImageFileTruthTable[i]);

    mImageFileTruthTable = newTruthTable;

    // if the image list contains person information
    if(mColorder == 3)
    {
        std::vector<int> newPIDTable;
        for(i=0; i< mImageFilePersonID.size(); i= i+ mSequenceSize)
        newPIDTable.push_back(mImageFilePersonID[i]);

        mImageFilePersonID = newPIDTable;
    }

    return true;
}