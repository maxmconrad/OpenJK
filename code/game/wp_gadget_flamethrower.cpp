/*
===========================================================================
Copyright (C) 2000 - 2013, Raven Software, Inc.
Copyright (C) 2001 - 2013, Activision, Inc.
Copyright (C) 2013 - 2015, OpenJK contributors
Copyright (C) 2020, Max-Morten Conrad

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
#include "wp_saber.h"
#include "w_local.h"
#include "../Ravl/CVec.h"

//////////////////////////////
// External variables
//////////////////////////////
extern cvar_t* g_bobaDebug;
extern pmove_t* pm;

//////////////////////////////
// External functions
//////////////////////////////
extern void CG_DrawEdge(vec3_t start, vec3_t end, int type);

////////////////////////////////////////////////////////////////////////////////////////
// Defines
////////////////////////////////////////////////////////////////////////////////////////
#define		WP_FLAMETHROWER_FLAMEDURATION			3000
#define		WP_FLAMETHROWER_FLAMETHROWRANGE		128
#define		WP_FLAMETHROWER_FLAMETHROWSIZE			40
#define		WP_FLAMETHROWER_FLAMETHROWDAMAGEMIN	1//10
#define		WP_FLAMETHROWER_FLAMETHROWDAMAGEMAX	5//40


//---------------------
//	Flamethrower Gadget
//---------------------

////////////////////////////////////////////////////////////////////////////////////////
// This func actually does the damage inflicting traces
////////////////////////////////////////////////////////////////////////////////////////
void WP_FireFlameThrower(gentity_t *self)
{
	trace_t		tr;
	vec3_t		start, end, dir;
	CVec3		traceMins(self->mins);
	CVec3		traceMaxs(self->maxs);
	gentity_t* traceEnt = NULL;
	int			damage = Q_irand(WP_FLAMETHROWER_FLAMETHROWDAMAGEMIN, WP_FLAMETHROWER_FLAMETHROWDAMAGEMAX);

	AngleVectors(self->currentAngles, dir, 0, 0);
	dir[2] = 0.0f;
	VectorCopy(self->currentOrigin, start);
	traceMins *= 0.5f;
	traceMaxs *= 0.5f;
	start[2] += 20.0f; //40.0f

	VectorMA(start, 150.0f, dir, end);


	CG_DrawEdge(start, end, EDGE_IMPACT_POSSIBLE);
	Com_Printf("Fired Flamethrower!\n");
	gi.trace(&tr, start, self->mins, self->maxs, end, self->s.number, MASK_SHOT, (EG2_Collision)0, 0);

	traceEnt = &g_entities[tr.entityNum];
	if (tr.entityNum < ENTITYNUM_WORLD && traceEnt->takedamage)
	{
		G_Damage(traceEnt, self, self, dir, tr.endpos, damage, DAMAGE_NO_ARMOR | DAMAGE_NO_KNOCKBACK | DAMAGE_NO_HIT_LOC | DAMAGE_IGNORE_TEAM, MOD_LAVA, HL_NONE);
		if (traceEnt->health > 0)
		{
			G_Knockdown( traceEnt, self, dir, Q_irand(200, 330), qfalse);
			//G_Throw(traceEnt, dir, 30);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void WP_StopFlameThrower(gentity_t *self)
{
		G_SoundOnEnt( self, CHAN_WEAPON, "sound/effects/flameoff.mp3" );
		G_StopEffect(G_EffectIndex("boba/fthrw"), self->playerModel, self->genericBolt3, self->s.number);
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void WP_StartFlameThrower(gentity_t *self)
{
		vec3_t dir;
		G_SoundOnEnt(self, CHAN_WEAPON, "sound/weapons/boba/bf_flame.mp3");
		G_PlayEffect(G_EffectIndex("boba/fthrw"), self->playerModel, self->genericBolt3, self->s.number, self->s.origin, 1);
		//G_PlayEffect(G_EffectIndex("boba/fthrw"), self->s.number, self->currentOrigin);
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
void WP_DoFlameThrower(gentity_t *self)
{
		WP_StartFlameThrower(self);
		WP_FireFlameThrower(self);
}