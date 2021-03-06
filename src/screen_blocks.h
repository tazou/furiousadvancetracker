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
 * \file screen_blocks.h
 * \brief Fichier réservé pour la gestion de l'écran BLOCKS.
 */

#ifndef _SCREEN_BLOCKS_H_
#define _SCREEN_BLOCKS_H_

/** \brief Numéro du tiles ou l'affichage démarre (en case tile). */
#define SCREENBLOCKS_LINE_START_Y 2
/** \brief Début de l'affichage des blocks. */
#define SCREENBLOCKS_BLOCK_LINE_X 3
/** \brief Début de l'affichage du paramètre transpose. */
#define SCREENBLOCKS_TRANSPOSE_LINE_X 6
/** \brief Début de l'affichage de la commande assignée au block. */
#define SCREENBLOCKS_EFFECT_LINE_X 8
/** \brief Début de l'affichage des numéros de lignes. */
#define SCREENBLOCKS_LINE_X 0
/** \brief Nombre de blocks visibles à l'écran. */
#define SCREENBLOCKS_NB_LINES_ON_SCREEN 16
/**  \brief Taille en tiles d'un block (Y). */
#define SCREENBLOCKS_LINE_SIZE_Y 1

// ID des colonnes réprésentées par l'interface (ne compte pas la colonne du numéro de ligne)
/** \brief Id de la colonne des BLOCKS. Utile seulement pour la gestion du curseur. */
#define SCREENBLOCKS_COLUMN_ID_BLK 0
/** \brief Id de la colonne du transpose. Utile seulement pour la gestion du curseur. */
#define SCREENBLOCKS_COLUMN_ID_TSP 1
/** \brief Id de la colonne du nom de la commande. Utile seulement pour la gestion du curseur. */
#define SCREENBLOCKS_COLUMN_ID_CMD_NAME 2
/** \brief Id de la colonne de la valeur de la commande. Utile seulement pour la gestion du curseur. */
#define SCREENBLOCKS_COLUMN_ID_CMD_PARAM 3

/**
 * \brief Permet de savoir si l'écran BLOCKS est actuellement "popupé".
 */
bool FAT_screenBlocks_isPopuped = 0;
/**
 * \brief Les blocks sont toujours inclus dans une séquence: cette variable sert
 * à stocker son id.
 */
u8 FAT_screenBlocks_currentSequenceId;

// prototypes
void FAT_screenBlocks_init();
void FAT_screenBlocks_checkButtons();
void FAT_screenBlocks_printInfos();
void FAT_screenBlocks_pressOrHeldA();
void FAT_screenBlocks_pressB();

#include "screen_blocks_cursor.h"

/**
 * \brief Fonction de routine qui affiche les numéros de ligne. 
 */
void FAT_screenBlocks_printLineColumns() {
    u8 y = SCREENBLOCKS_LINE_START_Y;
    int c;
    for (c = 0; c < (SCREENBLOCKS_NB_LINES_ON_SCREEN); c++) {
        hel_BgTextPrintF(TEXT_LAYER, SCREENBLOCKS_LINE_X, y, 0, FAT_FORMAT_LINE, c);
        y += SCREENBLOCKS_LINE_SIZE_Y;
    }
}

/**
 * \brief Affiche les infos dynamiques sur le coin haut droit de l'écran.
 * 
 * Pour le moment cette fonction se contente d'afficher le numéro de ligne 
 * en cours d'édition. 
 */
void FAT_screenBlocks_printInfos() {
    hel_BgTextPrintF(TEXT_LAYER, 18, 4, 0, "Line     %.2x", FAT_screenBlocks_currentSelectedLine);
}

/**
 * \brief Affiche le numéro de séquence dans le coin en haut à droite de l'écran.
 * 
 * Affiche seulement le numéro de séquence: cette fonction est sensé être appelée
 * peu de fois.  
 */
void FAT_screenBlocks_printSequenceNumber() {
    hel_BgTextPrintF(TEXT_LAYER, 18, 3, 0, "Sequence %.2x", FAT_screenBlocks_currentSequenceId);
}

/**
 * \brief Ecrit 1 block à l'écran à la bonne ligne.
 * 
 * @param line le numéro de la ligne du block à écrire
 */
