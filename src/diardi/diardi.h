// Copyright (c) 2014-2020, The Scala Project
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

#pragma once

#include <algorithm>
#include <map>
#include <chrono>
#include "misc_log_ex.h"
#include "crypto/hash.h"
#include "cryptonote_config.h"
#include "net/http_client.h"
#include "rapidjson/document.h"

using namespace rapidjson;

namespace cryptonote
{
  /**
   * @brief A container for getting data from the Diardi network
   *
   * Diardi is the network of notary nodes that publishes checkpoints on
   * a periodic basis, functions defined here will fetch data from diardi
   * from there and make it available for the daemon.
   */
  class diardi
  {
      public:
        /**
         * @brief default constructor
        */
        diardi();

        /**
         * @brief Get the latest checkpoint from all diardi nodes
         *
         * @param checkpoint if the request is a success this will contain the valid checkpoint that can be inserted by the daemon
         *
         * @return false if the checkpoints are bad or if the requests fail.
         *         true if the requests succeeds.
         *
        */

        bool getLatestCheckpoint(std::string& checkpoint);

      private:

        /**
         * @brief Send a GET request
         *
         * @param url the url of the request to be sent
         * @param response this will contain the response from the server
         *
         * @return false if the request fails.
         *         true if the request succeeds.
         *
        */

        bool getRequest(std::string& url, std::string& response);

        static const std::vector<std::string> offlineSeedList;
        static const std::vector<std::string> offlineBansList;
        static const std::string seedsName;
        static const std::string bansName;
        static const std::string localGatewayIPNS;
        static const std::string localGatewayIPFS;
        static const std::string errorDat;
        static const std::vector<std::string> notaryNodes;
  };
}