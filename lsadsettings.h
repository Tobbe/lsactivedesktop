#ifndef LSAD_SETTINGS_H_
#define LSAD_SETTINGS_H_

#include <map>
#include <string>

struct LSADWebWndProp {
	UINT x;
	UINT y;
	UINT width;
	UINT height;
	bool showScrollbars;
	std::string url;
};

struct LSADSettings {
	bool showScrollbars;
	bool showErrors;

	std::map<std::string, LSADWebWndProp> windowProperties;
};

#endif