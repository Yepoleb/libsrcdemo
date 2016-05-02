#pragma once
#include <stdint.h>
#include <string>

#include "common.hpp"

const uint16_t SND_NOFLAGS = 0;
const uint16_t SND_CHANGE_VOL = (1<<0);
const uint16_t SND_CHANGE_PITCH = (1<<1);
const uint16_t SND_STOP = (1<<2);
const uint16_t SND_SPAWNING = (1<<3);
const uint16_t SND_DELAY = (1<<4);
const uint16_t SND_STOP_LOOPING = (1<<5);
const uint16_t SND_SPEAKER = (1<<6);
const uint16_t SND_SHOULDPAUSE = (1<<7);
const uint16_t SND_IGNORE_PHONEMES = (1<<8);
const uint16_t SND_IGNORE_NAME = (1<<9);

const uint8_t PITCH_NORM = 100;
const uint8_t PITCH_LOW = 95;
const uint8_t PITCH_HIGH = 120;

const size_t MAX_SNDLVL_BITS = 9;
const size_t MAX_SOUND_DELAY_MSEC_ENCODE_BITS = 13;
const size_t SOUND_SEQNUMBER_BITS = 10;
const size_t SND_FLAG_BITS_ENCODE = 11;
const size_t COORD_INTEGER_BITS = 14;

const float SOUND_DELAY_OFFSET = 0.100f;
const uint16_t SNDLVL_NORM = 75;

const float DEFAULT_SOUND_PACKET_VOLUME = 1.0f;
const uint8_t DEFAULT_SOUND_PACKET_PITCH = 100;
const float DEFAULT_SOUND_PACKET_DELAY = 0.0f;

enum Channel
{
    CHAN_REPLACE = -1,
    CHAN_AUTO = 0,
    CHAN_WEAPON = 1,
    CHAN_VOICE = 2,
    CHAN_ITEM = 3,
    CHAN_BODY = 4,
    CHAN_STREAM = 5,
    CHAN_STATIC = 6,
    CHAN_VOICE2 = 7,
    CHAN_VOICE_BASE = 8,
    CHAN_USER_BASE = (CHAN_VOICE_BASE + 128)
};

struct SoundInfo
{
    SoundInfo();
    void setDefault();
    void clearStopFields();
    void readDelta(const SoundInfo& delta, BitBuffer& buf);
    std::string toString() const;

    uint16_t entity_index;
    uint16_t soundnum;
    uint16_t flags;
    Channel channel;

    bool is_ambient;
    bool is_sentence;

    uint16_t seq_num;
    float volume;
    uint16_t sound_lvl;
    uint8_t pitch;
    uint8_t special_dsp;
    float delay;
    Vector origin;
    int16_t speaker_entity;
};
