#include "common/common_utils/StrictMode.hpp"
STRICT_MODE_OFF
#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#include "rpc/rpc_error.h"
STRICT_MODE_ON

#include "vehicles/multirotor/api/MultirotorRpcLibClient.hpp"
#include "common/common_utils/FileSystem.hpp"
#include <iostream>
#include <chrono>
#include <sys/time.h>

typedef unsigned long long timestamp_t;
static timestamp_t
get_timestamp ()
{
    struct timeval now;
    gettimeofday (&now, NULL);
    return  now.tv_usec + (timestamp_t)now.tv_sec * 1000000;
}

int main() 
{
    int num_queries = 250;
    using namespace msr::airlib;

    msr::airlib::MultirotorRpcLibClient client;
    typedef ImageCaptureBase::ImageRequest ImageRequest;
    typedef ImageCaptureBase::ImageResponse ImageResponse;
    typedef ImageCaptureBase::ImageType ImageType;
    typedef common_utils::FileSystem FileSystem;
    
    try
    {
        client.confirmConnection();

        std::vector<ImageRequest> img_request = { 
            ImageRequest("front_left_custom", ImageType::Scene, false, false), 
            ImageRequest("front_right_custom", ImageType::Scene, false, false), 
            ImageRequest("front_left_custom", ImageType::DepthPlanner, true),
            ImageRequest("front_center_custom", ImageType::Scene, false, false)
        };
        int idx = 0;
        while (idx < 100)
        {
            timestamp_t t0 = get_timestamp();
            const vector<ImageResponse>& response = client.simGetImages(img_request);
            // std::cout << "# of images received: " << response.size() << std::endl;
            timestamp_t t1 = get_timestamp();
            double secs_simgetimages = (t1 - t0) / 1000000.0L * 1000.0;
            std::cout << "secs_simgetimages: " << secs_simgetimages << " milliseconds"<< std::endl;
            idx++;
        }
    }

    catch (rpc::rpc_error&  e) {
        std::string msg = e.get_error().as<std::string>();
        std::cout << "Exception raised by the API, something went wrong." << std::endl << msg << std::endl;
    }

    return 0;
}