void FAT_screenBlocks_printBlock(u8 line) {
    if (FAT_data_getBlock(FAT_screenBlocks_currentSequenceId, line) != NULL_VALUE) {
        hel_BgTextPrintF(TEXT_LAYER, SCREENBLOCKS_BLOCK_LINE_X,
                line + SCREENBLOCKS_LINE_START_Y, 0,
                "%.2x\0", FAT_data_getBlock(FAT_screenBlocks_currentSequenceId, line));
    } else {
        hel_BgTextPrint(TEXT_LAYER, SCREENBLOCKS_BLOCK_LINE_X,
                line + SCREENBLOCKS_LINE_START_Y, 0, "  ");
    }
}

/**
 * \brief Affiche les informations de transpose pour le block situé à la ligne
 * 
 * @param line le numéro de ligne du block
 */
void FAT_screenBlocks_printTranspose(u8 line) {
    if (!FAT_data_block_isTransposeEmpty(FAT_screenBlocks_currentSequenceId, line)) {
        hel_BgTextPrintF(TEXT_LAYER, SCREENBLOCKS_TRANSPOSE_LINE_X, line + SCREENBLOCKS_LINE_START_Y, 0,
                "%.2x\0", FAT_data_block_getTranspose(FAT_screenBlocks_currentSequenceId, line));
    } else {
        hel_BgTextPrint(TEXT_LAYER, SCREENBLOCKS_TRANSPOSE_LINE_X, line + SCREENBLOCKS_LINE_START_Y, 0,
                "00");
    }
}

/**
 * \brief Affiche les informations d'effets pour le block situé à une ligne donnée.
 *  
 * @param line le numéro de ligne du block
 */
void FAT_screenBlocks_printEffect(u8 line) {
    if (!FAT_data_block_isEffectEmpty(FAT_screenBlocks_currentSequenceId, line)) {

        effect* effect = FAT_data_block_getEffect(FAT_screenBlocks_currentSequenceId, line);

        hel_BgTextPrintF(TEXT_LAYER, SCREENBLOCKS_EFFECT_LINE_X, line + SCREENBLOCKS_LINE_START_Y, 0,
                "%s%.2x\0", blockEffectName[(effect->name & 0xfe) >> 1], effect->value);
    } else {
        hel_BgTextPrint(TEXT_LAYER, SCREENBLOCKS_EFFECT_LINE_X, line + SCREENBLOCKS_LINE_START_Y, 0,
                "    ");
    }
}

/**
 * \brief Affiche tous les blocks pour la séquence courante.
 */
void FAT_screenBlocks_printAllBlocks() {
    u8 b;
    for (b = 0; b < SCREENBLOCKS_NB_LINES_ON_SCREEN; b++) {
        FAT_screenBlocks_printBlock(b);
        FAT_screenBlocks_printTranspose(b);
        FAT_screenBlocks_printEffect(b);
    }
}

/**
 * \brief Fonction wrapping qui affiche toutes les informations intéressantes à l'écran.
 */
void FAT_screenBlocks_printAllScreenText() {
    FAT_screenBlocks_printLineColumns();
    FAT_screenBlocks_printAllBlocks();
    FAT_screenBlocks_printInfos();
}

/**
 * \brief Initialise l'écran BLOCKS.
 * 
 * Affiche du texte, déplace le curseur à la dernière position connue, change 
 * l'interface... 
 */
