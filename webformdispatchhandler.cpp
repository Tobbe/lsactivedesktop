#include "webformdispatchhandler.h"

void WebformDispatchHandler::BeforeNavigate(std::string url, bool *cancel)
{
	*cancel = false;
}

void WebformDispatchHandler::DocumentComplete(std::string url)
{
}
