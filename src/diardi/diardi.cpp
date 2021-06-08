//Copyright (c) 2014-2019, The Monero Project
//
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification, are
// permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this list of
//    conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice, this list
//    of conditions and the following disclaimer in the documentation and/or other
//    materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its contributors may be
//    used to endorse or promote products derived from this software without specific
//    prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL
// THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Parts of this file are originally copyright (c) 2012-2013 The Cryptonote developers

#include "diardi.h"

using namespace epee;

#undef SCALA_DEFAULT_LOG_CATEGORY
#define SCALA_DEFAULT_LOG_CATEGORY "diardi"

namespace cryptonote
{
    const std::vector<std::string> diardi::offlineSeedList = {"62.171.149.67:11811", "164.68.117.160:11811"};
    const std::vector<std::string> diardi::offlineBansList = {};
    const std::string diardi::seedsName = "seeds.scalaproject.io";
    const std::string diardi::bansName = "bans.scalaproject.io";
    const std::string diardi:: staticCheckpointsName = "static-checkpoints.scalaproject.io";

    const std::string diardi::localGatewayIPNS = "http://127.0.0.1:11815/ipns/";
    const std::string diardi::localGatewayIPFS = "http://127.0.0.1:11815/ipfs/";
    const std::string diardi::errorDat = "error:error";
    
    const std::vector<std::string> diardi::notaryNodes = {
            "alpha.scalaproject.io",
            "delta.scalaproject.io",
            "epsilon.scalaproject.io"
    };

    /* Initialize for epee HTTP client */
    epee::net_utils::http::http_simple_client client;
    epee::net_utils::http::url_content uC;

    //---------------------------------------------------------------------------
    diardi::diardi()
    {
    }
    //---------------------------------------------------------------------------
    bool diardi::getRequest(std::string& requestUrl, std::string& response){
        if (!epee::net_utils::parse_url(requestUrl, uC)){
            LOG_PRINT_L0("Failed to parse URL for diardi node " << requestUrl);
            return false;
        }

        if (uC.host.empty()){
			LOG_PRINT_L0("Failed to determine address from URL " << requestUrl);
            return false;
        }

        /* We're connecting locally so not needed */
        epee::net_utils::ssl_support_t ssl_requirement = epee::net_utils::ssl_support_t::e_ssl_support_disabled;
        uint16_t port = uC.port;
        client.set_server(uC.host, std::to_string(port), boost::none, ssl_requirement);
        epee::net_utils::http::fields_list fields;
        const epee::net_utils::http::http_response_info *info = NULL;
        if (!client.invoke_get(uC.uri, std::chrono::seconds(5), "", &info, fields)){
            LOG_PRINT_L0(requestUrl << " is not responding, skipping.");
            return false;
        }else{
            response = std::string(info->m_body);
            return true;
        }
    }
    //---------------------------------------------------------------------------
    CheckPointListType diardi::getHistoricalCheckpoints(){
        CheckPointListType m;

        std::string requestUrl = localGatewayIPNS + staticCheckpointsName;
        std::string response;

        bool tryRequest = getRequest(requestUrl, response);
        if(!tryRequest){
            LOG_PRINT_L0("Unable to get static list of checkpoints from IPFS");
        }else{
            try{
               Document checkpointsJson;
               checkpointsJson.Parse(response.c_str());

                for (rapidjson::Value::ConstValueIterator itr = checkpointsJson.Begin(); itr != checkpointsJson.End(); ++itr) {
                    if (itr->HasMember("height")) {
                        uint64_t height = (*itr)["height"].GetInt64();
                        std::stringstream hD;
                        hD << (*itr)["hash"].GetString() << ":" << (*itr)["c_difficulty"].GetString();
                        m.insert({height, hD.str()});
                    }
                }
            }
            catch(...){
                m.insert({0, "3fa5c8976978f52ad7d8fc3663e902a229a232ef987fc11ca99628366652ba99:0x1"});
            }
        }
        return m;
    }
    //---------------------------------------------------------------------------
    std::string diardi::getMajority(std::vector<std::string> &checkpoints)
    {
        uint64_t majIndex = 0;
        uint64_t count = 1;
        uint64_t vecSize = checkpoints.size();

         for(uint64_t i = 1; i < vecSize; i++)
         {
                if(checkpoints[i]==checkpoints[majIndex])
                {
                    count++;
                }
                else
                {
                    count--;
                }
                if(count == 0)
                {
                    majIndex = i;
                    count = 1;
                }
         }
        return checkpoints[majIndex];
    }
    //---------------------------------------------------------------------------
    bool diardi::checkMajority(std::vector<std::string> &checkpoints, std::string& checkpoint)
    {
          uint64_t count = 0;
          uint64_t vecSize = checkpoints.size();

          for(uint64_t i = 0; i < vecSize; i++)
          {
                if(checkpoints[i] == checkpoint)
                {
                    count++;
                }
          }

          if(count > (vecSize/2)){
              return true;
          }

          else
          {
              return false;
          }
    }
    //---------------------------------------------------------------------------
    bool diardi::getLatestCheckpoint(std::string& checkpoint)
    {
        std::vector<std::string> responses;

        for (auto &node: diardi::notaryNodes) {
            std::string requestUrl = localGatewayIPNS + node + "/latestCheckpoint.json";
            std::string response;

            bool tryRequest = getRequest(requestUrl, response);
            if(!tryRequest){
                return false;
            }else{
                responses.push_back(response);
            }
        }

        std::string mFcheckpoint = getMajority(responses);
        bool isMajority = checkMajority(responses, mFcheckpoint);

        if(isMajority){
            LOG_PRINT_L1("Most prevalent checkpoint is " << mFcheckpoint);
            checkpoint = mFcheckpoint;
            return true;
        }

        LOG_PRINT_L0("Skip checkpointing, since no checkpoint was prevalent enough");
        return false;
    }
    //---------------------------------------------------------------------------
}