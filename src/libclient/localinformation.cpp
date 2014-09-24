#include "localinformation.h"

LocalInformation::LocalInformation()
{
}

QVariantMap LocalInformation::getVariables() const
{
    QVariantMap map;

    map.insert("cpu_usage", deviceInfo.cpuUsage());
    map.insert("free_memory", deviceInfo.freeMemory());
    map.insert("signal_strength", deviceInfo.signalStrength());
    map.insert("battery_level", deviceInfo.batteryLevel());
    map.insert("available_disk_space", deviceInfo.availableDiskSpace());
    map.insert("connection_mode", networkMangager.connectionMode());

    return map;
}

QVariantMap LocalInformation::getConstants() const
{
    QVariantMap map;

    map.insert("device_id", deviceInfo.deviceId());
    map.insert("os_name", deviceInfo.OSName());
    map.insert("os_version", deviceInfo.OSVersion());
    map.insert("firmware_version", deviceInfo.firmwareVersion());
    map.insert("board", deviceInfo.board());
    map.insert("manufacturer", deviceInfo.manufacturer());
    map.insert("model", deviceInfo.model());

    return map;
}