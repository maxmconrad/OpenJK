/*
Testmod: new code
Client-side effects for flamethrower gadgets
*/


#include "cg_headers.h"

void FX_StopFlameThrower(gentity_t *self)
{
		G_SoundOnEnt( self, CHAN_WEAPON, "sound/effects/flameoff.mp3" );
		G_StopEffect(G_EffectIndex("boba/fthrw"), self->playerModel, self->genericBolt3, self->s.number);
}

void FX_StartFlameThrower(gentity_t *self)
{
		G_SoundOnEnt(self, CHAN_WEAPON, "sound/weapons/boba/bf_flame.mp3");
		G_PlayEffect(G_EffectIndex("boba/fthrw"), self->playerModel, self->genericBolt3, self->s.number, self->s.origin, 1);
		//G_PlayEffect(G_EffectIndex("boba/fthrw"), self->s.number, self->currentOrigin);
}

////////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////////
//void WP_DoFlameThrower(gentity_t *self)
//{
//		WP_StartFlameThrower(self);
//		WP_FireFlameThrower(self);
//}