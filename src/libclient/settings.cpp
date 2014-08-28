#include "settings.h"
#include "deviceinfo.h"
#include "log/logger.h"

#include <QSettings>
#include <QCryptographicHash>

LOGGER(Settings);

class Settings::Private
{
public:
    void sync();

    GetConfigResponse config;
    QSettings settings;
};

void Settings::Private::sync()
{
    settings.setValue("config", config.toVariant());
    settings.sync();
}

Settings::Settings(QObject *parent)
: QObject(parent)
, d(new Private)
{
}

Settings::~Settings()
{
    d->sync();
    delete d;
}

Settings::StorageType Settings::init()
{
    bool newSettings = deviceId().isNull();

    DeviceInfo info;

    QString generatedDeviceId = info.deviceId();

    if (generatedDeviceId.isEmpty())
    {
        if (deviceId().isEmpty())
        {
            QCryptographicHash hash(QCryptographicHash::Sha224);
            hash.addData(QUuid::createUuid().toByteArray());

            setDeviceId(QString::fromLatin1(hash.result().toHex()));

            LOG_INFO("Generated fallback device ID");
        }
        else
        {
            LOG_INFO("Took fallback device ID from config");
        }
    }
    else
    {
        setDeviceId(generatedDeviceId);
    }

    LOG_INFO(QString("Device ID: %1").arg(deviceId()));

    // Create new settings
    if (newSettings)
    {
        d->config.setConfigAddress("supervisor.measure-it.net:80");
        setAvailableTraffic(31457280);  // 30 MiB
        setAvailableMobileTraffic(20971520);  // 20 MiB
        setUsedTraffic(0);
        setUsedMobileTraffic(0);
        LOG_INFO("Created new settings for this device");

        return NewSettings;
    }
    else
    {
        d->config.fillFromVariant(qvariant_cast<QVariantMap>(d->settings.value("config")));

        // Always set the controller address if we have none
        if (d->config.configAddress().isEmpty())
        {
            LOG_WARNING("Controller address lost, setting back default one");
            d->config.setConfigAddress("supervisor.measure-it.net:80");
        }

        LOG_INFO("Loaded existing settings for this device");
        return ExistingSettings;
    }
}

bool Settings::hasLoginData() const
{
    return !userId().isEmpty() && !apiKey().isEmpty();
}

void Settings::setDeviceId(const QString &deviceId)
{
    if (this->deviceId() != deviceId)
    {
        d->settings.setValue("device-id", deviceId);
        emit deviceIdChanged(deviceId);
    }
}

QString Settings::deviceId() const
{
    return d->settings.value("device-id").toString();
}

void Settings::setUserId(const QString &userId)
{
    if (this->userId() != userId)
    {
        d->settings.setValue("user-id", userId);
        emit userIdChanged(userId);
    }
}

QString Settings::userId() const
{
    return d->settings.value("user-id").toString();
}

void Settings::setPassword(const QString &password)
{
    if (this->password() != password)
    {
        d->settings.setValue("password", password);
        emit passwordChanged(password);
    }
}

QString Settings::password() const
{
    return d->settings.value("password").toString();
}

void Settings::setApiKey(const QString &apiKey)
{
    if (this->apiKey() != apiKey)
    {
        d->settings.setValue("api-key", apiKey);
        emit apiKeyChanged(apiKey);
    }
}

QString Settings::apiKey() const
{
    return d->settings.value("api-key", "0").toString();
}

bool Settings::isPassive() const
{
    return d->settings.value("passive").toBool();
}

void Settings::setPassive(bool passive)
{
    if (this->isPassive() != passive)
    {
        d->settings.setValue("passive", passive);
        emit passiveChanged(passive);
    }
}

void Settings::setAvailableTraffic(quint32 traffic)
{
    if (this->availableTraffic() != traffic)
    {
        d->settings.setValue("available-traffic", traffic);
        emit availableTrafficChanged(traffic);
    }
}

quint32 Settings::availableTraffic() const
{
    return d->settings.value("available-traffic", 0).toUInt();
}

void Settings::setAvailableMobileTraffic(quint32 traffic)
{
    if (this->availableMobileTraffic() != traffic)
    {
        d->settings.setValue("available-mobile-traffic", traffic);
        emit availableMobileTrafficChanged(traffic);
    }
}

quint32 Settings::availableMobileTraffic() const
{
    return d->settings.value("available-mobile-traffic", 0).toUInt();
}

void Settings::setUsedTraffic(quint32 traffic)
{
    if (this->usedTraffic() != traffic)
    {
        d->settings.setValue("used-traffic", traffic);
        emit usedTrafficChanged(traffic);
    }
}

quint32 Settings::usedTraffic() const
{
    return d->settings.value("used-traffic", 0).toUInt();
}

void Settings::setUsedMobileTraffic(quint32 traffic)
{
    if (this->usedMobileTraffic() != traffic)
    {
        d->settings.setValue("used-mobile-traffic", traffic);
        emit usedMobileTrafficChanged(traffic);
    }
}

quint32 Settings::usedMobileTraffic() const
{
    return d->settings.value("used-mobile-traffic", 0).toUInt();
}

GetConfigResponse *Settings::config() const
{
    return &d->config;
}

void Settings::sync()
{
    d->sync();
}
