#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "explode.h"
#include <algorithm>

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
	pev->solid = SOLID_TRIGGER;
	SET_MODEL(edict(), STRING(pev->model));
}

void CFuncTrampoline::Touch(CBaseEntity *pOther)
{
	using namespace std;

	Vector position = pOther->pev->origin + (pOther->pev->maxs + pOther->pev->mins) * 0.5f;
    //ExplosionCreate(position, pev->angles, edict(), 50, false);
	Vector velocity = pOther->pev->velocity;

	// Meeting a minimum vertical speed to jump and making sure that only the bottom of the \
	entity's bounding box can trigger the jump
	if (velocity.z < -128.0f && pev->origin.z <= position.z - pev->size.z * 0.9f)
	{
		int iViewPunchDir = 1;
		if (fabs(velocity.x) > 128.0f || fabs(velocity.x) > 128.0f)
			iViewPunchDir = -1;
		pOther->pev->punchangle.x = clamp(velocity.z * 0.02f * pev->impulse * iViewPunchDir, -98.4f, 98.4f);
		iViewPunchDir = RANDOM_LONG(-1, 1);
		pOther->pev->punchangle.z = clamp(velocity.z * 0.005f * pev->impulse * iViewPunchDir, -98.4f, 98.4f);
		BounceSound();
		velocity.z *= -1.0f;
		Vector dir = (pev->origin - position).Normalize();
		velocity.x += dir.x * velocity.z * 0.15f;
		velocity.y += dir.y * velocity.z * 0.15f;

		pOther->pev->velocity.z = velocity.z * pev->dmg;
	}
	else
	{
		// Apply movement friction when we are not jumping (walking inside the entity)
		velocity.x *= 0.5f;
		velocity.y *= 0.5f;
	}

	pOther->pev->velocity.x = velocity.x;
	pOther->pev->velocity.y = velocity.y;
}