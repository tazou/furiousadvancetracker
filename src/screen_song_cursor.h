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
 * \file screen_song_cursor.h
 * \brief Ce fichier contient toutes les définitions de fonctions utiles pour la gestion
 * du curseur sur l'écran SONG.
 */

#ifndef _CURSOR_H_
#define _CURSOR_H_

/** \brief Taille d'un bloc d'affichage séquence 16*8 en pixels. */
#define SCREENSONG_BLOCK_SIZE_X 16
/** \brief Taille d'un bloc d'affichage séquence 16*8 en pixels. */
#define SCREENSONG_BLOCK_SIZE_Y 8

// paramètre de l'interface
/** \brief Espace définit entre chaque colonne (px). */
#define SCREENSONG_WHITE_SPACE_X 8
/** \brief Coordonnée du dernier bloc en bas à droite de l'écran (px). */
#define SCREENSONG_LAST_BLOCK_X 143
/** \brief Absisse du dernier bloc en bas à droite de l'écran (px). */
#define SCREENSONG_LAST_BLOCK_Y 135
/** \brief Coordonnée du premier bloc en haut à gauche (px). */
#define SCREENSONG_FIRST_BLOCK_X 24
/** \brief Abscisse du premier bloc en haut à gauche (px). */
#define SCREENSONG_FIRST_BLOCK_Y 16

/** \brief Position actuelle du curseur de sélection sur l'écran. */
u8 FAT_screenSong_cursorX; 
/** \brief Position actuelle du curseur de sélection sur l'écran. */
u8 FAT_screenSong_cursorY;
/** \brief Numéro de ligne actuellement sélectionnée. */
u8 FAT_screenSong_currentSelectedLine; 
/** \brief Numéro de colonne actuellement sélectionnée. */
u8 FAT_screenSong_currentSelectedColumn;

// prototypes
void FAT_screenSong_initCursor();
void FAT_screenSong_moveCursorRight();
void FAT_screenSong_moveCursorLeft();
void FAT_screenSong_moveCursorDown();
void FAT_screenSong_moveCursorUp();
void FAT_screenSong_commitCursorMove();

/**
 * \brief Initialisation du curseur (seulement la position) et remise à zéro
 * des valeurs de ligne et colonne sélectionnées. 
 */
void FAT_screenSong_initCursor() {
    FAT_screenSong_cursorX = SCREENSONG_FIRST_BLOCK_X - 1;
    FAT_screenSong_cursorY = SCREENSONG_FIRST_BLOCK_Y - 1;
    FAT_screenSong_currentSelectedLine = 0;
    FAT_screenSong_currentStartLine = 0;
    FAT_screenSong_currentSelectedColumn = 0;
    
};

/**
 * \brief Déplace le curseur vers la droite. 
 */
void FAT_screenSong_moveCursorRight() {
    if (!(FAT_screenSong_cursorX >= SCREENSONG_LAST_BLOCK_X - 1)) {
        FAT_screenSong_cursorX += SCREENSONG_BLOCK_SIZE_X + SCREENSONG_WHITE_SPACE_X;
        FAT_screenSong_currentSelectedColumn++;
        FAT_screenSong_printInfos();
    }
}

/**
 * \brief Déplace le curseur vers la gauche. 
 */
void FAT_screenSong_moveCursorLeft() {
    if (!(FAT_screenSong_cursorX <= SCREENSONG_FIRST_BLOCK_X)) {
        FAT_screenSong_cursorX -= SCREENSONG_BLOCK_SIZE_X + SCREENSONG_WHITE_SPACE_X;
        FAT_screenSong_currentSelectedColumn--;
        FAT_screenSong_printInfos();
    }
}

/**
 * \brief Déplace le curseur d'une page vers le bas. 
 */
