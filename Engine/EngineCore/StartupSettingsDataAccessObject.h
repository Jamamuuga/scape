#ifndef __STARTUPSETTINGSDATAACCESSOBJECT_H__
#define __STARTUPSETTINGSDATAACCESSOBJECT_H__

#include "ScapeEnginePrerequisites.h"

#include "StartupSettings.h"

namespace ScapeEngine
{

class _ScapeEngineExport StartupSettingsDataAccessObject
{
public:
    virtual ~StartupSettingsDataAccessObject();

    virtual const StartupSettings getStartupSettings() const = 0;
    virtual void updateStartupSettings(const StartupSettings& startupSettings) = 0;

protected:
    StartupSettingsDataAccessObject();
};
}

#endif /* __STARTUPSETTINGSDATAACCESSOBJECT_H__ */
