#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <notification/notification_app.h>
#include "ArduboyTones.h"

class ArduboyAudio {
public:
    void begin() {
        const bool system_enabled = systemAudioEnabled();

        if(system_enabled) {
            on();
        } else {
            off();
        }
    }

    void on() {
        bool was_enabled = g_arduboy_audio_enabled;
        g_arduboy_audio_enabled = true;
        if(!was_enabled) {
            arduboy_tone_sound_system_init();
        }
    }

    void off() {
        bool was_enabled = g_arduboy_audio_enabled;
        g_arduboy_audio_enabled = false;
        if(was_enabled) {
            ArduboyToneSoundRequest req = {.pattern = NULL};
            if(g_arduboy_sound_queue) (void)furi_message_queue_put(g_arduboy_sound_queue, &req, 0);
            arduboy_tone_sound_system_deinit();
        }
    }

    bool enabled() const {
        return g_arduboy_audio_enabled;
    }

    bool systemAudioEnabled() const {
        if(furi_hal_rtc_is_flag_set(FuriHalRtcFlagStealthMode)) return false;

        NotificationApp* notification = (NotificationApp*)furi_record_open(RECORD_NOTIFICATION);
        const float speaker_volume = notification->settings.speaker_volume;
        furi_record_close(RECORD_NOTIFICATION);

        return speaker_volume > 0.001f;
    }

    void saveOnOff() {
        // Audio state is driven by current OS/session settings.
    }
};
