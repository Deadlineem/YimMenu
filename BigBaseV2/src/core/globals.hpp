#pragma once
#include "enums.hpp"

#ifndef GLOBALS_H
#define GLOBALS_H

using namespace big;
struct globals {
	nlohmann::json default_options;
	nlohmann::json options;

	struct self {
		bool godmode = false;
		bool noclip = false;
	};

	struct vehicle {
		bool horn_boost = false;
		SpeedoMeter speedo_meter = SpeedoMeter::DISABLED;
	};

	struct weapons {
		CustomWeapon custom_weapon = CustomWeapon::NONE;
		char vehicle_gun_model[12] = "bus";
	};

	self self{};
	vehicle vehicle{};
	weapons weapons{};

	void from_json(const nlohmann::json& j)
	{
		this->self.godmode = j["self"]["godmode"];

		this->vehicle.horn_boost = j["vehicle"]["horn_boost"];
		this->vehicle.speedo_meter = (SpeedoMeter)j["vehicle"]["speedo_meter"];

		this->weapons.custom_weapon = (CustomWeapon)j["weapons"]["custom_weapon"];
	}

	nlohmann::json to_json()
	{
		return nlohmann::json{
			{
				"self", {
					{ "godmode", this->self.godmode }
				}
			},
			{
				"vehicle", {
					{ "horn_boost", this->vehicle.horn_boost },
					{ "speedo_meter", (int)this->vehicle.speedo_meter }
				}
			},
			{
				"weapons", {
					{ "custom_weapon", (int)this->weapons.custom_weapon }
				}
			}
		};
	}

	void attempt_save()
	{
		nlohmann::json& j = this->to_json();
		if (j != this->options)
		{
			this->save();

			this->options = j;
		}
	}

	bool load()
	{
		this->default_options = this->to_json();

		std::string settings_file = std::getenv("appdata");
		settings_file += "\\BigBaseV2\\settings.json";

		std::ifstream file(settings_file);

		if (!file.is_open())
		{
			this->write_default_config();

			file.open(settings_file);
		}

		file >> this->options;

		bool should_save = false;
		for (auto& e : this->default_options.items())
		{
			if (this->options.count(e.key()) == 0)
			{
				should_save = true;
				this->options[e.key()] = e.value();
			}
		}

		this->from_json(this->options);

		if (should_save)
		{
			LOG(INFO) << "Updating settings.";
			save();
		}

		return true;
	}

	bool save()
	{
		std::string settings_file = std::getenv("appdata");
		settings_file += "\\BigBaseV2\\settings.json";

		std::ofstream file(settings_file, std::ios::out | std::ios::trunc);
		file << this->to_json().dump(4);
		file.close();

		return true;
	}

	bool write_default_config()
	{
		std::string settings_file = std::getenv("appdata");
		settings_file += "\\BigBaseV2\\settings.json";

		std::ofstream file(settings_file, std::ios::out);
		file << this->to_json().dump(4);
		file.close();

		return true;
	}
};

inline struct globals g;
#endif // !GLOBALS_H