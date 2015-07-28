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

/**
 * \file screen_instrument.h
 * \brief Ce fichier contient toutes les fonctions relatives à la gestion des instruments.
 */

#ifndef _SCREEN_INSTRUMENT_H_
#define	_SCREEN_INSTRUMENT_H_

// Variables permettant de savoir l'état de l'écran (popup affichée, onglet affiché)
/** \brief Permet de savoir si la popup de déplacement est affichée au dessus de l'écran. */
bool FAT_screenInstrument_isPopuped = 0;
/** \brief Permet de savoir si l'utilisateur est en train de changer d'onglet (type d'instrument). */
u8 FAT_screenInstrument_isTabulating = 0;

// quel instrument est en cours d'édition ?
/** \brief L'id de l'instrument en cours d'édition. */
u8 FAT_screenInstrument_currentInstrumentId;

// sprites utiles à l'affichage de certaines données
/** \brief Sprite pour la direction de l'enveloppe: valeur 0. */
u8 FAT_instrument_envdir0_obj;
/** \brief Sprite pour la direction de l'enveloppe: valeur 1. */
u8 FAT_instrument_envdir1_obj;
/** \brief Sprite pour le paramètre waveduty: valeur 0. */
u8 FAT_instrument_waveduty0_obj;
/** \brief Sprite pour le paramètre waveduty: valeur 1. */
u8 FAT_instrument_waveduty1_obj;
/** \brief Sprite pour le paramètre waveduty: valeur 2. */
u8 FAT_instrument_waveduty2_obj;
/** \brief Sprite pour le paramètre waveduty: valeur 3. */
u8 FAT_instrument_waveduty3_obj;

/**
 * \brief Tableau constant de chaines utiles à l'affichage du paramètre output d'un instrument.
 */
const char* outputText [4] = {"  ", "L ", " R", "LR"};

#include "screen_instrument_cursor.h"
#include "data.h"

// prototypes
void FAT_screenInstrument_init();
void FAT_screenInstrument_checkButtons();
void FAT_screenInstrument_switchScreen(u8 type);
void FAT_screenInstrument_pressOrHeldA();
void FAT_screenInstrument_showOutput(u8 x, u8 y, u8 output);
void FAT_screenInstrument_showWaveduty(u8 wavedutyValue, u8 spriteX, u8 spriteY);
void FAT_screenInstrument_showEnvdir(u8 envdirValue, u8 spriteX, u8 spriteY);
void FAT_screenInstrument_hideAllEnvdirSprites();
void FAT_screenInstrument_hideAllWavedutySprite();

/**
 * \brief Affiche le numéro de l'écran en cours d'édition dans le cadre en haut à droite. 
 */
void FAT_screenInstrument_printInstrumentNumber() {
    hel_BgTextPrintF(TEXT_LAYER, 16, 3, 0, "Instrument %.2x", FAT_screenInstrument_currentInstrumentId);
}

/**
 * \brief Affiche toutes les valeurs des paramètres de l'instrument selon son type.
 * 
 * @param type le type de l'instrument
 */
