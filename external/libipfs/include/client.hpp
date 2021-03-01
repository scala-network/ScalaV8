#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "HTTPRequest.hpp"
#include "json.hpp"
#include <vector>
#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <future>

using json = nlohmann::json;

namespace libipfsClient {

            static std::vector<std::string> offlineSeedList {"62.171.149.67:11811", "164.68.117.160:11811"};
            static std::vector<std::string> offlineBansList {};

            static const std::string seedsName = "seeds.scalaproject.io";
            static const std::string bansName = "bans.scalaproject.io";
            static const std::string localGatewayIPNS = "http://127.0.0.1:11815/ipns/";
            static const std::string localGatewayIPFS = "http://127.0.0.1:11815/ipns/";
            static const std::string errorDat = "error:error";

            static std::vector<std::string> ldpowList 
            {
            "alpha.scalaproject.io", 
            "delta.scalaproject.io",
            "alpha.scalaproject.io", 
            "delta.scalaproject.io",
            "alpha.scalaproject.io", 
            "delta.scalaproject.io",
            "alpha.scalaproject.io", 
            "delta.scalaproject.io",
            "alpha.scalaproject.io", 
            "delta.scalaproject.io",
            "alpha.scalaproject.io", 
            "delta.scalaproject.io",
            "alpha.scalaproject.io", 
            "delta.scalaproject.io",
            "alpha.scalaproject.io", 
            "delta.scalaproject.io",
            };

    class utils {
        public:

            /**
             * @brief Get ban or seed list from IPFS
             *
             * @param seed a bool parameter that will return a std::string std::vector containing all the nodes
             *
             * @return std::string std::vector
             */

            static std::vector<std::string> getNodes(bool seed){
                std::vector<std::string> liveList;

                try{
                    std::ostringstream requestUrl;

                    if(seed == true){
                        requestUrl << localGatewayIPNS << seedsName;
                    }else{
                        requestUrl << localGatewayIPNS << bansName;
                    }

                    http::Request getNodes(requestUrl.str());
                    const http::Response gotNodes = getNodes.send("GET", "", {});

                    std::string seedListJson = std::string(gotNodes.body.begin(), gotNodes.body.end());
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
                }catch(const std::exception& e){
                    if(seed == true){
                        return offlineSeedList;
                    }else{
                        return offlineBansList;
                    }
                }
            }

             /**
             * @brief Get last checkpoint from a single node
             *
             * @param ldpowNode the IPNS name of the ldpow node to fetch from
             *
             * @return std::string containing height:hash
             */

            static std::string fetchCheckpoint(std::string ldpowNode){
                try{
                    std::ostringstream requestUrl;
                    requestUrl << localGatewayIPNS << ldpowNode << "/lastPoint.json";

                    http::Request getPoints(requestUrl.str());
                    const http::Response gotPoints = getPoints.send("GET", "", {});
                    std::string lastCheckpointJson = std::string(gotPoints.body.begin(), gotPoints.body.end());
                    auto lastCheckpoint = json::parse(lastCheckpointJson);
                    std::ostringstream element;

                    for (auto& el : lastCheckpoint.items()){
                        element << el.key() << ":" << el.value();
                    }
                    
                    return element.str();
                }catch(...){
                    return errorDat;
                }
            }

            /**
             * @brief Get last notarized checkpoint from all the LdPoW nodes
             *
             * @param none
             *
             * @return std::string
             */

            std::string getLastCheckpoint(){

                std::vector<std::future<std::string>> futures;
                std::vector<std::string> checkpointList;

                for(std::string& ldpowNode : ldpowList){
                    futures.push_back (std::async(fetchCheckpoint, ldpowNode));
                }

                for(auto &e : futures) {
                    checkpointList.push_back(e.get());
                }

                if (std::all_of(checkpointList.begin(), checkpointList.end(), [&] (std::string i) {return i == checkpointList[0];})){
                    return checkpointList.front();
                }

                return errorDat;
            }
    };
}

#endif