void FAT_screenBlocks_init() {
    FAT_reinitScreen();

    // initialisation du fond (interface)
    ham_bg[SCREEN_LAYER].ti = ham_InitTileSet((void*)ResData(RES_SCREEN_BLOCKS_RAW), RES_SCREEN_BLOCKS_RAW_SIZE16, 1, 1);
    ham_bg[SCREEN_LAYER].mi = ham_InitMapSet((void*)ResData(RES_SCREEN_BLOCKS_MAP), 1024, 0, 0);
    ham_InitBg(SCREEN_LAYER, 1, 3, 0);

    // affichage d'un peu de texte
    // numéro de la séquence en cours d'édition, tout est dans SCREEN_SONG :)
    FAT_screenBlocks_currentSequenceId = FAT_data_getSequence(FAT_screenSong_currentSelectedColumn, FAT_screenSong_currentSelectedLine);
    if (FAT_screenBlocks_currentSequenceId == NULL_VALUE) {
        FAT_screenBlocks_currentSequenceId = 0;
    }
    FAT_screenBlocks_printSequenceNumber();
    FAT_screenBlocks_printAllScreenText();

    // démarrage du cycle pour l'écran
    //hel_IntrUpdateHandler(INT_TYPE_VBL, (void*) &FAT_screenBlocks_mainFunc);

    // affichage du curseur
    FAT_player_hideAllCursors ();
    FAT_cursors_hideCursor2();
    FAT_screenBlocks_commitCursorMove();
    FAT_cursors_showCursor2();
    FAT_cursors_moveCursorChange(INPUT_R_CURSOR_CHANGE_X, INPUT_R_CURSOR_CHANGE_Y);
}

/**
 * \brief Fonction qui teste les actions utilisateur. 
 * 
 * La gestion des touches A et B est déportée dans des fonctions externes. 
 */
void FAT_screenBlocks_checkButtons() {
    hel_PadCapture();

    if (hel_PadQuery()->Held.Select) {
        if (!FAT_screenBlocks_isPopuped) {
            FAT_cursors_hideCursor2();
            FAT_cursors_hideCursorChange();
            FAT_popup_show();
            FAT_screenBlocks_isPopuped = 1;
        }

        FAT_popup_checkButtons();

    } else {
        if (FAT_screenBlocks_isPopuped) {
            FAT_popup_hide();
            FAT_cursors_showCursor2();
            FAT_screenBlocks_isPopuped = 0;

            if (FAT_popup_getSelectedIcon() != SCREEN_BLOCKS_ID) {
                FAT_cursors_hideCursor2();
                FAT_switchToScreen(FAT_popup_getSelectedIcon());
            }
        }

        if (hel_PadQuery()->Held.A || hel_PadQuery()->Pressed.A) {
            // on agit selon la colonne actuellement séléctionné
            FAT_screenBlocks_pressOrHeldA();

        } else {

            if (hel_PadQuery()->Held.R) {
                FAT_cursors_showCursorChange();

                if (hel_PadQuery()->Pressed.Right) {
                    if (FAT_screenBlocks_currentSequenceId < NB_MAX_SEQUENCES - 1) {
                        FAT_screenBlocks_currentSequenceId++;
                        FAT_screenBlocks_printSequenceNumber();
                        FAT_screenBlocks_printAllScreenText();
                    }
                }

                if (hel_PadQuery()->Pressed.Left) {
                    if (FAT_screenBlocks_currentSequenceId > 0) {
                        FAT_screenBlocks_currentSequenceId--;
                        FAT_screenBlocks_printSequenceNumber();
                        FAT_screenBlocks_printAllScreenText();
                    }
                }

                if (hel_PadQuery()->Pressed.Down) {
                    // TODO passer à la séquence qui suit si elle existe
                }

                if (hel_PadQuery()->Pressed.Up) {
                    // TODO passer à la séquence précédente si elle existe
                }

            } else {
                FAT_cursors_hideCursorChange();

                if (hel_PadQuery()->Pressed.Start) {
                    if (!FAT_isCurrentlyPlaying) {
                        FAT_player_startPlayerFromBlocks(FAT_screenBlocks_currentSequenceId,
                                FAT_screenBlocks_currentSelectedLine, FAT_screenSong_currentSelectedColumn);
                    } else {
                        FAT_player_stopPlayer();
                    }
                }

                if (hel_PadQuery()->Pressed.B) {
                    FAT_screenBlocks_pressB();
                }

                if (hel_PadQuery()->Pressed.Right) {
                    FAT_screenBlocks_moveCursorRight();
                }

                if (hel_PadQuery()->Pressed.Left) {
                    FAT_screenBlocks_moveCursorLeft();
                }

                if (hel_PadQuery()->Pressed.Down) {
                    if (hel_PadQuery()->Held.L) {
                        FAT_screenBlocks_moveCursorAllDown();
                    } else {
                        FAT_screenBlocks_moveCursorDown();
                    }

                }

                if (hel_PadQuery()->Pressed.Up) {
                    if (hel_PadQuery()->Held.L) {
                        FAT_screenBlocks_moveCursorAllUp();
                    } else {
                        FAT_screenBlocks_moveCursorUp();
                    }
                }

                FAT_screenBlocks_commitCursorMove();
            }

            if (hel_PadQuery()->Pressed.R && hel_PadQuery()->Pressed.L) {
                FAT_showHelp(SCREEN_BLOCKS_ID);
            }

        }
    }

}

