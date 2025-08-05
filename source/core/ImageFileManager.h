/**
  @file ImageFileManager.h
  This files defines the manager class for reading the list of image files feature vectors.
*/


#ifndef IMAGEFILEMANAGER_H_INCLUDED
#define IMAGEFILEMANAGER_H_INCLUDED
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "Exceptions.h"
#include "TraceMessage.h"

namespace kifas
{
	
/**
  Manage the list of image files.
  Gives the access to the paths, and the number of files.
*/
class ImageFileManager
{

public:
	ImageFileManager();
	ImageFileManager(std::string imageDir, std::string fileListName);

	~ImageFileManager();

	void readFileNames(std::string fileListName);
	bool resetFileIndex();					
	void setImageFileDir(std::string imageDir);
	void setColOrder(int ColOrder){ mColorder = ColOrder;};
	std::string getNextImageFilePath();		// 다음 파일 패스 가져오는 함수	"0"문자열이 들어오면 모두 끝난상태임
    std::vector<std::string> getNextImageFilePaths(); // next path
	int getImageFileNumber();//{ return mImageFileNumber; }
    bool vaildateSequenceNumber();


	std::vector<int> getImageFileTruthTable(){ return mImageFileTruthTable; }
	std::vector<int> getImageFilePersonTable(){ return mImageFilePersonID;}

    int getClassNumber(){ return mClassNum;}
    int getSequenceSize(){ return mSequenceSize;}
protected:
	
	std::string mFilePathList;			    // 이미지 파일 리스트가 저장된 파일 이름
	std::list<std::string> mImageFilePaths;	// 이미지 파일을 저장할 리스트 
	std::vector<int> mImageFileTruthTable;	// 이미지 파일의 분류 값 ex) 0:happy 1:disgust 2:........
	std::vector<int> mImageFilePersonID;	// 이미지 파일의 사람 ID 
    std::vector<int> mImageFileSeqID;	// 이미지 파일의 Sequence ID 

	std::string mImageFileDir;				// Directory of image files

	int mImageFileNumber;					// The number of image files

	std::list<std::string>::iterator mIterator;			// iterator for image file path
    std::vector<int>::iterator mIteratorSeq;		// iterator for sequence ID

	int mColorder;							// Column order. ex)filename personID expressionID or filename anyID

    int mClassNum;                          // The number of classes, ex) 6 is 6 class, 7 is 7 class
    int mSequenceNum;                       // The number of sequence
    int mSequenceSize;                      // The size of each sequence
};

}

#endif