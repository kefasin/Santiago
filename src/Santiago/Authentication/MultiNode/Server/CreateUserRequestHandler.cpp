#include "CreateUserRequestHandler.h"

namespace Santiago{namespace Authentication { namespace MultiNode { namespace Server
{
    CreateUserRequestHandler::CreateUserRequestHandler(ConnectionServer& connectionServer_,
                                                       ServerData& serverData_,
                                                       const OnCompletedCallbackFn& onCompletedCallbackFn_,
                                                       const ConnectionMessage& initiatingMessage_)
        :RequestHandlerBase(connectionServer_,serverData_,onCompletedCallbackFn_,initiatingMessage_)
    {}

    void CreateUserRequestHandler::handleRequest()
    {
        std::pair<ControllerPtr,StrandPtr> authenticatorStrandPair =
            _serverData._authenticatorStrandPair[static_cast<int>(toupper(_initiatingMessage._parameters[0][0]))
                                                 - static_cast<int>('a')];

        std::string userName = _initiatingMessage._parameters[0];
        std::string emailAddress = _initiatingMessage._parameters[1];
        std::string password = _initiatingMessage._parameters[2];

        boost::asio::spawn(
            *authenticatorStrandPair.second,
            [authenticatorStrandPair,userName,emailAddress,password](boost::asio::yield_context yield_)
            {
                std::error_code error;
                error = authenticatorStrandPair.first->createUser(userName,emailAddres_,password,yield_);
                ConnectionMessage replyMessage(_initiatingMessage._requestId,
                                               ConnectionMessageType::SUCEEDED,
                                               std::vector<std::string>());
                if(error)
                {
                    replyMessage._type = ConnectionMessageType::FAILED;
                }
                _connectionServer.sendMessage(_initiatingMessage._requestId,replyMessage,false,boost::none);
                _onCompletedCallbackFn(_initiatingMessage._requestId);
            });
    }

}}}}