void FAT_screenInstrument_printAllText(u8 type) {
    switch (type) {
        case INSTRUMENT_TYPE_PULSE:
        case INSTRUMENT_TYPE_PULSE2:
            hel_BgTextPrintF(TEXT_LAYER, 1, 4, 0, "Volume    %.1x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].envelope & 0x0f);
            hel_BgTextPrint(TEXT_LAYER, 1, 5, 0, "Direction");
            FAT_screenInstrument_showEnvdir((FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].envelope & 0x10) >> 4,
                    88, 40);
            hel_BgTextPrintF(TEXT_LAYER, 1, 6, 0, "Steptime  %.1x", (FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].envelope & 0xe0) >> 5);
            hel_BgTextPrint(TEXT_LAYER, 1, 7, 0, "Wave");
            FAT_screenInstrument_showWaveduty(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].wavedutyOrPolynomialStep,
                    88, 56);
            hel_BgTextPrintF(TEXT_LAYER, 1, 10, 0, "Timed     %.1x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].loopmode);
            if (FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].loopmode) {
                hel_BgTextPrintF(TEXT_LAYER, 1, 11, 0, "Length    %.2x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].soundlength);
            } else {
                hel_BgTextPrint(TEXT_LAYER, 1, 11, 0, "Length    NA");
            }
            FAT_screenInstrument_showOutput(1, 12, FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].output);
            hel_BgTextPrintF(TEXT_LAYER, 1, 13, 0, "Sweep     %.2x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].sweep);
            hel_BgTextPrintF(TEXT_LAYER, 1, 16, 0, "Test it!  %s%1x\0",
                    noteName[(FAT_data_simulator.note & 0xf0) >> 4], FAT_data_simulator.note & 0x0f);
            break;
        case INSTRUMENT_TYPE_WAVE:
            hel_BgTextPrintF(TEXT_LAYER, 1, 4, 0, "Volume    %.1x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].volumeRatio & 0x0f);
            hel_BgTextPrintF(TEXT_LAYER, 1, 7, 0, "Timed     %.1x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].loopmode);
            if (FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].loopmode) {
                hel_BgTextPrintF(TEXT_LAYER, 1, 8, 0, "Length    %.2x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].soundlength);
            } else {
                hel_BgTextPrint(TEXT_LAYER, 1, 8, 0, "Length    NA");
            }

            hel_BgTextPrintF(TEXT_LAYER, 1, 9, 0, "Voice     %.2x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].voiceAndBank & 0x1f);
            hel_BgTextPrintF(TEXT_LAYER, 1, 10, 0, "Bank      %.1x", (FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].voiceAndBank & 0x20) >> 5);

            if ((FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].voiceAndBank & 0x40) >> 6 == 0) {
                hel_BgTextPrint(TEXT_LAYER, 1, 11, 0, "Bankmode  SIN");
            } else {
                hel_BgTextPrintF(TEXT_LAYER, 1, 11, 0, "Bankmode  DUA");
            }
            FAT_screenInstrument_showOutput(1, 12, FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].output);
            hel_BgTextPrintF(TEXT_LAYER, 1, 15, 0, "Test it!  %s%1x\0",
                    noteName[(FAT_data_simulator.note & 0xf0) >> 4], FAT_data_simulator.note & 0x0f);
            break;
        case INSTRUMENT_TYPE_NOISE:
            hel_BgTextPrintF(TEXT_LAYER, 1, 4, 0, "Volume    %.1x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].envelope & 0x0f);
            hel_BgTextPrint(TEXT_LAYER, 1, 5, 0, "Direction");
            FAT_screenInstrument_showEnvdir((FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].envelope & 0x10) >> 4,
                    88, 40);
            hel_BgTextPrintF(TEXT_LAYER, 1, 6, 0, "Steptime  %.1x", (FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].envelope & 0xe0) >> 5);
            hel_BgTextPrintF(TEXT_LAYER, 1, 7, 0, "Polystep  %.1x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].wavedutyOrPolynomialStep);

            hel_BgTextPrintF(TEXT_LAYER, 1, 10, 0, "Timed     %.1x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].loopmode);
            if (FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].loopmode) {
                hel_BgTextPrintF(TEXT_LAYER, 1, 11, 0, "Length    %.2x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].soundlength);
            } else {
                hel_BgTextPrint(TEXT_LAYER, 1, 11, 0, "Length    NA");
            }
            FAT_screenInstrument_showOutput(1, 12, FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].output);
            hel_BgTextPrintF(TEXT_LAYER, 1, 16, 0, "Test it!  %s%1x\0",
                    noteName[(FAT_data_simulator.note & 0xf0) >> 4], FAT_data_simulator.note & 0x0f);
            break;
        case INSTRUMENT_TYPE_SAMPLEA:
        case INSTRUMENT_TYPE_SAMPLEB:
            hel_BgTextPrintF(TEXT_LAYER, 16, 4, 0, "Nb samples %.2x", snd_countSamplesInKitById(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].kitNumber));
            //ham_DrawText(16, 5, "NB KITS    %.2x", snd_countAvailableKits ());

            hel_BgTextPrintF(TEXT_LAYER, 1, 4, 0, "Name %s", snd_getKitNameById(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].kitNumber));
            hel_BgTextPrintF(TEXT_LAYER, 1, 7, 0, "Volume    %.1x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].volumeRatio >> 4);
            hel_BgTextPrintF(TEXT_LAYER, 1, 10, 0, "Speed     %.1xx", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].speedOrLooping & 0x0f);
            if (FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].speedOrLooping >> 4) {
                hel_BgTextPrintF(TEXT_LAYER, 1, 11, 0, "Loop      %s", "YES");
            } else {
                hel_BgTextPrintF(TEXT_LAYER, 1, 11, 0, "Loop      %s", "NOP");
            }
            hel_BgTextPrintF(TEXT_LAYER, 1, 12, 0, "Timed     %.1x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].loopmode);
            if (FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].loopmode) {
                hel_BgTextPrintF(TEXT_LAYER, 1, 13, 0, "Length    %.2x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].soundlength);
            } else {
                hel_BgTextPrint(TEXT_LAYER, 1, 13, 0, "Length    NA");
            }
            hel_BgTextPrintF(TEXT_LAYER, 1, 14, 0, "Offset    %.2x", FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].offset);
            FAT_screenInstrument_showOutput(1, 15, FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].output);

            hel_BgTextPrintF(TEXT_LAYER, 16, 12, 0, "Test it!  %s%1x\0", "SM", 1);

            break;
    }
}

