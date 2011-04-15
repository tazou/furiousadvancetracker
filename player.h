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
#ifndef _PLAYER_H_
#define	_PLAYER_H_

#include "soundApi/soundApi.h"
#include "data.h"

// DOC: Délai pour 1/16e de mesure = (60000 / bpm) / 4.
// BPM 128 -> 1 note = 117,18
// BPM 255 -> 1 note = 58,82

#define TEMPO_TIMER_HARDWARE_VALUE 70
#define WAIT_FOR_START 10

/**
 * Toutes ces variables sont des repères pour le player. Afin de savoir quelle séquence/block/note jouer.
 * Attention. Les valeurs dans les tableaux représentent des NUMEROS DE LIGNES. 
 * Ce ne sont pas les ids des séquences/blocks/notes. 
 * Les 3 autres variables représentent, elles, des ids d'objets.
 */
u8 actualSequencesForChannel[6], actualBlocksForChannel[6], actualNotesForChannel[6];
u8 FAT_currentPlayedSequence = NULL_VALUE,
        FAT_currentPlayedChannel = NULL_VALUE, FAT_currentPlayedBlock = NULL_VALUE;

u8 FAT_cursor_player3_obj, FAT_cursor_player2_obj;
u8 FAT_cursor_playerSequences_obj[6];

u32 tempoReach = TEMPO_TIMER_HARDWARE_VALUE;

u8 iCanPressStart = 1, waitForStart = 0;

/**
 * Function timerisée qui lit toutes les séquences. 
 */
void FAT_player_timerFunc_playSequences();
/**
 * Function timerisée qui lit tous les blocks d'une séquence. 
 */
void FAT_player_timerFunc_playBlocks();
/**
 * Function timerisée qui lit toutes les notes d'un block. 
 */
void FAT_player_timerFunc_playNotes();

void FAT_player_moveOrHideCursor(u8 channel);
void FAT_player_hideAllCursors();
void FAT_player_hideSequencesCursors();
void FAT_player_hideBlockCursor();
void FAT_player_hideNoteCursor();

void FAT_player_init();

/**
 * Initialisation du curseur player pour les notes.
 * S'occupe de créer le sprite (TRANSPARENCE) et de le rendre invisible. 
 */
void FAT_player_initCursors() {
    FAT_cursor_player3_obj = ham_CreateObj((void*) cursor3_player_Bitmap, OBJ_SIZE_32X16,
            OBJ_MODE_SEMITRANSPARENT, 1, 0, 0, 0, 0, 0, 0, 0, 0);
    FAT_cursor_player2_obj = ham_CreateObj((void*) cursor2_player_Bitmap, OBJ_SIZE_32X16,
            OBJ_MODE_SEMITRANSPARENT, 1, 0, 0, 0, 0, 0, 0, 0, 0);

    ham_SetObjVisible(FAT_cursor_player3_obj, 0);
    ham_SetObjVisible(FAT_cursor_player2_obj, 0);
    ham_SetObjPrio(FAT_cursor_player3_obj, 3);
    ham_SetObjPrio(FAT_cursor_player2_obj, 3);

    // on clone les sprites pour la lecture des 6 séquences à la fois
    for (u8 i = 0; i < 6; i++) {
        FAT_cursor_playerSequences_obj[i] = ham_CloneObj(FAT_cursor_player2_obj, 0, 0);
        ham_SetObjPrio(FAT_cursor_playerSequences_obj[i], 3);
    }
}

void FAT_player_timerFunc_iCanPressStart() {
    waitForStart ++;
    if (waitForStart >= WAIT_FOR_START && !iCanPressStart) {
        iCanPressStart = 1;
        waitForStart = 0;
        M_TIM0CNT_IRQ_DISABLE
        M_TIM0CNT_TIMER_STOP
    }
}

/**
 * Joue une note sur un canal.
 * @param note
 * @param channel
 */
