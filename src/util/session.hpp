#pragma once
#include "core/data/session_types.hpp"
#include "script_global.hpp"
#include "script.hpp"
#include "natives.hpp"
#include "util/misc.hpp"
#include "util/globals.hpp"
#include "gta/joaat.hpp"
#include "rage/rlSessionByGamerTaskResult.hpp"
#include "pointers.hpp"
#include "services/players/player_service.hpp"
#include "services/player_database/player_database_service.hpp"

namespace big::session
{
	inline void join_type(eSessionType session)
	{
		*script_global(2726795).as<int*>() = (session == eSessionType::SC_TV ? 1 : 0); // If SC TV Then Enable Spectator Mode

		if (session == eSessionType::LEAVE_ONLINE)
			*script_global(1574589).at(2).as<int*>() = -1;
		else
			*script_global(1575015).as<int*>() = (int)session;

		*script_global(1574589).as<int*>() = 1;
		script::get_current()->yield(200ms);
		*script_global(1574589).as<int*>() = 0;
	}

	static constexpr char const* weathers[] = {
		"EXTRASUNNY", "CLEAR", "CLOUDS", "SMOG",
		"FOGGY", "OVERCAST", "RAIN", "THUNDER",
		"CLEARING", "NEUTRAL", "SNOW", "BLIZZARD",
		"SNOWLIGHT", "XMAS", "HALLOWEEN" };
	inline void local_weather()
	{
		MISC::CLEAR_OVERRIDE_WEATHER();

		MISC::SET_OVERRIDE_WEATHER(weathers[g->session.local_weather]);

		*script_global(262145).at(4723).as<bool*>() = g->session.local_weather == 13;
	}

	inline void set_fm_event_index(int index)
	{
		int idx = index / 32;
		int bit = index % 32;
		misc::set_bit(scr_globals::gsbd_fm_events.at(11).at(341).at(idx, 1).as<int*>(), bit);
		misc::set_bit(scr_globals::gsbd_fm_events.at(11).at(348).at(idx, 1).as<int*>(), bit);
		misc::set_bit(scr_globals::gpbd_fm_3.at(self::id, scr_globals::size::gpbd_fm_3).at(10).at(205).at(idx, 1).as<int*>(), bit);
	}

	inline void clear_fm_event_index(int index)
	{
		int idx = index / 32;
		int bit = index % 32;
		misc::clear_bit(scr_globals::gsbd_fm_events.at(11).at(341).at(idx, 1).as<int*>(), bit);
		misc::clear_bit(scr_globals::gsbd_fm_events.at(11).at(348).at(idx, 1).as<int*>(), bit);
		misc::clear_bit(scr_globals::gpbd_fm_3.at(self::id, scr_globals::size::gpbd_fm_3).at(10).at(205).at(idx, 1).as<int*>(), bit);
	}

	inline void join_session(const rage::rlSessionInfo& info)
	{
		g->session.join_queued = true;
		g->session.info = info;
		session::join_type({ eSessionType::NEW_PUBLIC });
		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(RAGE_JOAAT("maintransition")) == 0)
		{
			g->session.join_queued = false;
			g_notification_service->push_error("RID Joiner", "Unable to launch maintransition");
		}
		return;
	}
  
	inline void join_by_rockstar_id(uint64_t rid)
	{
		if (SCRIPT::GET_NUMBER_OF_THREADS_RUNNING_THE_SCRIPT_WITH_THIS_HASH(RAGE_JOAAT("maintransition")) != 0 ||
			STREAMING::IS_PLAYER_SWITCH_IN_PROGRESS())
		{
			g_notification_service->push_error("RID Joiner", "Player switch in progress, wait a bit.");
			return;
		}

		rage::rlGamerHandle player_handle(rid);
		rage::rlSessionByGamerTaskResult result;
		bool success = false;
		int state = 0;
		if (g_pointers->m_start_get_session_by_gamer_handle(0, &player_handle, 1, &result, 1, &success, &state))
		{
			while (state == 1)
				script::get_current()->yield();

			if (state == 3 && success)
			{
				join_session(result.m_session_info);
				return;
			}
		}

		g_notification_service->push_error("RID Joiner", "Target player is offline?");
	}

	inline void add_infraction(player_ptr player, Infraction infraction)
	{
		auto plyr = g_player_database_service->get_or_create_player(player);
		if (!plyr->infractions.contains((int)infraction))
		{
			plyr->is_modder = true;
			player->is_modder = true;
			plyr->infractions.insert((int)infraction);
			g_player_database_service->save();
		}
	}

	inline void give_collectible(Player target, eCollectibleType col, int index = 0, bool uncomplete = false)
	{
		const size_t arg_count = 7;
		int64_t args[arg_count] = {
			(int64_t)eRemoteEvent::GiveCollectible,
			(int64_t)self::id,
			(int64_t)col, // iParam0
			(int64_t)index, // iParam1
			!uncomplete, // bParam2
			true,
			0  // bParam3
		};

		g_pointers->m_trigger_script_event(1, args, arg_count, 1 << target);
	}

	// TODO this is really broken
	inline void enter_player_interior(player_ptr player)
	{
		if (*scr_globals::globalplayer_bd.at(player->id(), scr_globals::size::globalplayer_bd).at(318).at(6).as<int*>() == -1)
		{
			g_notification_service->push_error("Enter Interior", "Player does not seem to be in an interior");
			return;
		}

		int owner = *scr_globals::globalplayer_bd.at(player->id(), scr_globals::size::globalplayer_bd).at(318).at(9).as<int*>();
		if (owner == -1)
			owner = player->id();

		*script_global(1946250).at(3607).as<int*>() = 0;
		*script_global(1946250).at(3605).as<int*>() = 1;
		*script_global(1946250).at(4703).as<int*>() = 1;
		*script_global(1946250).at(3218).as<int*>() = 1;
		*script_global(1946250).at(3214).as<int*>() = 1;
		*script_global(1946250).at(3612).as<int*>() = 1;

		// misc::set_bit(script_global(1946250).at(1).as<int*>(), 22);
		misc::set_bit(script_global(1946250).as<int*>(), 6);
		misc::clear_bit(script_global(1946250).at(1).as<int*>(), 9);

		*script_global(1946250).at(3280).as<int*>() = owner;
		*script_global(1946250).at(3606).as<int*>() = *scr_globals::globalplayer_bd.at(player->id(), scr_globals::size::globalplayer_bd).at(318).at(6).as<int*>();
	}
}