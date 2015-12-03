#ifndef _CODE_CONVERT_
#define _CODE_CONVERT_
#include <iconv.h>  
class CodeConverter
{
private:
	iconv_t cd;
public:
	CodeConverter(const char *from_charset,const char *to_charset) 
	{
		cd = iconv_open(to_charset,from_charset);
	}

	int convert(char *inbuf,int inlen,char *outbuf,int outlen)
	{
		char **pin = &inbuf;
		char **pout = &outbuf;
		memset(outbuf,0,outlen);
		return iconv(cd,pin,(size_t *)&inlen,pout,(size_t *)&outlen);
	}


	~CodeConverter()
	{
		iconv_close(cd);
	}
};


#endif