/**
 * \brief Initialisation de l'écran instrument. 
 */
void FAT_screenInstrument_init() {
    // quel instrument est en cours d'édition ?
    note* FAT_screenInstrument_currentNote = FAT_data_getNote(FAT_screenNotes_currentBlockId, FAT_screenNotes_currentSelectedLine);
    FAT_screenInstrument_currentInstrumentId = FAT_screenInstrument_currentNote->instrument;
    if (FAT_screenInstrument_currentInstrumentId == NULL_VALUE) {

        FAT_screenInstrument_currentInstrumentId = 0;
    }
    FAT_data_initInstrumentIfNeeded(FAT_screenInstrument_currentInstrumentId, FAT_screenSong_currentSelectedColumn);

    // initialisation des curseurs
    FAT_screenInstrument_initCursor(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);

    // initialisation du fond (interface)
    FAT_screenInstrument_switchScreen(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);

    // initialisation des autre curseurs
    FAT_cursors_moveCursorChange(INPUT_R_CURSOR_CHANGE_X, INPUT_R_CURSOR_CHANGE_Y);

    // démarrage du cycle pour l'écran
    //hel_IntrUpdateHandler(INT_TYPE_VBL, (void*) &FAT_screenInstrument_mainFunc);
}

/**
 * \brief Utilisez cette méthode pour changer d'écran selon le type d'instrument
 * édité. 
 * 
 * @param type le type d'instru à éditer cf. #define INSTRUMENT_TYPE_*
 */
