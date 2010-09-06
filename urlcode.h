#ifndef _URL_CODE_H__
#define _URL_CODE_H__

#include <string>

class UrlCode {
private:
	static char FromHex(char ch);
public:
	static std::string Decode(std::string url);
};
#endif