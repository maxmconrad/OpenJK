/*
===========================================================================
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors

This file is part of the OpenJK source code.

OpenJK is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <http://www.gnu.org/licenses/>.
===========================================================================
*/

#include "g_local.h"
#include "b_local.h"
#include "g_functions.h"
#include "wp_saber.h"
#include "w_local.h"

//--------------------------------------------
//	Wookiee Bowcaster aka EE-3 blaster carbine
//--------------------------------------------

//---------------------------------------------------------
void WP_FireBowcasterMissile(gentity_t* ent, vec3_t start, vec3_t dir, qboolean altFire)
//---------------------------------------------------------
{
	// set up Bowcaster as EE-3 rifle missile, similar to E-11 but more accturate and powerful
	int velocity = BOWCASTER_VELOCITY;
	int	damage = altFire ? weaponData[WP_BOWCASTER].altDamage : weaponData[WP_BOWCASTER].damage;

	// If an enemy is shooting at us, lower the velocity so you have a chance to evade
	if (ent->client && ent->client->ps.clientNum != 0 && ent->client->NPC_class != CLASS_BOBAFETT)
	{
		if (g_spskill->integer < 2)
		{
			velocity *= BLASTER_NPC_VEL_CUT;
		}
		else
		{
			velocity *= BLASTER_NPC_HARD_VEL_CUT;
		}
	}

	WP_TraceSetStart(ent, start, vec3_origin, vec3_origin);//make sure our start point isn't on the other side of a wall

	WP_MissileTargetHint(ent, start, dir);

	gentity_t* missile = CreateMissile(start, dir, velocity, 10000, ent, altFire);

	missile->classname = "bowcaster_proj";
	missile->s.weapon = WP_BOWCASTER;

	// Do the damages
	if (ent->s.number != 0)
	{
		if (g_spskill->integer == 0)
		{
			damage = BOWCASTER_NPC_DAMAGE_EASY;
		}
		else if (g_spskill->integer == 1)
		{
			damage = BOWCASTER_NPC_DAMAGE_NORMAL;
		}
		else
		{
			damage = BOWCASTER_NPC_DAMAGE_HARD;
		}
	}

	missile->damage = damage;
	missile->dflags = DAMAGE_DEATH_KNOCKBACK;
	if (altFire)
	{
		missile->methodOfDeath = MOD_BOWCASTER_ALT;
	}
	else
	{
		missile->methodOfDeath = MOD_BOWCASTER;
	}
	missile->clipmask = MASK_SHOT | CONTENTS_LIGHTSABER;

	// we don't want it to bounce forever
	missile->bounceCount = 8;
}

//---------------------------------------------------------
void WP_BowcasterAltFire(gentity_t* ent, qboolean alt_fire)
//---------------------------------------------------------
{
	vec3_t		start;
	VectorCopy(ent->client->renderInfo.eyePoint, start);
	AngleVectors(ent->client->renderInfo.eyeAngles, forwardVec, NULL, NULL);
	WP_FireBowcasterMissile(ent, start, forwardVec, alt_fire);
}

//----------------------------------------------------------
void WP_BowcasterMainFire(gentity_t *ent, qboolean alt_fire)
//----------------------------------------------------------
{
	WP_FireBowcasterMissile(ent, muzzle, forwardVec, alt_fire);
}

//---------------------------------------------------------
void WP_FireBowcaster( gentity_t *ent, qboolean alt_fire )
//---------------------------------------------------------
{
	if ( alt_fire )
	{
		WP_BowcasterAltFire( ent, alt_fire );
	}
	else
	{
		WP_BowcasterMainFire( ent, alt_fire );
	}
}
