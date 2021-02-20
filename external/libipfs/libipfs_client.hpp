#ifndef LIBIPFS_CLIENT
#define LIBIPFS_CLIENT

#include "HTTPRequest.hpp"
#include "json.hpp"
#include <vector>
#include <iostream>
#include <sstream>

using namespace std;
using json = nlohmann::json;

namespace libipfsClient {
    class utils {
        public:
            /**
             * @brief Get ban or seed list from IPFS
             *
             * @param seed a bool parameter that will return a string vector containing all the nodes
             *
             * @return string vector
             */

            vector<string> getNodes(bool seed){
                vector<string> liveList;

                try{
                    ostringstream requestUrl;

                    if(seed == true){
                        requestUrl << localGatewayIPNS << seedsName;
                    }else{
                        requestUrl << localGatewayIPNS << bansName;
                    }

                    http::Request getNodes(requestUrl.str());
                    const http::Response gotNodes = getNodes.send("GET", "", {});

                    string seedListJson = string(gotNodes.body.begin(), gotNodes.body.end());
                    auto seedList = json::parse(seedListJson);
                    if(seed == true){
                            for (auto& node : seedList["seed_nodes"])
                            {
                                liveList.push_back(node);
                            }
                    }else{
                            for (auto& node : seedList["banned_nodes"])
                            {
                                liveList.push_back(node);
                            }
                    }
                    return liveList;
                }catch(const exception& e){
                    if(seed == true){
                        return offlineSeedList;
                    }else{
                        return offlineBansList;
                    }
                }
            }
        private:
            vector<string> offlineSeedList {"62.171.149.67:11811", "164.68.117.160:11811"};
            vector<string> offlineBansList {};

            const string seedsName = "seeds.scalaproject.io";
            const string bansName = "bans.scalaproject.io";
            const string localGatewayIPNS = "http://127.0.0.1:8082/ipns/";
            const string localGatewayIPFS = "http://127.0.0.1:8082/ipns/";
    };
}

#endif