void FAT_player_playNote(note* note, u8 channel) {
    if (note->freq != NULL_VALUE) {
        instrument* inst = &(FAT_tracker.allInstruments[note->instrument]);
        u16 sweepshifts = (inst->sweep & 0x70) >> 4;
        u16 sweeptime = (inst->sweep & 0x0F);
        u16 sweepdir = 1;
        if (sweeptime > 7) {
            sweeptime -= 8;
            sweepdir = 0;
        }

        switch (channel) {
            case 0: // PU1
                //ham_DrawText (23, 16, "PU1");
                snd_playSoundOnChannel1(
                        sweeptime, sweepdir, sweepshifts,
                        inst->volume, inst->envdirection, inst->envsteptime, inst->wavedutyOrPolynomialStep,
                        inst->soundlength, inst->loopmode,
                        freqs[note->freq]);
                break;
            case 1: // PU2
                //ham_DrawText (23, 16, "PU2");
                snd_playSoundOnChannel2(inst->volume, inst->envdirection, inst->envsteptime, inst->wavedutyOrPolynomialStep,
                        inst->soundlength, inst->loopmode,
                        freqs[note->freq]);
                break;

            case 2: // WAV
                snd_playSoundOnChannel3(inst->volumeRatio, inst->soundlength, inst->loopmode, inst->voice,
                        inst->bank, inst->bankMode, freqs[note->freq]);
                break;
            case 3: // NOISE
                //ham_DrawText (23, 16, "NOI");
                snd_playSoundOnChannel4(inst->volume, inst->envdirection, inst->envsteptime, inst->soundlength,
                        inst->loopmode, note->octave, inst->wavedutyOrPolynomialStep, note->freq / NB_FREQUENCES);
                break;
        }

    }
}

/**
 * Lance la lecture de toute la track.
 */
void FAT_player_startPlayerFromSequences(u8 startLine) {

    if (iCanPressStart) {
        iCanPressStart = 0;

        // initialisation des séquences au démarrage
        memset(actualSequencesForChannel, startLine, sizeof (u8)*6);
        memset(actualBlocksForChannel, 0, sizeof (u8)*6);
        memset(actualNotesForChannel, 0, sizeof (u8)*6);

        tempoReach = ((60000 / FAT_tracker.tempo) / 4) - TEMPO_TIMER_HARDWARE_VALUE;
        FAT_isCurrentlyPlaying = 1;
        ham_StartIntHandler(INT_TYPE_TIM3, (void*) &FAT_player_timerFunc_playSequences);

        R_TIM3CNT = 0;
        M_TIM3CNT_IRQ_ENABLE
        M_TIM3CNT_TIMER_START

        ham_StartIntHandler(INT_TYPE_TIM0, (void*) &FAT_player_timerFunc_iCanPressStart);

        R_TIM0CNT = 3;
        M_TIM0CNT_IRQ_ENABLE
        M_TIM0CNT_TIMER_START
    }

}

void FAT_player_startPlayerFromBlocks(u8 sequenceId, u8 startLine, u8 channel) {

    if (iCanPressStart) {
        iCanPressStart = 0;

        // initialisation 
        memset(actualSequencesForChannel, NULL_VALUE, sizeof (u8)*6);
        memset(actualBlocksForChannel, 0, sizeof (u8)*6);
        memset(actualNotesForChannel, 0, sizeof (u8)*6);
        actualBlocksForChannel[channel] = startLine;
        FAT_currentPlayedSequence = sequenceId;
        FAT_currentPlayedChannel = channel;

        tempoReach = ((60000 / FAT_tracker.tempo) / 4) - TEMPO_TIMER_HARDWARE_VALUE;
        FAT_isCurrentlyPlaying = 1;
        ham_StartIntHandler(INT_TYPE_TIM3, (void*) &FAT_player_timerFunc_playBlocks);

        R_TIM3CNT = 0;
        M_TIM3CNT_IRQ_ENABLE
        M_TIM3CNT_TIMER_START

        ham_StartIntHandler(INT_TYPE_TIM0, (void*) &FAT_player_timerFunc_iCanPressStart);

        R_TIM0CNT = 3;
        M_TIM0CNT_IRQ_ENABLE
        M_TIM0CNT_TIMER_START
    }
}

void FAT_player_startPlayerFromNotes(u8 blockId, u8 startLine, u8 channel) {

    if (iCanPressStart) {
        iCanPressStart = 0;

        memset(actualSequencesForChannel, NULL_VALUE, sizeof (u8)*6);
        memset(actualBlocksForChannel, NULL_VALUE, sizeof (u8)*6);
        memset(actualNotesForChannel, 0, sizeof (u8)*6);
        actualNotesForChannel[channel] = startLine;
        FAT_currentPlayedBlock = blockId;
        FAT_currentPlayedChannel = channel;

        tempoReach = ((60000 / FAT_tracker.tempo) / 4) - TEMPO_TIMER_HARDWARE_VALUE;
        FAT_isCurrentlyPlaying = 1;
        ham_StartIntHandler(INT_TYPE_TIM3, (void*) &FAT_player_timerFunc_playNotes);

        R_TIM3CNT = 0;
        M_TIM3CNT_IRQ_ENABLE
        M_TIM3CNT_TIMER_START

        ham_StartIntHandler(INT_TYPE_TIM0, (void*) &FAT_player_timerFunc_iCanPressStart);

        R_TIM0CNT = 3;
        M_TIM0CNT_IRQ_ENABLE
        M_TIM0CNT_TIMER_START
    }
}

