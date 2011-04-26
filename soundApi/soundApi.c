/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/
typedef     unsigned char           u8;
typedef     unsigned short int      u16;
typedef     unsigned int            u32;

#include "soundApi.h"

#define NULL_VALUE 0xff

#define NB_FREQUENCES 72
const u16 freqs[NB_FREQUENCES] = 
   {44, 156, 262, 363, 457, 547, 631, 710, 786, 854, 923, 986,
    1046, 1102, 1155, 1205, 1253, 1297, 1339, 1379, 1417, 1452, 1486, 1517,
    1546, 1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750, 1767, 1783,
    1798, 1812, 1825, 1837, 1849, 1860, 1871, 1881, 1890, 1899, 1907, 1915,
    1923, 1930, 1936, 1943, 1949, 1954, 1959, 1964, 1969, 1974, 1978, 1982,
    1985, 1988, 1992, 1995, 1998, 2001, 2004, 2006, 2009, 2011, 2013, 2015};

const unsigned long voices[] = {
    /*?*/
    0x02468ace, 0xfdb97531, 0x02468ace, 0xfdb97531,
    0x2064a8ec, 0xdf9b5713, 0x2064a8ec, 0xdf9b5713,

    /*double saw*/
    0x01234567, 0x89abcdef, 0x01234567, 0x89abcdef,
    0x10325476, 0x98badcfe, 0x10325476, 0x98badcfe,

    /*?*/
    0x88808fff, 0x88808880, 0x00080888, 0x80008000,
    0x8808f8ff, 0x88088808, 0x00808088, 0x08000800,

    /*?*/
    0x34455667, 0x899aabbc, 0x34455667, 0x899aabbc,
    0x43546576, 0x98a9bacd, 0x43546576, 0x98a9bacb,

    /*?*/
    0x23345667, 0x899abcde, 0x23345667, 0x899abcde,
    0x32436576, 0x98a9cded, 0x32436576, 0x98a9cded,

    /*?*/
    0xacddda48, 0x3602cf16, 0x2c04e52c, 0xacddda48,
    0xcaddad84, 0x6320fc61, 0xc2405ec2, 0xcaddad84,

    /*triangular*/
    0x10325476, 0x67452301, 0xefcdab89, 0x98badcfe,
    0x01234567, 0x76543210, 0xfedcba98, 0x89abcdef,

    /*?*/
    0x01234567, 0x89abcdef, 0xffffffff, 0xfedcba98,
    0x10325376, 0x98badcfe, 0xffffffff, 0xefcdab89,

    /*?*/
    0xf0eeca86, 0x0a468ace, 0xffeeca86, 0x0a468ace,
    0x0feeac68, 0xa064a8ec, 0xffeeac68, 0xa064a8ec,

    /*?*/
    0x7ec9cea7, 0xcfe8ab72, 0x8d757203, 0x85136317,
    0xe79cec7a, 0xfc8eba27, 0xd8572730, 0x58313671,

    /*?*/
    0xfedcba98, 0x76543210, 0x76543210, 0x44002200,
    0xefcdab89, 0x67452301, 0x67452301, 0x44002200,

    /*?*/
    0x02468ace, 0xffffffff, 0xeca86420, 0x00448844,
    0x2064a8ec, 0xffffffff, 0xce8a4602, 0x00448844

};

void snd_loadWav(u8 inst) {
    REG_DMA3SAD = (u32) & voices[inst << 2];
    REG_DMA3DAD = (u32) & REG_WAVE_RAM0;
    REG_DMA3CNT_L = 4;
    REG_DMA3CNT_H = 0x8400;
    REG_SOUND3CNT_L ^= 0x0040;
}

u16 snd_calculateTransposedFrequency (u16 freq, u8 transpose){
    freq += transpose;
    if (freq >= NB_FREQUENCES){
        // 32 + 244 = 276 
        freq /= NB_FREQUENCES;
        // 276 / 72 = 8 reste 0,6
    }
    return freq;
}

void snd_init_soundApi() {

    // activation de la puce sonore
    REG_SOUNDCNT_X = 0x80;

    // volume à fond, activation stéréo des 4 canaux
    REG_SOUNDCNT_L = 0xff77;
    REG_SOUNDCNT_H = 2;

    REG_SOUND3CNT_L = SOUND3BANK32 | SOUND3SETBANK1;
    snd_loadWav(0);

}