/**
 * \brief Fonction pour gérer la touche B sur l'écran BLOCKS.
 */
void FAT_screenBlocks_pressB() {
    switch (FAT_screenBlocks_currentSelectedColumn) {
        case SCREENBLOCKS_COLUMN_ID_BLK:
            if (hel_PadQuery()->Held.L) {
                if (FAT_data_isBlockAllocatable(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine)) {
                    // espace libre
                    FAT_data_pasteBlockWithNewNumber(FAT_screenBlocks_currentSequenceId,
                            FAT_screenBlocks_currentSelectedLine);
                } else {
                    FAT_data_cloneBlock(FAT_screenBlocks_currentSequenceId,
                            FAT_screenBlocks_currentSelectedLine);
                }

            } else {
                if (FAT_data_isBlockAllocatable(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine)) {
                    // espace libre
                    FAT_data_pasteBlock(FAT_screenBlocks_currentSequenceId,
                            FAT_screenBlocks_currentSelectedLine);
                } else {
                    FAT_data_cutBlock(FAT_screenBlocks_currentSequenceId,
                            FAT_screenBlocks_currentSelectedLine);
                }

            }

            FAT_screenBlocks_printBlock(FAT_screenBlocks_currentSelectedLine);
            break;
        case SCREENBLOCKS_COLUMN_ID_TSP:
            FAT_data_removeBlockTranspose(FAT_screenBlocks_currentSequenceId,
                    FAT_screenBlocks_currentSelectedLine);

            FAT_screenBlocks_printTranspose(FAT_screenBlocks_currentSelectedLine);
            break;
    }
}

/**
 * \brief Fonction pour gérer la touche A sur l'écran BLOCKS.
 */
void FAT_screenBlocks_pressOrHeldA() {
    switch (FAT_screenBlocks_currentSelectedColumn) {
        case SCREENBLOCKS_COLUMN_ID_BLK:

            if (hel_PadQuery()->Held.L) {
                FAT_data_smartAllocateBlock(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine);
            } else {
                FAT_data_allocateBlock(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine);
            }

            if (hel_PadQuery()->Pressed.Right) {
                FAT_data_block_changeValue(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine, 1); // ajout de 1
            }

            if (hel_PadQuery()->Pressed.Left) {
                FAT_data_block_changeValue(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine, -1); // retrait de 1
            }

            if (hel_PadQuery()->Pressed.Up) {
                FAT_data_block_changeValue(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine, 16);
            }

            if (hel_PadQuery()->Pressed.Down) {
                FAT_data_block_changeValue(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine, -16);
            }

            FAT_screenBlocks_printBlock(FAT_screenBlocks_currentSelectedLine);
            break;

        case SCREENBLOCKS_COLUMN_ID_TSP:
            FAT_data_block_allocateTranspose(FAT_screenBlocks_currentSequenceId,
                    FAT_screenBlocks_currentSelectedLine);

            if (hel_PadQuery()->Pressed.Left) {
                FAT_data_block_changeTransposeValue(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine, -1);
            }

            if (hel_PadQuery()->Pressed.Right) {
                FAT_data_block_changeTransposeValue(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine, 1);
            }

            if (hel_PadQuery()->Pressed.Down) {
                FAT_data_block_changeTransposeValue(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine, -16);
            }

            if (hel_PadQuery()->Pressed.Up) {
                FAT_data_block_changeTransposeValue(FAT_screenBlocks_currentSequenceId,
                        FAT_screenBlocks_currentSelectedLine, 16);
            }

            FAT_screenBlocks_printTranspose(FAT_screenBlocks_currentSelectedLine);
            break;

    }
}

#endif
