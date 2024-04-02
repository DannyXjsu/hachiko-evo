#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "explode.h"

class CFuncTrampoline : public CBaseEntity
{
public:
	void Spawn() override;
	void Touch(CBaseEntity *pOther);
	void Precache();

	void BounceSound();

	static const char* pBounceSounds[];
};

LINK_ENTITY_TO_CLASS(func_trampoline, CFuncTrampoline);

const char* CFuncTrampoline::pBounceSounds[] =
	{
		"common/bodydrop1.wav",
		"common/bodydrop2.wav",
		"common/bodydrop3.wav",
		"common/bodydrop4.wav",
};

void CFuncTrampoline::Precache() {
	PRECACHE_SOUND_ARRAY(pBounceSounds);
}

void CFuncTrampoline::BounceSound()
{
	int pitch = 95 + RANDOM_LONG(0, 9);
	EMIT_SOUND_DYN(ENT(pev), CHAN_STATIC, RANDOM_SOUND_ARRAY(pBounceSounds), 1.0, ATTN_NORM, 0, pitch);
}

void CFuncTrampoline::Spawn()
{
	Precache();
	pev->movetype = MOVETYPE_NONE;
	pev->solid = SOLID_BBOX;
	SET_MODEL(edict(), STRING(pev->model));
}

void CFuncTrampoline::Touch(CBaseEntity *pOther)
{
	Vector position = pOther->pev->origin + (pOther->pev->maxs + pOther->pev->mins) * 0.5f;
    //ExplosionCreate(position, pev->angles, edict(), 50, false);
	Vector velocity = pOther->pev->velocity;
	if (velocity.z < -128.0f && pev->origin.z <= position.z)
	{
		BounceSound();
		velocity.z = velocity.z * -0.9f;
		Vector dir = (pev->origin - position).Normalize();
		velocity.x += dir.x * velocity.z * 0.25f;
		velocity.y += dir.y * velocity.z * 0.25f;

		pOther->pev->velocity.z = velocity.z * pev->impulse;
	}
	else
	{
		velocity.z *= 0.75f;
	}
	velocity.x *= 0.8f;
	velocity.y *= 0.8f;

	pOther->pev->velocity.x = velocity.x;
	pOther->pev->velocity.y = velocity.y;
}