void FAT_screenInstrument_switchScreen(u8 type) {
    FAT_reinitScreen();

    FAT_cursors_hideAllCursors();
    FAT_screenInstrument_hideAllEnvdirSprites();
    FAT_screenInstrument_hideAllWavedutySprite();

    switch (type) {
        case INSTRUMENT_TYPE_PULSE:
        case INSTRUMENT_TYPE_PULSE2:
            ham_bg[SCREEN_LAYER].ti = ham_InitTileSet((void*) screen_instrument_pulse_Tiles, SIZEOF_16BIT(screen_instrument_pulse_Tiles), 1, 1);
            ham_bg[SCREEN_LAYER].mi = ham_InitMapSet((void *) screen_instrument_pulse_Map, 1024, 0, 0);
            FAT_screenInstrument_printInstrumentNumber();
            FAT_screenInstrument_printAllText(type);
            FAT_screenInstrument_initCursor(type);
            FAT_screenInstrument_displayGoodCursor(type);
            break;
        case INSTRUMENT_TYPE_WAVE:
            ham_bg[SCREEN_LAYER].ti = ham_InitTileSet((void*) screen_instrument_wave_Tiles, SIZEOF_16BIT(screen_instrument_wave_Tiles), 1, 1);
            ham_bg[SCREEN_LAYER].mi = ham_InitMapSet((void *) screen_instrument_wave_Map, 1024, 0, 0);
            FAT_screenInstrument_printInstrumentNumber();
            FAT_screenInstrument_printAllText(type);
            FAT_screenInstrument_initCursor(type);
            FAT_screenInstrument_displayGoodCursor(type);
            break;
        case INSTRUMENT_TYPE_NOISE:
            ham_bg[SCREEN_LAYER].ti = ham_InitTileSet((void*) screen_instrument_noise_Tiles, SIZEOF_16BIT(screen_instrument_noise_Tiles), 1, 1);
            ham_bg[SCREEN_LAYER].mi = ham_InitMapSet((void *) screen_instrument_noise_Map, 1024, 0, 0);
            FAT_screenInstrument_printInstrumentNumber();
            FAT_screenInstrument_printAllText(type);
            FAT_screenInstrument_initCursor(type);
            FAT_screenInstrument_displayGoodCursor(type);
            break;
        case INSTRUMENT_TYPE_SAMPLEA:
        case INSTRUMENT_TYPE_SAMPLEB:
            ham_bg[SCREEN_LAYER].ti = ham_InitTileSet((void*) screen_instrument_sample_Tiles, SIZEOF_16BIT(screen_instrument_sample_Tiles), 1, 1);
            ham_bg[SCREEN_LAYER].mi = ham_InitMapSet((void *) screen_instrument_sample_Map, 1024, 0, 0);

            FAT_screenInstrument_printInstrumentNumber();
            FAT_screenInstrument_printAllText(type);
            FAT_screenInstrument_initCursor(type);
            FAT_screenInstrument_displayGoodCursor(type);

            break;
    }
    ham_InitBg(SCREEN_LAYER, 1, 3, 0);
}

/**
 * \brief Cette fonction permet de changer le type de l'instrument en cours d'édition.
 * 
 * Elle gère le déplacement du sprite onglet et permet le switchscreen.
 * @param move +1 -> type vers la droite, -1 type vers la gauche
 */
