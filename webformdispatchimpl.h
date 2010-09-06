#ifndef _WEBFORM_DISPATCH_IMPL_H__
#define _WEBFORM_DISPATCH_IMPL_H__

#include "webformdispatchhandler.h"

class WebformDispatchImpl : public WebformDispatchHandler {
	virtual void BeforeNavigate(std::string url, bool *cancel);
};

#endif