void FAT_player_timerFunc_playSequences() {
    tempoReach--;
    if (tempoReach <= 0) {

        for (u8 i = 0; i < 6; i++) {
            FAT_currentPlayedSequence = FAT_tracker.channels[i].sequences[actualSequencesForChannel[i]];
            if (FAT_currentPlayedSequence != NULL_VALUE) {
                // lire la séquence actuelle
                sequence* seq = &FAT_tracker.allSequences[FAT_currentPlayedSequence];

                FAT_currentPlayedBlock = seq->blocks[actualBlocksForChannel[i]];
                if (FAT_currentPlayedBlock != NULL_VALUE) {
                    block* block = &FAT_tracker.allBlocks[FAT_currentPlayedBlock];

                    // Déplacement des curseurs de lecture
                    FAT_player_moveOrHideCursor(i);

                    // TODO BUFFERISER
                    FAT_player_playNote(&(block->notes[actualNotesForChannel[i]]), i);

                    actualNotesForChannel[i]++;
                    if (actualNotesForChannel[i] >= NB_NOTES_IN_ONE_BLOCK) {
                        actualNotesForChannel[i] = 0;
                        actualBlocksForChannel[i]++;
                        if (actualBlocksForChannel[i] >= NB_BLOCKS_IN_SEQUENCE
                                || seq->blocks[actualBlocksForChannel[i]] == NULL_VALUE) {
                            actualBlocksForChannel[i] = 0;
                            actualSequencesForChannel[i]++;
                            if (actualSequencesForChannel[i] > NB_MAX_SEQUENCES
                                    || FAT_tracker.channels[i].sequences[actualSequencesForChannel[i]] == NULL_VALUE
                                    || FAT_data_isSequenceEmpty(FAT_tracker.channels[i].sequences[actualSequencesForChannel[i]])) {
                                actualSequencesForChannel[i] = 0;
                                // TODO ne pas remonter à zéro -> remonter à la premiere séquence dispo
                                // si pas de séquences dispo -> NULL_VALUE
                            }
                        }
                    }
                } else {
                    actualBlocksForChannel[i] = 0;
                    actualSequencesForChannel[i]++;
                    if (actualSequencesForChannel[i] > NB_MAX_SEQUENCES) {
                        actualSequencesForChannel[i] = 0;
                    }
                }

            } else {
                actualSequencesForChannel[i] = 0;
            }

        }

        tempoReach = ((60000 / FAT_tracker.tempo) / 4) - TEMPO_TIMER_HARDWARE_VALUE;
    }
}

void FAT_player_timerFunc_playBlocks() {

    tempoReach--;
    if (tempoReach <= 0) {
        if (FAT_currentPlayedSequence != NULL_VALUE) {
            // lire la séquence actuelle
            sequence* seq = &FAT_tracker.allSequences[FAT_currentPlayedSequence];

            FAT_currentPlayedBlock = seq->blocks[actualBlocksForChannel[FAT_currentPlayedChannel]];
            if (FAT_currentPlayedBlock != NULL_VALUE) {
                block* block = &FAT_tracker.allBlocks[FAT_currentPlayedBlock];

                // Déplacement des curseurs de lecture
                FAT_player_moveOrHideCursor(FAT_currentPlayedChannel);

                // TODO BUFFERISER
                FAT_player_playNote(&(block->notes[actualNotesForChannel[FAT_currentPlayedChannel]]), FAT_currentPlayedChannel);

                actualNotesForChannel[FAT_currentPlayedChannel]++;
                if (actualNotesForChannel[FAT_currentPlayedChannel] >= NB_NOTES_IN_ONE_BLOCK) {
                    actualNotesForChannel[FAT_currentPlayedChannel] = 0;
                    actualBlocksForChannel[FAT_currentPlayedChannel]++;
                    if (actualBlocksForChannel[FAT_currentPlayedChannel] >= NB_BLOCKS_IN_SEQUENCE
                            || seq->blocks[actualBlocksForChannel[FAT_currentPlayedChannel]] == NULL_VALUE) {
                        actualBlocksForChannel[FAT_currentPlayedChannel] = 0;
                    }
                }
            } else {
                actualBlocksForChannel[FAT_currentPlayedChannel] = 0;
            }

        }

        tempoReach = ((60000 / FAT_tracker.tempo) / 4) - TEMPO_TIMER_HARDWARE_VALUE;
    }


}