void FAT_screenInstrument_changeInstrumentType(s8 move) {
    u8 currentType = FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type;

    switch (currentType) {
        case INSTRUMENT_TYPE_PULSE:
        case INSTRUMENT_TYPE_PULSE2:
            if (move > 0) {
                FAT_data_instrument_changeType(FAT_screenInstrument_currentInstrumentId, INSTRUMENT_TYPE_WAVE);
            }
            break;
        case INSTRUMENT_TYPE_WAVE:
            if (move > 0) {
                FAT_data_instrument_changeType(FAT_screenInstrument_currentInstrumentId, INSTRUMENT_TYPE_NOISE);
            } else {
                FAT_data_instrument_changeType(FAT_screenInstrument_currentInstrumentId, INSTRUMENT_TYPE_PULSE);
            }
            break;
        case INSTRUMENT_TYPE_NOISE:
            if (move > 0) {
                FAT_data_instrument_changeType(FAT_screenInstrument_currentInstrumentId, INSTRUMENT_TYPE_SAMPLEA);
            } else {
                FAT_data_instrument_changeType(FAT_screenInstrument_currentInstrumentId, INSTRUMENT_TYPE_WAVE);
            }
            break;
        case INSTRUMENT_TYPE_SAMPLEA:
        case INSTRUMENT_TYPE_SAMPLEB:
            if (move < 0) {

                FAT_data_instrument_changeType(FAT_screenInstrument_currentInstrumentId, INSTRUMENT_TYPE_NOISE);
            }
            break;
    }

    FAT_screenInstrument_moveTabulationCursor(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
}

/**
 * \brief Tester l'appui sur les boutons.
 * 
 * <b>TODO cette méthode est trop grosse et difficilement maintenable. A refactorer. </b>
 */
void FAT_screenInstrument_checkButtons() {
    hel_PadCapture();
    
    if (hel_PadQuery()->Held.Select) {
        if (!FAT_screenInstrument_isPopuped) {
            FAT_screenInstrument_hideAllWavedutySprite();
            FAT_cursors_hideAllCursors();
            FAT_popup_show();
            FAT_screenInstrument_isPopuped = 1;
        }

        FAT_popup_checkButtons();

    } else {
        if (FAT_screenInstrument_isPopuped) {
            FAT_popup_hide();
            FAT_screenInstrument_displayGoodCursor(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
            FAT_screenInstrument_printAllText(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
            FAT_screenInstrument_isPopuped = 0;

            if (FAT_popup_getSelectedIcon() != SCREEN_INSTRUMENTS_ID) {
                FAT_screenInstrument_hideAllEnvdirSprites();
                FAT_screenInstrument_hideAllWavedutySprite();
                FAT_cursors_hideAllCursors();
                FAT_switchToScreen(FAT_popup_getSelectedIcon());
            }
        }


        if (hel_PadQuery()->Held.L) {
            if (!FAT_screenInstrument_isTabulating) {
                FAT_screenInstrument_isTabulating = 1;
                FAT_screenInstrument_showTabulationCursor();
            }

            if (hel_PadQuery()->Pressed.Left) {
                FAT_screenInstrument_changeInstrumentType(-1);
            }

            if (hel_PadQuery()->Pressed.Right) {
                FAT_screenInstrument_changeInstrumentType(1);
            }

        } else {
            if (FAT_screenInstrument_isTabulating) {
                // relachement du L
                FAT_screenInstrument_isTabulating = 0;
                FAT_screenInstrument_switchScreen(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
                FAT_screenInstrument_hideTabulationCursor();
            }

            if (hel_PadQuery()->Held.R) {
                FAT_cursors_showCursorChange();

                if (hel_PadQuery()->Pressed.Right) {
                    if (FAT_screenInstrument_currentInstrumentId < NB_MAX_INSTRUMENTS - 1) {
                        u8 type = FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type;
                        FAT_screenInstrument_currentInstrumentId++;
                        FAT_data_initInstrumentIfNeeded(FAT_screenInstrument_currentInstrumentId, type);
                        FAT_screenInstrument_switchScreen(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
                        FAT_screenInstrument_printInstrumentNumber();
                        FAT_screenInstrument_printAllText(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
                    }
                }

                if (hel_PadQuery()->Pressed.Left) {
                    if (FAT_screenInstrument_currentInstrumentId > 0) {
                        u8 type = FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type;
                        FAT_screenInstrument_currentInstrumentId--;
                        FAT_data_initInstrumentIfNeeded(FAT_screenInstrument_currentInstrumentId, type);
                        FAT_screenInstrument_switchScreen(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
                        FAT_screenInstrument_printInstrumentNumber();
                        FAT_screenInstrument_printAllText(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
                    }
                }

            } else {

                FAT_cursors_hideCursorChange();

                if (hel_PadQuery()->Pressed.A || hel_PadQuery()->Held.A) {
                    FAT_screenInstrument_pressOrHeldA();

                } else {

                    if (hel_PadQuery()->Pressed.Start) {
                        if (!FAT_isCurrentlyPlaying) {
                            FAT_player_startPlayerFromNotes(FAT_screenNotes_currentBlockId,
                                    0, FAT_screenSong_currentSelectedColumn);
                        } else {
                            FAT_player_stopPlayer();
                        }
                    }

                    if (hel_PadQuery()->Pressed.Right) {
                        FAT_screenInstrument_moveCursorRight(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
                    }

                    if (hel_PadQuery()->Pressed.Left) {
                        FAT_screenInstrument_moveCursorLeft(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
                    }

                    if (hel_PadQuery()->Pressed.Down) {
                        FAT_screenInstrument_moveCursorDown(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
                    }

                    if (hel_PadQuery()->Pressed.Up) {
                        FAT_screenInstrument_moveCursorUp(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
                    }
                }
            }
        }

        if (hel_PadQuery()->Pressed.R && hel_PadQuery()->Pressed.L) {
            FAT_screenInstrument_hideTabulationCursor();
            FAT_showHelp(SCREEN_INSTRUMENTS_ID);
        }

        FAT_screenInstrument_commitCursorMove(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
    }
}

/**
 * \brief Cette méthode charge les sprites utilisés dans l'écran de gestion des instruments.
 * 
 * Attention, ne faire appel à cette méthode qu'une seule fois !
 */
void FAT_screenInstrument_initSpritesForInstrument() {

    FAT_instrument_envdir0_obj = ham_CreateObj((void*) envdir_0_Bitmap, OBJ_SIZE_8X8,
            OBJ_MODE_NORMAL, 1, 0, 0, 0, 0, 0, 0, 0, 0);
    FAT_instrument_envdir1_obj = ham_CreateObj((void*) envdir_1_Bitmap, OBJ_SIZE_8X8,
            OBJ_MODE_NORMAL, 1, 0, 0, 0, 0, 0, 0, 0, 0);

    FAT_instrument_waveduty0_obj = ham_CreateObj((void*) waveduty_0_Bitmap, OBJ_SIZE_16X8,
            OBJ_MODE_NORMAL, 1, 0, 0, 0, 0, 0, 0, 0, 0);
    FAT_instrument_waveduty1_obj = ham_CreateObj((void*) waveduty_1_Bitmap, OBJ_SIZE_16X8,
            OBJ_MODE_NORMAL, 1, 0, 0, 0, 0, 0, 0, 0, 0);
    FAT_instrument_waveduty2_obj = ham_CreateObj((void*) waveduty_2_Bitmap, OBJ_SIZE_16X8,
            OBJ_MODE_NORMAL, 1, 0, 0, 0, 0, 0, 0, 0, 0);
    FAT_instrument_waveduty3_obj = ham_CreateObj((void*) waveduty_3_Bitmap, OBJ_SIZE_16X8,
            OBJ_MODE_NORMAL, 1, 0, 0, 0, 0, 0, 0, 0, 0);

    FAT_screenInstrument_hideAllEnvdirSprites();
    FAT_screenInstrument_hideAllWavedutySprite();
}

/**
 * \brief Cache tous les sprites relatifs à l'affiche de la direction de l'enveloppe. 
 */
void FAT_screenInstrument_hideAllEnvdirSprites() {

    ham_SetObjVisible(FAT_instrument_envdir0_obj, 0);
    ham_SetObjVisible(FAT_instrument_envdir1_obj, 0);
}

/**
 * \brief Cache tous les sprites relatifs à l'affiche du paramètre waveduty. 
 */
void FAT_screenInstrument_hideAllWavedutySprite() {

    ham_SetObjVisible(FAT_instrument_waveduty0_obj, 0);
    ham_SetObjVisible(FAT_instrument_waveduty1_obj, 0);
    ham_SetObjVisible(FAT_instrument_waveduty2_obj, 0);
    ham_SetObjVisible(FAT_instrument_waveduty3_obj, 0);
}

/**
 * \brief Affiche le bon sprite pour la valeur de la direction de l'enveloppe.
 *  
 * @param envdirValue la valeur du paramètre
 * @param spriteX position en X pour l'affichage
 * @param spriteY position en Y pour l'affichage
 */
void FAT_screenInstrument_showEnvdir(u8 envdirValue, u8 spriteX, u8 spriteY) {
    FAT_screenInstrument_hideAllEnvdirSprites();
    if (envdirValue == 1) {
        ham_SetObjXY(FAT_instrument_envdir1_obj, spriteX, spriteY);
        ham_SetObjVisible(FAT_instrument_envdir1_obj, 1);
    } else {

        ham_SetObjXY(FAT_instrument_envdir0_obj, spriteX, spriteY);
        ham_SetObjVisible(FAT_instrument_envdir0_obj, 1);
    }

}

/**
 * \brief Affiche la valeur de l'output.
 *  
 * @param x la position du texte (en tiles)
 * @param y la position du texte (en tiles)
 * @param output la valeur de paramètre (0,1,2 ou 3)
 */
void FAT_screenInstrument_showOutput(u8 x, u8 y, u8 output) {

    hel_BgTextPrintF(TEXT_LAYER, x, y, 0, "Output    %.2s", outputText[output]);
}

/**
 * \brief Affiche le bon sprite pour la valeur du waveduty.
 * 
 * @param wavedutyValue la valeur du paramètre
 * @param spriteX position en X pour l'affichage
 * @param spriteY position en Y pour l'affichage
 */
void FAT_screenInstrument_showWaveduty(u8 wavedutyValue, u8 spriteX, u8 spriteY) {
    FAT_screenInstrument_hideAllWavedutySprite();

    if (wavedutyValue == 0) {
        ham_SetObjXY(FAT_instrument_waveduty0_obj, spriteX, spriteY);
        ham_SetObjVisible(FAT_instrument_waveduty0_obj, 1);
    } else if (wavedutyValue == 1) {
        ham_SetObjXY(FAT_instrument_waveduty1_obj, spriteX, spriteY);
        ham_SetObjVisible(FAT_instrument_waveduty1_obj, 1);
    } else if (wavedutyValue == 2) {
        ham_SetObjXY(FAT_instrument_waveduty2_obj, spriteX, spriteY);
        ham_SetObjVisible(FAT_instrument_waveduty2_obj, 1);
    } else if (wavedutyValue == 3) {

        ham_SetObjXY(FAT_instrument_waveduty3_obj, spriteX, spriteY);
        ham_SetObjVisible(FAT_instrument_waveduty3_obj, 1);
    }
}

/**
 * \brief Fonction outil permettant de demander une valeur en fonction d'une touche appuyée.
 *  
 * @return Droite: 1, Gauche: -1, Haut: 16, Bas: -16
 */
s8 FAT_screenInstrument_giveMeAddedValue() {
    s8 addedValue = 0;
    if (hel_PadQuery()->Pressed.Right) {
        addedValue = 1;
    }

    if (hel_PadQuery()->Pressed.Left) {
        addedValue = -1;
    }

    if (hel_PadQuery()->Pressed.Up) {
        addedValue = 16;
    }

    if (hel_PadQuery()->Pressed.Down) {
        addedValue = -16;
    }

    return addedValue;
}

/**
 * \brief Cette fonction permet de gérer l'appui sur la touche A pour un instrument de type PULSE.
 */
void FAT_screenInstrument_pulse_pressA() {

    s8 addedValue = FAT_screenInstrument_giveMeAddedValue();

    switch (FAT_screenInstruments_currentSelectedLine) {
        case 0: // VOLUME
            FAT_data_instrumentPulse_changeVolume(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 1: // ENVELOPE DIRECTION
            FAT_data_instrumentPulse_changeEnvelopeDirection(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 2: // STEPTIME
            FAT_data_instrumentPulse_changeSteptime(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 3: // WAVEDUTY
            FAT_data_instrumentPulse_changeWaveduty(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 4: // TIMED
            FAT_data_instrumentPulse_changeLoopmode(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 5: // LENGTH
            FAT_data_instrumentPulse_changeSoundLength(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 6: // OUTPUT
            FAT_data_instrumentPulse_changeOutput(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 7: // SWEEP
            FAT_data_instrumentPulse_changeSweep(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 8: // SIMULATOR
            FAT_data_instrument_changeSimulator(FAT_screenInstrument_currentInstrumentId, addedValue);
            FAT_data_instrument_playSimulator(FAT_screenInstrument_currentInstrumentId);

            break;
    }

    FAT_screenInstrument_printAllText(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
}

/**
 * \brief Cette fonction permet de gérer l'appui sur la touche A pour un instrument de type WAVE.
 */
void FAT_screenInstrument_wave_pressA() {
    s8 addedValue = FAT_screenInstrument_giveMeAddedValue();

    switch (FAT_screenInstruments_currentSelectedLine) {
        case 0: // VOLUME RATIO
            FAT_data_instrumentWave_changeVolume(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 1: // TIMED
            FAT_data_instrumentWave_changeLoopmode(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 2: // LENGTH
            FAT_data_instrumentWave_changeSoundLength(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 3: // VOICE
            FAT_data_instrumentWave_changeVoice(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 4: // BANK
            FAT_data_instrumentWave_changeBank(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 5: // BANKMODE
            FAT_data_instrumentWave_changeBankmode(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 6: // OUTPUT
            FAT_data_instrumentWave_changeOutput(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 7: // SIMULATOR
            FAT_data_instrument_changeSimulator(FAT_screenInstrument_currentInstrumentId, addedValue);
            FAT_data_instrument_playSimulator(FAT_screenInstrument_currentInstrumentId);

            break;
    }

    FAT_screenInstrument_printAllText(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
}

/**
 * \brief Cette fonction permet de gérer l'appui sur la touche A pour un instrument de type NOISE.
 */
void FAT_screenInstrument_noise_pressA() {
    s8 addedValue = FAT_screenInstrument_giveMeAddedValue();

    switch (FAT_screenInstruments_currentSelectedLine) {
        case 0: // VOLUME
            FAT_data_instrumentNoise_changeVolume(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 1: // ENVELOPE DIRECTION
            FAT_data_instrumentNoise_changeEnvelopeDirection(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 2: // STEPTIME
            FAT_data_instrumentNoise_changeSteptime(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 3: // WAVEDUTY
            FAT_data_instrumentNoise_changePolystep(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 4: // TIMED
            FAT_data_instrumentNoise_changeLoopmode(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 5: // LENGTH
            FAT_data_instrumentNoise_changeSoundLength(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 6: // OUTPUT
            FAT_data_instrumentNoise_changeOutput(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 7: // SIMULATOR
            FAT_data_instrument_changeSimulator(FAT_screenInstrument_currentInstrumentId, addedValue);
            FAT_data_instrument_playSimulator(FAT_screenInstrument_currentInstrumentId);

            break;
    }

    FAT_screenInstrument_printAllText(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
}

/**
 * \brief Cette fonction permet de gérer l'appui sur la touche A pour un instrument de type SAMPLE.
 */
void FAT_screenInstrument_sample_pressA() {
    s8 addedValue = FAT_screenInstrument_giveMeAddedValue();

    switch (FAT_screenInstruments_currentSelectedLine) {
        case 0: // KIT COLLECTION
            FAT_data_instrumentSample_changeKitNumber(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 1: // VOLUME
            FAT_data_instrumentSample_changeVolume(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 2: // SPEED
            FAT_data_instrumentSample_changeSpeed(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 3: // LOOP
            FAT_data_instrumentSample_changeLooping(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 4: //TIMED
            FAT_data_instrumentSample_changeLoopmode(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 5: // LENGTH
            FAT_data_instrumentSample_changeSoundLength(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 6: // OFFSET
            FAT_data_instrumentSample_changeOffset(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
        case 7: // OUTPUT
            FAT_data_instrumentSample_changeOutput(FAT_screenInstrument_currentInstrumentId, addedValue);
            break;
    }

    FAT_screenInstrument_printAllText(FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type);
}

/**
 * \brief Fonction wrapper pour gérer l'appui sur la touche A.
 */
void FAT_screenInstrument_pressOrHeldA() {
    u8 currentType = FAT_tracker.allInstruments[FAT_screenInstrument_currentInstrumentId].type;

    switch (currentType) {
        case INSTRUMENT_TYPE_PULSE:
        case INSTRUMENT_TYPE_PULSE2:
            FAT_screenInstrument_pulse_pressA();
            break;
        case INSTRUMENT_TYPE_WAVE:
            FAT_screenInstrument_wave_pressA();
            break;
        case INSTRUMENT_TYPE_NOISE:
            FAT_screenInstrument_noise_pressA();
            break;
        case INSTRUMENT_TYPE_SAMPLEA:
        case INSTRUMENT_TYPE_SAMPLEB:
            FAT_screenInstrument_sample_pressA();
            break;
    }

}

#endif	/* SCREEN_INSTRUMENT_H */
