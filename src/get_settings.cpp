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
#include "common/AirSimSettings.hpp"

typedef msr::airlib::AirSimSettings AirSimSettings;

bool readSettingsTextFromFile(std::string settingsFilepath, std::string& settingsText) 
{
	// check if path exists
    bool found = std::ifstream(settingsFilepath.c_str()).good(); 
    if (found)
    {
		std::ifstream ifs(settingsFilepath);
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		// todo airsim's simhud.cpp does error checking here
        settingsText = buffer.str();// todo convert to utf8 as done in simhud.cpp?
    }

    return found;
}

bool getSettingsText(std::string& settingsText) 
{
    bool success = readSettingsTextFromFile(msr::airlib::Settings::Settings::getUserDirectoryFullPath("settings.json"), settingsText);
    return success;
}

std::string getSimMode()
{
	return "Multirotor";
}

// mimics void ASimHUD::initializeSettings()
bool initializeSettings()
{
    std::string settingsText;
    if (getSettingsText(settingsText))
    {
        AirSimSettings::initializeSettings(settingsText);

        // not sure where settings_json initialized in AirSimSettings::initializeSettings() is actually used
        Settings& settings_json = Settings::loadJSonString(settingsText);
        std::string simmode_name = settings_json.getString("SimMode", "");
        std::cout << "simmode_name: " << simmode_name << std::endl; 

		AirSimSettings::singleton().load(std::bind(&getSimMode));

		// iterate over std::map<std::string, std::unique_ptr<VehicleSetting>> vehicles;
		for (const auto &curr_vehicle_elem : AirSimSettings::singleton().vehicles)
		{
			auto& vehicle_setting = curr_vehicle_elem.second;
			std::cout << "vehicle_name:" << curr_vehicle_elem.first << '\t' << std::endl;
			std::cout << vehicle_setting->vehicle_name << '\t' << std::endl;
			std::cout << vehicle_setting->vehicle_type << '\t' << std::endl;
			std::cout << vehicle_setting->position << '\t' << std::endl;
			// std::cout << vehicle_setting->rotation << '\t' << std::endl;

			// iterate over camera map std::map<std::string, CameraSetting> cameras;
			for (const auto &curr_camera_elem : vehicle_setting->cameras)
			{
				auto& camera_setting = curr_camera_elem.second;
				std::cout << "camera_name:" << curr_camera_elem.first << std::endl;
				std::cout << camera_setting.position << '\t' << std::endl;
				// camera_setting.gimbal
				// std::cout << camera_setting->rotation << '\t' << std::endl;
				
				// iterate over capture_setting std::map<int, CaptureSetting> capture_settings
				for (const auto &curr_capture_elem : camera_setting.capture_settings)
				{
					auto& capture_setting = curr_capture_elem.second;

					// todo why does AirSimSettings::loadCaptureSettings calls AirSimSettings::initializeCaptureSettings()
					// which initializes default capture settings for _all_ NINE msr::airlib::ImageCaptureBase::ImageType
					if (!(std::isnan(capture_setting.fov_degrees)))
					{
						std::cout << "image_type:" << capture_setting.image_type << std::endl;
						std::cout << "fov_degrees:" << capture_setting.fov_degrees << std::endl;
						std::cout << "publish_to_ros:" << capture_setting.publish_to_ros << std::endl;
					}

				}
			}

			// iterate over sensors std::map<std::string, std::unique_ptr<SensorSetting>> sensors;
		} 

    }
    else
    {
    	return false;
    }
}


int main() 
{
	bool success = initializeSettings();
	return 0;
}