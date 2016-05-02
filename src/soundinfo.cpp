#include <stdint.h>
#include <string>
#include <sstream>

#include "bitbuffer.hpp"
#include "soundinfo.hpp"

SoundInfo::SoundInfo()
{
    setDefault();
}

void SoundInfo::setDefault()
{
    entity_index = 0;
    soundnum = 0;
    flags = 0;
    channel = CHAN_STATIC;

    is_ambient = false;
    is_sentence = false;

    seq_num = 0;
    volume = DEFAULT_SOUND_PACKET_VOLUME;
    sound_lvl = SNDLVL_NORM;
    pitch = DEFAULT_SOUND_PACKET_PITCH;
    delay = DEFAULT_SOUND_PACKET_DELAY;
    special_dsp = 0;
    origin = {0.0f, 0.0f, 0.0f};
}

void SoundInfo::clearStopFields()
{
    seq_num = 0;

    volume = 0.0f;
    sound_lvl = SNDLVL_NORM;
    pitch = PITCH_NORM;
    special_dsp = 0;
    delay = 0.0f;
    origin = {0.0f, 0.0f, 0.0f};
}

void SoundInfo::readDelta(const SoundInfo& delta, BitBuffer& buf)
{
    if (not buf.readBool()) {
        entity_index = delta.entity_index;
    } else {
        if (buf.readBool()) {
            entity_index = buf.readBits(5);
        } else {
            entity_index = buf.readBits(MAX_EDICT_BITS);
        }
    }

    if (buf.readBool()) {
        if (NETPROTOCOL_VERSION > 22) {
            soundnum = buf.readBits(MAX_SOUND_INDEX_BITS);
        } else {
            soundnum = buf.readBits(13);
        }
    } else {
        soundnum = delta.soundnum;
    }

    if (buf.readBool()) {
        if (NETPROTOCOL_VERSION > 18) {
            flags = buf.readBits(SND_FLAG_BITS_ENCODE);
        } else {
            flags = buf.readBits(9);
        }
    } else {
        flags = delta.flags;
    }

    if (buf.readBool()) {
        channel = static_cast<Channel>(buf.readBits(3));
    } else {
        channel = delta.channel;
    }

    is_ambient = buf.readBool();
    is_sentence = buf.readBool();

    if (flags == SND_STOP) {
        clearStopFields();
        return;
    }

    if (buf.readBool()) {
        seq_num = delta.seq_num;
    } else if (buf.readBool()) {
        seq_num = delta.seq_num + 1;
    } else {
        seq_num = buf.readBits(SOUND_SEQNUMBER_BITS);
    }

    if (buf.readBool()) {
        volume = buf.readBits(7) / 127.0f;
    } else {
        volume = delta.volume;
    }

    if (buf.readBool()) {
        sound_lvl = buf.readBits(MAX_SNDLVL_BITS);
    } else {
        sound_lvl = delta.sound_lvl;
    }

    if (buf.readBool()) {
        pitch = buf.readBits(8);
    } else {
        pitch = delta.pitch;
    }

    if (NETPROTOCOL_VERSION > 21) {
        if (buf.readBool()) {
            special_dsp = buf.readBits(8);
        } else {
            special_dsp = delta.special_dsp;
        }
    }

    if (buf.readBool()) {
        int16_t delay_msec = buf.readSBits(MAX_SOUND_DELAY_MSEC_ENCODE_BITS);
        delay = delay_msec / 1000.0f;

        if (delay < 0) {
            delay *= 10.0f;
        }

        delay -= SOUND_DELAY_OFFSET;
    } else {
        delay = delta.delay;
    }

    if (buf.readBool()) {
        origin.x = buf.readSBits(COORD_INTEGER_BITS - 2) * 8.0f;
    } else {
        origin.x = delta.origin.x;
    }
    if (buf.readBool()) {
        origin.y = buf.readSBits(COORD_INTEGER_BITS - 2) * 8.0f;
    } else {
        origin.y = delta.origin.y;
    }
    if (buf.readBool()) {
        origin.z = buf.readSBits(COORD_INTEGER_BITS - 2) * 8.0f;
    } else {
        origin.z = delta.origin.z;
    }
    if (buf.readBool()) {
        speaker_entity = buf.readSBits(MAX_EDICT_BITS + 1);
    } else {
        speaker_entity = delta.speaker_entity;
    }
}

std::string SoundInfo::toString() const
{
    std::stringstream ss;
    ss << std::boolalpha;
    ss << "entity_index: " << entity_index << std::endl;
    ss << "soundnum: " << soundnum << std::endl;
    ss << "flags: " << std::hex << flags << std::dec << std::endl;
    ss << "channel: " << (int)channel << std::endl;
    ss << "is_ambient: " << is_ambient << std::endl;
    ss << "is_sentence: " << is_sentence << std::endl;
    ss << "seq_num: " << seq_num << std::endl;
    ss << "volume: " << volume << std::endl;
    ss << "sound_lvl: " << sound_lvl << std::endl;
    ss << "pitch: " << (int)pitch << std::endl;
    ss << "delay: " << delay << std::endl;
    ss << "origin.x: " << origin.x << std::endl;
    ss << "origin.y: " << origin.y << std::endl;
    ss << "origin.z: " << origin.z << std::endl;
    ss << "speaker_entity: " << speaker_entity << std::endl;
    return ss.str();
}
