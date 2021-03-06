/**
 * Giliam de Carpentier, Copyright (c) 2007.
 * Licensed under the Simplified BSD license.
 * See Docs/ScapeLicense.txt for details.
 */

#include "ScapeEngineStableHeaders.h"
#include "SkySettings.h"
#include "Input/InputManager.h"

using namespace ScapeEngine;


enum EPropertyId
{
    PROPERTYID_SKYCOLOR,
    PROPERTYID_SKYOVERBRIGHT,
    PROPERTYID_SUNHEIGHT,
    PROPERTYID_SUNDIRECTION,
    PROPERTYID_SUNCOLOR,
    PROPERTYID_SUNOVERBRIGHT,
    PROPERTYID_FOGCOLOR,
    PROPERTYID_FOGDISTANCE,

    PROPERTYID_SKYBOX_TEXTURE
};

bool SkySettings::initPersistentElementValueMap(StringStringMap& map)
{
    setUIElementPropertyValueMap(getUIElementPresetPropertyValueMap(defaultPresetString));
    return true;
}

bool SkySettings::initPersistentElementStringEnumMap(StringEnumMap& map)
{
    // lighting properties
    ADD_STRINGENUM(map, PROPERTYID_, SKYCOLOR);
    ADD_STRINGENUM(map, PROPERTYID_, SKYOVERBRIGHT);
    ADD_STRINGENUM(map, PROPERTYID_, SUNCOLOR);
    ADD_STRINGENUM(map, PROPERTYID_, SUNOVERBRIGHT);
    ADD_STRINGENUM(map, PROPERTYID_, SUNHEIGHT);
    ADD_STRINGENUM(map, PROPERTYID_, SUNDIRECTION);
    ADD_STRINGENUM(map, PROPERTYID_, FOGCOLOR);
    ADD_STRINGENUM(map, PROPERTYID_, FOGDISTANCE);

    ADD_STRINGENUM(map, PROPERTYID_, SKYBOX_TEXTURE);

    return true;
}

string SkySettings::setUIElementPropertyValue(const string& elementName, const string& value)
{
    EPropertyId propertyId =
        (EPropertyId)Utils::findEnumFromStringEnumMap(getPersistentElementStringEnumMap(), elementName);
    Ogre::Real realValue = Ogre::StringConverter::parseReal(value);

    string outValue;

    switch (propertyId)
    {
    case PROPERTYID_SKYCOLOR:
    {
        mSkyColor = Utils::getColourValueFromString(value);
        outValue = value;
        break;
    }
    case PROPERTYID_SKYOVERBRIGHT:
    {
        realValue = Utils::clamp(realValue, 1.f, 100.0f);
        mSkyOverbright = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_SUNCOLOR:
    {
        mSunColor = Utils::getColourValueFromString(value);
        outValue = value;
        break;
    }
    case PROPERTYID_SUNOVERBRIGHT:
    {
        realValue = Utils::clamp(realValue, 1.f, 100.0f);
        mSunOverbright = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_FOGCOLOR:
    {
        mFogColor = Utils::getColourValueFromString(value);
        outValue = value;
        break;
    }
    case PROPERTYID_FOGDISTANCE:
    {
        realValue = Utils::clamp(realValue, 1.f, 100000.0f);
        mFogDistance = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_SUNHEIGHT:
    {
        realValue = Utils::clamp(realValue, 0.0f, 90.0f);
        mSunHeight = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }
    case PROPERTYID_SUNDIRECTION:
    {
        realValue = Utils::clamp(realValue, -10000.f, 10000.0f);
        mSunDirection = realValue;
        outValue = Ogre::StringConverter::toString(realValue);
        break;
    }

    case PROPERTYID_SKYBOX_TEXTURE:
    {
        outValue = value;
        mSkyBoxTextureName = outValue;
        break;
    }

    default:
    {
        Ogre::LogManager::getSingleton().logMessage(
            _T("SkySettings's setUIElementPropertyValue(): unknown elementName '") + elementName + _T("'"));
        break;
    }
    }

    getPersistentElementValueMap()[elementName] = outValue;

    notifyListeners();

    return outValue;
}

SkySettings::SkySettings()
    : mSkyColor(Ogre::ColourValue::White), mSkyOverbright(1.0f), mSunColor(Ogre::ColourValue::White),
      mSunOverbright(1.0f), mSunDirection(0.0f), mSunHeight(0.0f), mFogColor(Ogre::ColourValue::White),
      mFogDistance(1.0E6f), mSkyBoxTextureName()
{
    wantsPreFrameTick();
    loadPersistentProperties();
}

void SkySettings::addListener(SkySettingsListener* listener) { mListeners.push_front(listener); }

void SkySettings::removeListener(SkySettingsListener* listener)
{
    SkySettingsListeners::iterator it, itEnd = mListeners.end();
    for (it = mListeners.begin(); it != itEnd; ++it)
    {
        if (*it == listener)
        {
            mListeners.erase(it);
            break;
        }
    }
}

void SkySettings::onPreFrameTick()
{
    InputManager* inputManager = getEngineCore()->getInputManager();
    if (inputManager->getButton(ButtonId::LIGHT_CHANGEDIRECTION)->isPressed())
    {
        Ogre::Real deltaX = getEngineCore()->getTimeSinceLastFrame() *
                            inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAX);
        Ogre::Real deltaY = getEngineCore()->getTimeSinceLastFrame() *
                            inputManager->getAnalogInput(AnalogInputId::POINTER_DELTAY);

        setUIElementPropertyValue(_T("SUNHEIGHT"),
                                  Ogre::StringConverter::toString(mSunHeight - 5.0f * deltaY));
        setUIElementPropertyValue(_T("SUNDIRECTION"),
                                  Ogre::StringConverter::toString(mSunDirection + deltaX * 10.0f));
    }
}

void SkySettings::notifyListeners()
{
    SkySettingsListeners::iterator it, itEnd = mListeners.end();
    for (it = mListeners.begin(); it != itEnd; ++it)
    {
        (*it)->onSkySettingsUpdate();
    }
}