#include "webformdispatchimpl.h"
#include <locale>
#include "windows.h"
#include "lsapi.h"
#include "urlcode.h"

void WebformDispatchImpl::BeforeNavigate(std::string url, bool *cancel)
{
	static const std::string lsexconst = "LSEXECUTE://";

	bool lsexecute = true;
	size_t i = 0;
	while (lsexecute && i < lsexconst.length()) {
		if (std::toupper(url[i], std::locale()) != lsexconst[i]) {
			lsexecute = false;
		}

		++i;
	}

	if (lsexecute) {
		std::string command = url.substr(lsexconst.length());
		if (command[command.length() - 1] == '/') {
			command = command.substr(0, command.length() - 1);
		}

		//MessageBox(NULL, UrlCode::Decode(command).c_str(), "LSExecute", MB_OK);
		LSExecute(NULL, UrlCode::Decode(command).c_str(), SW_NORMAL);
		// Set Cancel parameter to TRUE to cancel the current event
		*cancel = true;
	}
}
