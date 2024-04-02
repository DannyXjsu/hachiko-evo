#include "extdll.h"
#include "util.h"
#include "cbase.h"

class CTriggerPrint : public CBaseEntity
{
public:
	// Gets called when the entity spawns in world
	void Spawn() override;
	// Gets called when this entity is triggered by another entity or player
	void Use(CBaseEntity *pActivator, CBaseEntity *pCaller, USE_TYPE useType, float value) override;

};

LINK_ENTITY_TO_CLASS(trigger_print, CTriggerPrint);

void CTriggerPrint::Spawn()
{
	// do nothing here 
}

void CTriggerPrint::Use(CBaseEntity* pActivator, CBaseEntity* pCaller, USE_TYPE useType, float value)
{
	ALERT(at_console, "%s \n", STRING(pev->message));
}