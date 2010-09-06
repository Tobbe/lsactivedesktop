#ifndef _WEBFORM_DISPATCH_HANDLER_H__
#define _WEBFORM_DISPATCH_HANDLER_H__

#include <string>

class WebformDispatchHandler {
public:
	virtual void BeforeNavigate(std::string url, bool *cancel);
	virtual void DocumentComplete(std::string url);
};

#endif