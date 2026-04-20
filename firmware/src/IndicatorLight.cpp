#include <Arduino.h>
#include "Application.h"
#include "state_machine/DetectWakeWordState.h"
#include "state_machine/RecogniseCommandState.h"
#include "IndicatorLight.h"
#include "Speaker.h"
#include "IntentProcessor.h"

Application::Application(I2SSampler *sample_provider, IntentProcessor *intent_processor, Speaker *speaker, IndicatorLight *indicator_light)
{
    // detect wake word state
    m_detect_wake_word_state = new DetectWakeWordState(sample_provider);

    // command recogniser
    m_recognise_command_state = new RecogniseCommandState(sample_provider, indicator_light, speaker, intent_processor);

    // 初始狀態：等待 wake word
    m_current_state = m_detect_wake_word_state;
    m_current_state->enterState();

    m_speaker = speaker;

    // 存 LED 控制
    m_indicator_light = indicator_light;

    // 一開始燈關閉（等待 wake word）
    m_indicator_light->setState(OFF);
}

// process the next batch of samples
void Application::run()
{
    bool state_done = m_current_state->run();

    if (state_done)
    {
        m_current_state->exitState();

        // 🔁 狀態切換
        if (m_current_state == m_detect_wake_word_state)
        {
            // 偵測到 wake word → 開燈
            m_indicator_light->setState(ON);

            m_current_state = m_recognise_command_state;

            // 播放提示音
            m_speaker->playOK();
        }
        else
        {
            // 辨識完成 → 關燈
            m_indicator_light->setState(OFF);

            m_current_state = m_detect_wake_word_state;
        }

        m_current_state->enterState();
    }

    vTaskDelay(10);
}
