#include <Arduino.h>
#include "Application.h"
#include "state_machine/DetectWakeWordState.h"
#include "state_machine/RecogniseCommandState.h"
#include "IndicatorLight.h"
#include "Speaker.h"
#include "IntentProcessor.h"

Application::Application(I2SSampler *sample_provider, IntentProcessor *intent_processor, Speaker *speaker, IndicatorLight *indicator_light)
{
    // 存 LED
    m_indicator_light = indicator_light;

    // detect wake word state
    m_detect_wake_word_state = new DetectWakeWordState(sample_provider);

    // recognise command state
    m_recognise_command_state = new RecogniseCommandState(sample_provider, indicator_light, speaker, intent_processor);

    // 初始狀態：等待 wake word
    m_current_state = m_detect_wake_word_state;

    // LED 預設關閉
    m_indicator_light->setState(OFF);

    m_current_state->enterState();

    m_speaker = speaker;
}

// process the next batch of samples
void Application::run()
{
    bool state_done = m_current_state->run();

    if (state_done)
    {
        m_current_state->exitState();

        // 如果剛剛是 wake word -> 切到辨識
        if (m_current_state == m_detect_wake_word_state)
        {
            m_current_state = m_recognise_command_state;

            // 👉 LED 開（開始處理）
            m_indicator_light->setState(ON);

            m_speaker->playOK();
        }
        else
        {
            // 👉 LED 關（結束回待機）
            m_indicator_light->setState(OFF);

            m_current_state = m_detect_wake_word_state;
        }

        m_current_state->enterState();
    }

    vTaskDelay(10);
}
