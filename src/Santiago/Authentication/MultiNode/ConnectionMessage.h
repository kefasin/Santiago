#ifndef SANTIAGO_AUTHENTICATION_MULTINODE_CONNECTIONMESSAGE_H
#define SANTIAGO_AUTHENTICATION_MULTINODE_CONNECTIONMESSAGE_H

/**
 * @file ConnectionMessage.h
 *
 * @section DESCRIPTION
 *
 * Contains the structs for the data that is passed between Client and Server
 */

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <string>

#include "../../Utils/STLog.h"

namespace Santiago{ namespace Authentication { namespace MultiNode
{
    enum class ConnectionMessageType
    {
        /* 2 types of messages: Requests and Replies
         * Requests - CR - Client Request, SR - Server Request
         * Replies - Succeeded, Failed
         */
        SUCCEEDED,
        FAILED,

        CR_CREATE_USER, 
        CR_LOGIN_USER,
        CR_VERIFY_COOKIE_AND_GET_USER_INFO,
        CR_LOGOUT_USER_FOR_COOKIE,
        CR_LOGOUT_USER_FOR_ALL_COOKIES,
        CR_CHANGE_USER_PASSWORD,
	CR_GET_USER_FOR_EMAIL_ADDRESS_AND_RECOVERY_STRING,
	CR_CHANGE_USER_PASSWORD_FOR_EMAIL_ADDRESS_AND_RECOVERY_STRING,
        CR_CHANGE_USER_EMAIL_ADDRESS,
	CR_CREATE_AND_RETURN_RECOVERY_STRING,
        CR_DELETE_USER,
        CR_PING_TYPE1,

        SR_LOGOUT_USER_FOR_COOKIE,
        SR_LOGOUT_USER_FOR_ALL_COOKIES
    };

    /***********************************************************
     * RequestId
     ***********************************************************/
    struct RequestId 
    {
        RequestId(unsigned _initiatingConnectionId,unsigned _requestNo);

        bool operator<(const RequestId& rhs_);

        unsigned  _initiatingConnectionId;
        unsigned  _requestNo;
    };

    /***********************************************************
     * ConnectionMessageContent
     ***********************************************************/
    struct ConnectionMessageContent
    {
        /**
         * The constructor
         * @param content_- 
         * @param size_- ///NEED TO WRITE\\\
         */
        ConnectionMessageContent(const char* content_,unsigned size_);

        /**
         * The constructor
         * @param type_- 
         * @param parameters_- ///NEED TO WRITE\\\
         */
        ConnectionMessageContent(ConnectionMessageType type_,
                                 const std::vector<std::string>& parameters_);

        /**
         * ///Message\\
         * @param outStream_ -
         */
        std::ostream& writeToStream(std::ostream& outStream_) const;
        /**
         * ///Message\\
         */
        unsigned getSize() const;

        ConnectionMessageType     _type;
        std::vector<std::string>  _parameters;
    };

    inline std::ostream& operator<<(std::ostream& ostream_, const ConnectionMessageContent& connectionMessage_)
    { return connectionMessage_.writeToStream(ostream_);}

    //Another classification of the ConnectionMessage
    enum class ConnectionMessageRequest
    {
        CONNECTION_MESSAGE_NEW, //new request 
        CONNECTION_MESSAGE_REPLY, //reply to a SR
        CONNECTION_DISCONNECT,
    };

}}} //closing namespace Santiago::User::Server

#endif