void snd_playSoundOnChannel1(
        u16 sweeptime, u16 sweepdir, u16 sweepshifts, u16 volume,
        u16 envdirection, u16 envsteptime, u16 waveduty, u16 soundlength,
        u16 loopmode, u16 sfreq, u8 transpose) {


    if (loopmode == 0) {
        soundlength = 0;
    }
    
    if (transpose!= NULL_VALUE){
        sfreq = snd_calculateTransposedFrequency(sfreq, transpose);
    }
    
    REG_SOUND1CNT_L = (sweeptime << 4) | (sweepdir << 3) | sweepshifts;
    REG_SOUND1CNT_H = (volume << 12) | (envdirection << 11) | (envsteptime << 8) | (waveduty << 6) | soundlength;

    REG_SOUND1CNT_X = SOUND1INIT | (loopmode << 14) | freqs[sfreq];
    REG_SOUND1CNT_X = (REG_SOUND1CNT_X & 0xf800) | (loopmode << 14) | freqs[sfreq];

    /*
        REG_SOUND1CNT_L = (sweeptime << 4)+(sweepdir << 3) + sweepshifts;
        REG_SOUND1CNT_H = (envinit << 12) + (envdirection << 11)+(envsteptime << 8)+(waveduty << 6) + soundlength;
        REG_SOUND1CNT_X = SOUND1INIT + (loopmode << 14) + sfreq;
        REG_SOUND1CNT_X = (REG_SOUND1CNT_X & 0xf800)+ (loopmode << 14) + sfreq;
     */
}

void snd_playSoundOnChannel2(u16 volume,
        u16 envdir, u16 envsteptime, u16 waveduty, u16 soundlength,
        u16 loopmode, u16 sfreq, u8 transpose) {

    if (loopmode == 0) {
        soundlength = 0;
    }
    
    if (transpose!= NULL_VALUE){
        sfreq = snd_calculateTransposedFrequency(sfreq, transpose);
    }

    REG_SOUND2CNT_L = (volume << 12) | (envdir << 11) | (envsteptime << 8) | (waveduty << 6) | soundlength;

    REG_SOUND2CNT_H = SOUND2INIT | (loopmode << 14) | freqs[sfreq];
    REG_SOUND2CNT_H = (REG_SOUND2CNT_H & 0xf800) | (loopmode << 14) | freqs[sfreq];
}

void snd_playSoundOnChannel3(u16 volume, u16 soundLength, u16 loopmode, u16 voice,
        u16 bank, u16 bankMode, u16 freq, u8 transpose) {

    REG_SOUND3CNT_L |= SOUND3PLAY;
    REG_SOUND3CNT_X |= SOUND3INIT;
    REG_SOUND3CNT_H = SOUND3OUTPUT1;

    if (bankMode == 1) { // SINGLE MODE 
        REG_SOUND3CNT_L &= ~SOUND3PLAY; //stop sound
        snd_loadWav(voice);
        snd_loadWav(voice + 2);
        REG_SOUND3CNT_L |= SOUND3PLAY;
        REG_SOUND3CNT_X |= SOUND3INIT;

        REG_SOUND3CNT_L |= SOUND3BANK32;
    } else { // DUAL MODE
        snd_loadWav(voice);
        REG_SOUND3CNT_L |= SOUND3BANK64;
    }

    if (bank == 1) {
        REG_SOUND3CNT_L |= SOUND3SETBANK1;
    } else {
        REG_SOUND3CNT_L &= 0xffbf;
    }

    switch (volume) {
        case 0:
            REG_SOUND3CNT_H = SOUND3OUTPUT0 | soundLength;
            break;
        case 1:
            REG_SOUND3CNT_H = SOUND3OUTPUT14 | soundLength;
            break;
        case 2:
            REG_SOUND3CNT_H = SOUND3OUTPUT12 | soundLength;
            break;
        case 3:
            REG_SOUND3CNT_H = SOUND3OUTPUT34 | soundLength;
            break;
        case 4:
            REG_SOUND3CNT_H = SOUND3OUTPUT1 | soundLength;
            break;
    }

    if (transpose!= NULL_VALUE){
        freq = snd_calculateTransposedFrequency(freq, transpose);
    }
    
    if (loopmode == 0) {
        REG_SOUND3CNT_X = freqs[freq] | SOUND3PLAYLOOP | SOUND3INIT;
    } else {
        REG_SOUND3CNT_X = freqs[freq] | SOUND3PLAYONCE | SOUND3INIT;
    }
}

void snd_playSoundOnChannel4(u16 volume, u16 envdir, u16 envsteptime, u16 soundlength,
        u16 loopmode, u16 shiftFreq, u16 stepping, u16 freqRatio, u8 transpose) {

    REG_SOUND4CNT_L = (volume << 12) | (envdir << 11) | (envsteptime << 8) | soundlength;

    REG_SOUND4CNT_H = SOUND4INIT | (loopmode << 14) | (shiftFreq << 4) | (stepping << 3) | freqRatio;
    REG_SOUND4CNT_H = (REG_SOUND4CNT_H & 0xF800) | (loopmode << 14) | (shiftFreq << 4) | (stepping << 3) | freqRatio;
}

void snd_stopAllSounds() {
    REG_SOUNDCNT_X = 0x0;
    //REG_SOUND1CNT_X = 0x8000;
    //REG_SOUND2CNT_H = 0x8000;
    //REG_SOUND3CNT_X = 0x8000;
    //REG_SOUND4CNT_H = 0x8000;

    snd_init_soundApi();
}
