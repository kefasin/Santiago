#ifndef SANTIAGO_EXAMPLES_APPSERVER_GETUSERINFO_H
#define SANTIAGO_EXAMPLES_APPSERVER_GETUSERINFO_H

#include <memory>

#include "RequestHandlerBase.h"

namespace Test{ namespace AppServer
{
   class GetUserInfoHandler:public RequestHandlerBase
   {
   public:

       GetUserInfoHandler(Santiago::User::ControllerBase& userController_):
            RequestHandlerBase(userController_)
       {}

   protected:

       void handleGetUserInfo(const RequestPtr& request_,
                              std::error_code error_,
                              const boost::optional<Santiago::User::UserInfo> &userInfo_);

       virtual void handleVerifiedRequest(const RequestPtr& request_,
                                          const std::string& userName_,
                                          const std::string& emailAddress_,
                                          const std::string& cookieString_);

       virtual void handleNonVerifiedRequest(const RequestPtr& request_);

   };
    }}


#endif
