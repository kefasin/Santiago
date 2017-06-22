#ifndef SANTIAGO_AUTHENTICATION_MULTINODE_AUTHENTICATOR_H
#define SANTIAGO_AUTHENTICATION_MULTINODE_AUTHENTICATOR_H

#include "AuthenticatorMessageSocket.h"
#include "ClientCache.h"

namespace Santiago{ namespace Authentication{ namespace MultiNode
{
    class Authenticator:public AuthenticatorBase
    {

    protected:
        
        virtual void createUserImpl(const std::string& userName_,
                                    const std::string& emailAddress_,
                                    const std::string& password_,
                                    const ErrorCodeCallbackFn& onCreateUserCallbackFn_);
        
        void handleCreateUserConnectionMessage(const std::error_code& error_,
                                               const ConnectionMessage& connectionMessage_,
                                               const onCreateUserCallbackFn& onCreateUserCallbackFn_);
        
        virtual void loginUserImpl(const std::string& userNameOrEmailAddress_,
                                   bool isUserNameNotEmailAddress_,
                                   const std::string& password_,
                                   const ErrorCodeUserInfoStringPairCallbackFn& onLoginUserCallbackFn_);

        virtual void verifyCookieAndGetUserInfoImpl(const std::string& cookieString_,
                                                    const ErrorCodeUserInfoCallbackFn& onVerifyUserCallbackFn_);

        virtual void logoutUserForCookieImpl(const std::string& cookieString_,
                                             const ErrorCodeCallbackFn& onLogoutCookieCallbackFn_);

        virtual void logoutUserForAllCookiesImpl(const std::string& userName_,
                                                 const ErrorCodeCallbackFn& onLogoutAllCookiesCallbackFn_);

        virtual void changeUserPasswordImpl(const std::string& cookieString_,
                                            const std::string& oldPassword_,
                                            const std::string& newPassword_,
                                            const ErrorCodeCallbackFn& onChangePasswordCallbackFn_);

        virtual void getUserForEmailAddressAndRecoveryStringImpl(const std::string& emailAddress_,
                                                                 const std::string& recoveryString_,
                                                                 const ErrorCodeStringCallbackFn& onGetUserForEmailAddressAndRecoveryStringCallbackFn_);

        virtual void changeUserPasswordForEmailAddressAndRecoveryStringImpl(const std::string& emailAddress_,
                                                                            const std::string& recoverystring_,
                                                                            const std::string& newPassword_,
                                                                            const ErrorCodeCallbackFn& onChangePasswordForEmailAddressAndRecoveryStringCallbackFn_);

        virtual void changeUserEmailAddressImpl(const std::string& cookieString_,
                                                const std::string& newEmailAddress_,
                                                const std::string& password_,
                                                const ErrorCodeCallbackFn& onChangeEmailAddressCallbackFn_);

        virtual void createAndReturnRecoveryStringImpl(const std::string& emailAddress_,
                                                       const ErrorCodeStringCallbackFn& onCreateAndReturnRecoveryStringCallbackFn_);

        virtual void deleteUserImpl(const std::string& cookieString_,
                                    const ErrorCodeCallbackFn& onDeleteUserCallbackFn_);
        
        std::string generateSHA256(const std::string str);

    protected:

        AuthenticatorMessageSocket         _messageSocket;
        ClientCache                        _clientCache;
    };

}}}

#endif