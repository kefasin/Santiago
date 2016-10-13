#ifndef FASTCGI_REQUEST_H
#define FASTCGI_REQUEST_H
/**
 * @file Fastcgi/Request.h
 *
 * @section DESCRIPTION
 *
 * Contains the Fastcgi::Request class.
 */

#include <mutex>
#include <stdexcept>
#include <thread>

#include <boost/asio.hpp>

#include "Common.h"
#include "RequestData.h"
#include "Connection.h"

namespace Santiago{namespace Fastcgi
{
/**
 * The class which will be used by the users of the Fastcgi module to read and write to
 * a fastcgi request.It takes the boost asio protocol tags as the template argument. 
 * This class object will be created by the acceptor class and not directly by the user.
 */
    template<class Protocol>
    class Request
    {
    public:
        typedef std::shared_ptr<RequestData> RequestDataPtr;
        typedef std::weak_ptr<RequestData> RequestDataWeakPtr;

        typedef std::shared_ptr<Connection<Protocol> > ConnectionPtr;
        typedef std::weak_ptr<Connection<Protocol> > ConnectionWeakPtr;

        typedef std::pair<unsigned,unsigned> RequestId;

        /**
         * Delete the copy constructor to make the class private
         */
        Request(const Request&) = delete;

        /**
         * Delete the copy assignment operator to make the class private
         */
        Request& operator=(const Request&) = delete;

        /**
         * The constructor
         * @param ioService_- the ioservice of the acceptor
         * @param requestId - the requestId of the request
         * @param dataWeakptr_- weak ptr to the data
         * @param connectionId - the id given to the connection by the acceptor
         * @param connectionWeakPtr - weak ptr to the connection
         */
        Request(boost::asio::io_service& ioService_,
                uint requestId_,
                RequestDataWeakPtr dataWeakPtr_,
                uint connectionId_,
                ConnectionWeakPtr connectionWeakPtr_):
            _requestId(requestId_),
            _dataWeakPtr(dataWeakPtr_),
            _connectionId(connectionId_),
            _connectionWeakPtr(connectionWeakPtr_),
            _hasReplied(false)
        {}

        /**
         * Returns the requestId which is a pair of connectionId and RequestId.
         */
        RequestId getId()
        {
            return std::pair<uint,uint>(_connectionId,_requestId);
        }

        /**
         * Tells if the request and the connection has not be aborted/disconnected.
         * @return true if the request is still valid
         */
        bool isValid()
        {
            return ((!_hasReplied) && (_dataWeakPtr.lock() != NULL) && (_connectionWeakPtr.lock() != NULL));
        }

        /**
         * Commits the data in the request and closes the request.Can be called from 
         * any thread is thread safe. Please note that this function actually need not
         * be thread safe as it just post's the data in the connection's strand. Here
         * the multiple commits from different threads are weeded out. But making this
         * thread safe using lock will allow for giving error messages to the user on
         * multiple writes from multiple threads.
         */
        void commit()
        {
            std::lock_guard<std::mutex> lock(_commitMutex);
            std::pair<ConnectionPtr,RequestDataPtr> connectionRequestPair = getConnectionAndRequestData();
            connectionRequestPair.first->commitReply(_requestId,connectionRequestPair.second);            
            _dataWeakPtr.reset();
            _hasReplied = true;
        }

        /**
         * Closes the request.Can be called from any thread is thread safe. Is similar
         * to commit except that all the data in the out buffer is emptied before
         * flushing. 
         */
        void cancel()
        {
            std::lock_guard<std::mutex> lock(_commitMutex);
            std::pair<ConnectionPtr,RequestDataPtr> connectionRequestPair = getConnectionAndRequestData();
            connectionRequestPair.second->_outBuffer.consume(connectionRequestPair.second->_outBuffer.size());
            connectionRequestPair.second->_err<<"Request cancelled by application server";
            connectionRequestPair.first->commitReply(_requestId,connectionRequestPair.second);            
            _dataWeakPtr.reset();
            _hasReplied = true;
        }

        /**
         * Sets the appstatus to be replied
         * @param status_
         */
        void setAppStatus(uint status_)
        {
            getConnectionAndRequestData().second->_appStatus = status_;
        }

        /**
         * returns the stdin buffer
         */
        const std::string& getStdinBuffer()
        {
            return getConnectionAndRequestData().second->_in;
        }

        /**
         * returns the parameter map
         */
        RequestData::ParamsMap& getFCGIParams()
        {
            return getConnectionAndRequestData().second->_paramsMap;
        }

        /**
         * returns the get name value pairs. //TODO---
         */
        const std::map<std::string,std::string>& getGetData() const;
        {
            return _dataWeakPtr->_requestGetData;
        }
        

        /**
         * returns the post name value pairs. //TODO
         */
        const std::map<std::string,std::string>& getPostData() const
        {
            return _dataWeakPtr->_requestPostData;
        }

        /**
         * returns the cookie name value params received from server. //TODO
         */
        const std::map<std::string,std::string>& getHTTPCookiesReceived() const
        {
            return _dataWeakPtr->_requestHTTPCookies;
        }

        /**
         * sets the content type to be sent to the user. Default Html
         */
        void setContentMIMEType(MIMEType contentType_);

        /**
         * returns cookies to be sent to the user //TODO
         */
        std::map<std::string,HTTPCookieData>& responseHTTPCookies();

        /**
         * the error buffer of the request
         */
        std::ostream& err()
        {
            return getConnectionAndRequestData().second->_err;
        }

        /**
         * the out buffer of the request
         */
        std::ostream& out()
        {
            return getConnectionAndRequestData().second->_out;
        }

       
        /**
         * The destructor
         */
        ~Request()
        {
            try
            {
                if(isValid())
                {
                    cancel();
                }
            }
            catch(...)
            {
                BOOST_ASSERT(false);
            }
        }

    private:
        /**
         * Converts the connection and data weak ptrs to shared ptrs and returns. 
         * Inability to convert indicates that the request/connection has be
         * aborted/disconnected/flushed. An exception is called then which is passed 
         * unhandled to the user by the public functions which calls this functions.
         */
        std::pair<ConnectionPtr,RequestDataPtr> getConnectionAndRequestData()
        {
            if(_hasReplied)
            {
                throw std::runtime_error("The request has already been committed or cancelled");
            }
            std::pair<ConnectionPtr,RequestDataPtr> ret(_connectionWeakPtr.lock(),_dataWeakPtr.lock());
            if(ret.first == NULL || ret.second == NULL)
            {
                throw std::runtime_error("The request is not active or aborted");
            }
            return ret;
        }

        std::mutex _commitMutex;
        uint _requestId;
        RequestDataWeakPtr _dataWeakPtr;
        uint _connectionId;
        ConnectionWeakPtr _connectionWeakPtr;
        bool _hasReplied;

    };    
}}

#endif
