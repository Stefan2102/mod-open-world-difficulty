#include "OWD.h"
#include "WorldScript.h"

// ==========================================
// OWD WORLD SCRIPT
// ==========================================

class OWD_WorldScript : public WorldScript
{
public:
    OWD_WorldScript() : WorldScript("OWD_WorldScript", {
        WORLDHOOK_ON_AFTER_CONFIG_LOAD
    }) { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        sOWD->LoadConfig();
    }
};

// ==========================================
// OWD WORLD SCRIPT REGISTRATION
// ==========================================

void AddSC_OWD_WorldScript()
{
    new OWD_WorldScript();
}
