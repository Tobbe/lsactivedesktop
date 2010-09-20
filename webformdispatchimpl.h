#ifndef _WEBFORM_DISPATCH_IMPL_H__
#define _WEBFORM_DISPATCH_IMPL_H__

#include "webformdispatchhandler.h"

class WebForm;
class JSLiteStep;

class WebformDispatchImpl : public WebformDispatchHandler {
private:
	JSLiteStep *jsls;
public:
	WebformDispatchImpl(JSLiteStep *jsls);
	virtual void BeforeNavigate(std::string url, bool *cancel);
	virtual void NavigateComplete(std::string url, WebForm *webForm);
};

#endif