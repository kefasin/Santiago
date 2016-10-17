#ifndef SANTIAGO_USER_SERVER_CREATEUSERREQUESTHANDLER_H
#define SANTIAGO_USER_SERVER_CREATEUSERREQUESTHANDLER_H

#include "RequestHandlerBase.h"

namespace Santiago{namespace User { namespace Server
{

    class CreateUserRequestHandler:public RequestHandlerBase
    {
    public:
        CreateUserRequestHandler(ConnectionServer& connectionServer_
                                 ,const OnCompletedCallbackFn& onCompletedCallbackFn_
                                 ,const ServerMessage& initiatingMessage_);
        virtual void start();
	virtual void handleReplyMessage(const ServerMessage& serverMessage);
        
    };

}}}

#endif