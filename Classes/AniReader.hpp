#ifndef __ANI_READER__
#define __ANI_READER__
#include "stdio.h"
#include "cocos2d.h"
#include "string.h"
//#include <CCFileUtils.h>
USING_NS_CC;
//#include<string.h>
//using namespace std;
class AniReader{
public:

	//char *filename;
    static int list[100][4];
    static int num;
    static int mapw;
    static int maph;
    static void read(char* filename)
    {
    	//FILE *inFp;
    	char anipath[80];
    	sprintf(anipath,"data/%s",filename);
    	//std::string path=CCFileUtils::sharedFileUtils()->fullPathForFilename(filename);
    	unsigned long int len;
    	unsigned char* data = CCFileUtils::sharedFileUtils()->getFileData(CCFileUtils::sharedFileUtils()->fullPathForFilename(anipath).c_str(),"r",&len);
    	num = (int)data[1] + 16 * 16 * (int)data[0];
    	for (int i = 0; i < num; i++)
    	{
    		list[i][0] = (int)data[i*13+4] + 16 * 16 * (int)data[i*13+3];
    		list[i][1] = (int)data[i*13+6] + 16 * 16 * (int)data[i*13+5];
    		list[i][2] = (int)data[i*13+8] + 16 * 16 * (int)data[i*13+7];
    		list[i][3] = (int)data[i*13+10] + 16 * 16 * (int)data[i*13+9];
    	}
    	/*inFp = fopen(data, "rb");
    	unsigned char x,y,z;
    	fread(&x, sizeof(char), 1, inFp);
    	fread(&y, sizeof(char), 1, inFp);
    	num = (int)y + 16 * 16 * (int)x;
    	for (int i = 0; i < num; i++)
    	{
    		fread(&x, sizeof(char), 1, inFp);

    	    fread(&x, sizeof(char), 1, inFp);   //X
    	    fread(&y, sizeof(char), 1, inFp);
    	    list[i][0] = (int)y + 16 * 16 * (int)x;
    	    fread(&x, sizeof(char), 1, inFp);    //Y
    	    fread(&y, sizeof(char), 1, inFp);
    	    list[i][1] = (int)y + 16 * 16 * (int)x;
    	    fread(&x, sizeof(char), 1, inFp);    //W
    	    fread(&y, sizeof(char), 1, inFp);
    	    list[i][2] = (int)y + 16 * 16 * (int)x;
    	    fread(&x, sizeof(char), 1, inFp);    //H
    	    fread(&y, sizeof(char), 1, inFp);
    	    list[i][3] = (int)y + 16 * 16 * (int)x;

    	    fread(&x, sizeof(char), 1, inFp);
    	    fread(&y, sizeof(char), 1, inFp);
    	    //printf("%02x %02x ",x,y);
    	    fread(&x, sizeof(char), 1, inFp);
    	    fread(&y, sizeof(char), 1, inFp);
    	    //printf("%02x %02x\n",x,y);*/

    }

};

int AniReader::list[100][4];
int AniReader::num;
int AniReader::mapw;
int AniReader::maph;

#endif
