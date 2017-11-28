#include "LogoutUserForAllCookiesRequestHandler.h"

namespace Santiago{ namespace Authentication { namespace Server
{
    
    LogoutUserForAllCookiesRequestHandler::LogoutUserForAllCookiesRequestHandler(ConnectionServer& connectionServer_,
                                                                                 ServerData& serverData_,
                                                                                 const OnCompletedCallbackFn& onCompletedCallbackFn_,
                                                                                 const ConnectionMessage& initiatingMessage_)
        :RequestHandlerBase(connectionServer_,serverData_,onCompletedCallbackFn_,initiatingMessage_)
    {}
    
    void LogoutUserForAllCookiesRequestHandler::handleRequest()
    {
        std::string cookieString = _connectionMessage._parameters[0];
        std::string userName;
        boost::optional<SantiagoDBTables::UsersRec> sessionsRec = serverData_._databaseConnection.get().getSessionsRec(cookieString,error);
        if(sessionsRec)
        {
            userName = sessionsRec._userName;
        }
        std::pair<ControllerPtr,StrandPtr> authenticatorStrandPair =
            _serverData._authenticatorStrandPair[static_cast<int>(toupper(userName[0]))
                                                 - static_cast<int>('a')];
        
        boost::asio::spawn(
            *authenticatorStrandPair.second,
            [authenticatorStrandPair,cookieString](boost::asio::yield_context yield_)
            {
                std::error_code error;
                error = authenticatorStrandPair.first->logoutUserForAllCookies(cookieString,yield_);
                ConnectionMessage replyMessage(connectionMessage._requestId,
                                               ConnectionMessageType::SUCEEDED,
                                               std::vector<std::string>());
                if(error)
                {
                    replyMessage._type = ConnectionMessageType::FAILED;
                }
                _connectionServer.sendMessage(serverMessage,false,boost::none);
                _onCompletedCallbackFn(_initiatingMessage._requestId);
            });
    }
            
}}}