void FAT_player_timerFunc_playNotes() {
    tempoReach--;
    if (tempoReach <= 0) {
        if (FAT_currentPlayedBlock != NULL_VALUE) {
            block* block = &FAT_tracker.allBlocks[FAT_currentPlayedBlock];

            // Déplacement des curseurs de lecture
            FAT_player_moveOrHideCursor(FAT_currentPlayedChannel);

            // TODO BUFFERISER
            FAT_player_playNote(&(block->notes[actualNotesForChannel[FAT_currentPlayedChannel]]), FAT_currentPlayedChannel);

            actualNotesForChannel[FAT_currentPlayedChannel]++;
            if (actualNotesForChannel[FAT_currentPlayedChannel] >= NB_NOTES_IN_ONE_BLOCK) {
                actualNotesForChannel[FAT_currentPlayedChannel] = 0;
            }
        }

        tempoReach = ((60000 / FAT_tracker.tempo) / 4) - TEMPO_TIMER_HARDWARE_VALUE;
    }
}

void FAT_player_stopPlayer() {
    if (iCanPressStart) {
        iCanPressStart = 0;

        M_TIM3CNT_TIMER_STOP
        M_TIM3CNT_IRQ_DISABLE

        // stop le son
        snd_stopAllSounds();

        // cache tous les curseurs de lecture
        FAT_player_hideAllCursors();

        // la lecture est terminée.
        FAT_isCurrentlyPlaying = 0;

        ham_StartIntHandler(INT_TYPE_TIM0, (void*) &FAT_player_timerFunc_iCanPressStart);

        R_TIM0CNT = 3;
        M_TIM0CNT_IRQ_ENABLE
        M_TIM0CNT_TIMER_START
    }
}

void FAT_player_hideAllCursors() {
    FAT_player_hideNoteCursor();
    FAT_player_hideBlockCursor();
    FAT_player_hideSequencesCursors();
}

void FAT_player_hideSequencesCursors() {
    ham_SetObjVisible(FAT_cursor_playerSequences_obj[0], 0);
    ham_SetObjVisible(FAT_cursor_playerSequences_obj[1], 0);
    ham_SetObjVisible(FAT_cursor_playerSequences_obj[2], 0);
    ham_SetObjVisible(FAT_cursor_playerSequences_obj[3], 0);
    ham_SetObjVisible(FAT_cursor_playerSequences_obj[4], 0);
    ham_SetObjVisible(FAT_cursor_playerSequences_obj[5], 0);
}

void FAT_player_hideBlockCursor() {
    ham_SetObjVisible(FAT_cursor_player2_obj, 0);
}

void FAT_player_hideNoteCursor() {
    ham_SetObjVisible(FAT_cursor_player3_obj, 0);
}

void FAT_player_moveOrHideCursor(u8 channel) {

    switch (FAT_currentScreen) {
        case SCREEN_SONG_ID: // on est dans l'écran SONG !
            FAT_player_hideBlockCursor();
            FAT_player_hideNoteCursor();
            if (actualSequencesForChannel[channel] != NULL_VALUE) {
                // la lecture a été lancée depuis l'écran SONG
                // on dispose du numéro de ligne actuellement jouée dans actualSequencesForChannel[channel]
                ham_SetObjXY(FAT_cursor_playerSequences_obj[channel],
                        23 + (channel * (8 + 16)), 15 + (actualSequencesForChannel[channel]*8));
                ham_SetObjVisible(FAT_cursor_playerSequences_obj[channel], 1);
            } else {
                FAT_player_hideSequencesCursors();
            }
            break;
        case SCREEN_BLOCKS_ID: // on est dans l'écran BLOCKS
            FAT_player_hideSequencesCursors();
            FAT_player_hideNoteCursor();
            if (actualBlocksForChannel[channel] != NULL_VALUE &&
                    FAT_currentPlayedSequence == FAT_screenBlocks_currentSequenceId) {
                ham_SetObjXY(FAT_cursor_player2_obj, 23, 15 + (actualBlocksForChannel[channel]*8));
                ham_SetObjVisible(FAT_cursor_player2_obj, 1);
            } else {
                FAT_player_hideBlockCursor();
            }

            break;
        case SCREEN_NOTES_ID: // on est dans l'écran NOTES
            FAT_player_hideSequencesCursors();
            FAT_player_hideBlockCursor();
            if (actualNotesForChannel[channel] != NULL_VALUE
                    && FAT_currentPlayedBlock == FAT_screenNotes_currentBlockId) {
                ham_SetObjXY(FAT_cursor_player3_obj, 23, 15 + (actualNotesForChannel[channel]*8));
                ham_SetObjVisible(FAT_cursor_player3_obj, 1);
            } else {
                FAT_player_hideNoteCursor();
            }
            break;
        default: // on cache tous les curseurs de lecture
            FAT_player_hideAllCursors();
            break;
    }

}

#endif	/* PLAYER_H */

