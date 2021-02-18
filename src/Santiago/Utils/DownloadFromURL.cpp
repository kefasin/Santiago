#include <curl/curl.h>

#include "STLog.h"

#include "DownloadFromURL.h"

namespace Santiago{ namespace Utils
{
    namespace
    {
        std::size_t callback(
            const char* in,
            std::size_t size,
            std::size_t num,
            std::string* out)
        {
            const std::size_t totalBytes(size * num);
            out->append(in, totalBytes);
            return totalBytes;
        }
    }
    
    boost::optional<std::string> TypicalSettingDownloadFromURL(const std::string& url_)
    {
        CURL* curl = curl_easy_init();
        
        // Set remote URL.
        curl_easy_setopt(curl, CURLOPT_URL, url_.c_str());
        
        // Don't bother trying IPv6, which would increase DNS resolution time.
        curl_easy_setopt(curl, CURLOPT_IPRESOLVE, CURL_IPRESOLVE_V4);
        
        // Don't wait forever, time out after 10 seconds.
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
        
        // Follow HTTP redirects if necessary.
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        
        // Response information.
        int httpCode(0);
        std::string httpData;
        
        // Hook up data handling function.
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callback);
        
        // Hook up data container (will be passed as the last parameter to the
        // callback handling function).  Can be any pointer type, since it will
        // internally be passed as a void pointer.
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &httpData);
        
        // Run our HTTP GET command, capture the HTTP response code, and clean up.
        curl_easy_perform(curl);
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpCode);
        curl_easy_cleanup(curl);
        
        if (httpCode == 200)
        {
            ST_LOG_INFO("\nGot successful response from " << url_ << std::endl);
            return httpData;
        }
        else
        {
            ST_LOG_INFO("Couldn't GET from " << url_ << " - exiting" << std::endl);
            return boost::none;
        }
    }
}}