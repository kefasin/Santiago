#ifndef SANTIAGO_USER_SERVER_SERVER_H
#define SANTIAGO_USER_SERVER_SERVER_H

/**
 * @file ServerV1.h
 *
 * @section DESCRIPTION
 *
 * Contains the Server class  
 */


#include <boost/asio/socket_acceptor_service.hpp>
#include <boost/asio/error.hpp>
#include <boost/optional.hpp>

#include "ConnectionServer.h"
#include "ServerData.h"

using boost::asio::ip::tcp;

namespace Santiago{ namespace User { namespace Server
{
    class Server
    {
    public:

        typedef std::shared_ptr<RequestHandlerBase> RequestHandlerBasePtr;
        
        /**
         * The constructor
         * @param ioService_- 
         * @param port_ -
         */
        Server(boost::asio::io_service& ioService_,unsigned port_);
       /**
        * ///Message\\
        */
        void start();

    protected:
       /**
        * ///Message\\
        * @param connectionId - 
        */
        void handleDisconnect(unsigned connectionId_);
       /**
        * ///Message\\
        * @param message_- 
        */
        void handleRequestNew(const ServerMessage& message_);
       /**
        * ///Message\\
        * @param message_- 
        */
        void handleRequestReply(const ServerMessage& message_);
       /**
        * ///Message\\
        * @param requestId_- 
        */
        void handleRequestCompleted(const RequestId& requestId_);

        std::map<RequestId,RequestHandlerBasePtr>   _activeRequestHandlersList;
        boost::asio::io_service&                    _ioService;
        unsigned                                    _port;
        ServerData                                  _serverData;
        ConnectionServer                            _connectionServer;
    };
}}}


#endif