#include "gaugecar.h"
#include "gaugecarplugin.h"

#include <QtPlugin>

GaugeCarPlugin::GaugeCarPlugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void GaugeCarPlugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here

    m_initialized = true;
}

bool GaugeCarPlugin::isInitialized() const
{
    return m_initialized;
}

QWidget *GaugeCarPlugin::createWidget(QWidget *parent)
{
    return new GaugeCar(parent);
}

QString GaugeCarPlugin::name() const
{
    return QLatin1String("GaugeCar");
}

QString GaugeCarPlugin::group() const
{
    return QLatin1String("MyWidget");
}

QIcon GaugeCarPlugin::icon() const
{
    return QIcon(":/guagecar.png");
}

QString GaugeCarPlugin::toolTip() const
{
    return QLatin1String("");
}

QString GaugeCarPlugin::whatsThis() const
{
    return QLatin1String("");
}

bool GaugeCarPlugin::isContainer() const
{
    return false;
}

QString GaugeCarPlugin::domXml() const
{
    return QLatin1String("<widget class=\"GaugeCar\" name=\"gaugeCar\">\n</widget>\n");
}

QString GaugeCarPlugin::includeFile() const
{
    return QLatin1String("gaugecar.h");
}
#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(gaugecarplugin, GaugeCarPlugin)
#endif // QT_VERSION < 0x050000
