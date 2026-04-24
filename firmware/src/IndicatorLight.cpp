#include <Arduino.h>
#include "IndicatorLight.h"

IndicatorLight::IndicatorLight()
{
    m_state = OFF;
    m_taskHandle = NULL;
}

void IndicatorLight::setState(IndicatorState state)
{
    m_state = state;

    if (state == OFF)
    {
        Serial1.print("{870100}");
    }
    else if (state == ON)
    {
        Serial1.print("{8701ff}");
    }
    else if (state == PULSING)
    {
        Serial1.print("{8701ff}");
    }
}

IndicatorState IndicatorLight::getState()
{
    return m_state;
}
