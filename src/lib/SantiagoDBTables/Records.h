#ifndef SANTIAGO_SANTIAGODBTABLES_RECORDS_H
#define SANTIAGO_SANTIAGODBTABLES_RECORDS_H

#include <string>

#include "UserPermissions.h"

#include <boost/date_time/posix_time/posix_time.hpp>

namespace Santiago{ namespace SantiagoDBTables
{
    using namespace boost::posix_time;
            
    struct UserProfile
    {
        int _id;
        std::string _userName;
        std::string _password;
    };
    
    struct Session
    {
        int _id;
        std::string _userName;
        std::string _cookieId;
        ptime _loginTime;
        ptime _logoutTime;  
    };
        
    struct Permission
    {
        int _id;
        int _resId;
        std::string _userName;
        UserPermission  _userPermission;
    };
}}

#endif