void FAT_screenSong_movePageDown() {
    if (FAT_screenSong_currentSelectedLine < NB_SEQUENCES_IN_ONE_CHANNEL - SCREENSONG_NB_LINES_ON_SCREEN) {
        FAT_screenSong_currentStartLine += SCREENSONG_NB_LINES_ON_SCREEN;
        FAT_screenSong_currentSelectedLine += SCREENSONG_NB_LINES_ON_SCREEN;
        FAT_screenSong_printAllScreenText();
    }
}

/**
 * \brief Déplace le curseur tout en bas du tableau. 
 */
void FAT_screenSong_moveCursorAllDown() {
    FAT_screenSong_cursorY = SCREENSONG_LAST_BLOCK_Y;
    FAT_screenSong_currentStartLine = NB_SEQUENCES_IN_ONE_CHANNEL - SCREENSONG_NB_LINES_ON_SCREEN;
    FAT_screenSong_currentSelectedLine = NB_SEQUENCES_IN_ONE_CHANNEL - 1;
    FAT_screenSong_printAllScreenText();
}

/**
 * \brief Déplace le curseur vers le bas. 
 */
void FAT_screenSong_moveCursorDown() {
    if (FAT_screenSong_currentSelectedLine < NB_SEQUENCES_IN_ONE_CHANNEL) {
        if (FAT_screenSong_cursorY >= SCREENSONG_LAST_BLOCK_Y - 1) {
            if (FAT_screenSong_currentStartLine < NB_SEQUENCES_IN_ONE_CHANNEL - SCREENSONG_NB_LINES_ON_SCREEN) {
                // on n'avance pas mais on change de ligne
                FAT_screenSong_currentStartLine++;
                FAT_screenSong_currentSelectedLine++;
                FAT_screenSong_printAllScreenText();
            }
        } else {
            FAT_screenSong_cursorY += SCREENSONG_BLOCK_SIZE_Y;
            FAT_screenSong_currentSelectedLine++;
            FAT_screenSong_printInfos();
        }
    }
}

/**
 * \brief Déplace le curseur d'une page vers le haut. 
 */
void FAT_screenSong_movePageUp() {
    if (FAT_screenSong_currentSelectedLine >= SCREENSONG_NB_LINES_ON_SCREEN
            && FAT_screenSong_currentStartLine >= SCREENSONG_NB_LINES_ON_SCREEN) {
        FAT_screenSong_currentStartLine -= SCREENSONG_NB_LINES_ON_SCREEN;
        FAT_screenSong_currentSelectedLine -= SCREENSONG_NB_LINES_ON_SCREEN;
        FAT_screenSong_printAllScreenText();
    }
}

/**
 * \brief Déplace le curseur tout en haut du tableau. 
 */
void FAT_screenSong_moveCursorAllUp() {
    FAT_screenSong_cursorY = SCREENSONG_FIRST_BLOCK_Y-1;
    FAT_screenSong_currentStartLine = 0;
    FAT_screenSong_currentSelectedLine = 0;
    FAT_screenSong_printAllScreenText();
}

/**
 * \brief Déplace le curseur vers le haut. 
 */
void FAT_screenSong_moveCursorUp() {

    if (FAT_screenSong_currentSelectedLine > 0) {
        if (FAT_screenSong_cursorY <= SCREENSONG_FIRST_BLOCK_Y - 1) {
            if (FAT_screenSong_currentStartLine > 0) {
                // on ne recule pas mais on change de ligne
                FAT_screenSong_currentStartLine--;
                FAT_screenSong_currentSelectedLine--;
                FAT_screenSong_printAllScreenText();
            }
        } else {
            FAT_screenSong_cursorY -= SCREENSONG_BLOCK_SIZE_Y;
            FAT_screenSong_currentSelectedLine--;
            FAT_screenSong_printInfos();
        }
    }

}

/**
 * \brief Valide le déplacement du curseur sur l'écran. 
 */
void FAT_screenSong_commitCursorMove() {
    hel_ObjSetXY(FAT_cursor2_obj, FAT_screenSong_cursorX, FAT_screenSong_cursorY);
}

#endif
