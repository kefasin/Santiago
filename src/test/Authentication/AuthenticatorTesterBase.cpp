#include <iostream>

#include "AuthenticatorTesterBase.h"

namespace Test{ namespace Authentication
{
    void AuthenticatorTesterBase::testAssert(const std::string& description, bool value_)
    {
        std::cout<<"description: \t "<<(value_?"[SUCCEEDED]":"[FAILED]")<<std::endl;
    }

    void  AuthenticatorTesterBase::runTests(boost::asio::yield_context yieldContext_)
    {
        //first create 3 users santiagoTestUser1, santiagoTestUser2, santiagoTestUser3
        //call spawn on run1UserTests, run2UserTests
        
        SantiagoTestUser user1,user2,user3;

        user1.userName = "user1";
        user1.password = "pass1";
        user1.emailAddress = "user1@email";

        user2.userName = "user2";
        user2.password = "pass2";
        user2.emailAddress = "user2@email";
        
        user3.userName = "user3";
        user3.password = "pass3";
        user3.emailAddress = "user3@email";


        boost::asio::spawn(_ioService, std::bind(&AuthenticatorTesterBase::run1UserTests, this, yieldContext_, user1));
        boost::asio::spawn(_ioService, std::bind(&AuthenticatorTesterBase::run1UserTests, this, yieldContext_, user2));
        boost::asio::spawn(_ioService, std::bind(&AuthenticatorTesterBase::run1UserTests, this, yieldContext_, user3));
        boost::asio::spawn(_ioService, std::bind(&AuthenticatorTesterBase::run2UserTests, this, yieldContext_, user2, user3));
        boost::asio::spawn(_ioService, std::bind(&AuthenticatorTesterBase::run2UserTests, this, yieldContext_, user1, user3));
        boost::asio::spawn(_ioService, std::bind(&AuthenticatorTesterBase::run2UserTests, this, yieldContext_, user1, user2));

    }
    
    void AuthenticatorTesterBase::run1UserTests(boost::asio::yield_context yieldContext_,
                                                const SantiagoTestUser& santiagoTestUser1)
    {
        std::error_code error;
        _authenticatorBasePtr->createUser(santiagoTestUser1.userName,
                                          santiagoTestUser1.emailAddress,
                                          santiagoTestUser1.password,
                                          yieldContext_,
                                          error);
        if(error)
        {
            std::cout << "\nCreate user error." << std::endl;
        }
        else
        {
            std::cout << "\nSuccessfully created user." << std::endl;
        }
                  
        boost::optional<std::pair<Santiago::Authentication::UserInfo, std::string> > userInfo1 = _authenticatorBasePtr->loginUser(santiagoTestUser1.userName,
                                                                                                                                  1,
                                                                                                                                  santiagoTestUser1.password,
                                                                                                                                  yieldContext_,
                                                                                                                                  error);
        if(error)
        {
            std::cout << "\nLogin user error." << std::endl;
        }
        else
        {
            std::cout << "\nSuccessfully logged in.\n\nUsername : " << userInfo1->first._userName << "\nEmail Address : " << userInfo1->first._emailAddress << std::endl;
        }
         
        boost::optional<std::pair<Santiago::Authentication::UserInfo, std::string> > userInfo2 = _authenticatorBasePtr->loginUser(santiagoTestUser1.emailAddress,
                                                                                                                                  0,
                                                                                                                                  santiagoTestUser1.password,
                                                                                                                                  yieldContext_,
                                                                                                                                  error);
        if(error)
        {
            std::cout << "\nLogin user error." << std::endl;
        }
        else
        {
            std::cout << "\nSuccessfully logged in.\n\nUsername : " << userInfo2->first._userName << "\nEmail Address : " << userInfo2->first._emailAddress << std::endl;
        }
        
        const std::string cookieString = userInfo1->second;
        _authenticatorBasePtr->logoutUserForCookie(cookieString,
                                                   yieldContext_,
                                                   error);
        if(error)
        {
            std::cout << "\nLogout user error." << std::endl;
        }
        else
        {
            std::cout << "\nSuccessfully logged out user " << userInfo1->first._userName << "." << std::endl;
        }

        _authenticatorBasePtr->logoutUserForAllCookies(santiagoTestUser1.userName,
                                                       yieldContext_,
                                                       error);
        if(error)
        {
            std::cout << "\nLogout user for all cookies error." << std::endl;
        }
        else
        {
            std::cout << "\nSuccessfully logged out user " << userInfo1->first._userName << " for all cookies." << std::endl;
        }

        _authenticatorBasePtr->changeUserPassword(cookieString,
                                                  santiagoTestUser1.password,
                                                  "new",
                                                  yieldContext_,
                                                  error);
        if(error)
        {
            std::cout << "\nChange password error." << std::endl;
        }
        else
        {
            std::cout << "\nSuccessfully changed password for the user " << userInfo1->first._userName << "." << std::endl;
        }

        _authenticatorBasePtr->changeUserEmailAddress(cookieString,
                                                      "user@gmail",
                                                      santiagoTestUser1.password,
                                                      yieldContext_,
                                                      error);
        
        if(error)
        {
            std::cout << "\nChange email address error." << std::endl;
        }
        else
        {
            std::cout << "\nSuccessfully changed email address for the user " << userInfo1->first._userName << "." << std::endl;
        }

    }
    
    void AuthenticatorTesterBase::run2UserTests(boost::asio::yield_context yieldContext_,
                                                const SantiagoTestUser& santiagoTestUser1,
                                                const SantiagoTestUser& santiagoTestUser2)
    {
        //TODO
    }
    

}}
