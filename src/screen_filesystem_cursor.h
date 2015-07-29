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
 * \file screen_filesystem_cursor.h
 * \brief Fichier dédié à la gestion du curseur dans l'écran filesystem.
 */


#ifndef _SCREEN_FILESYSTEM_CURSOR_H_
#define	_SCREEN_FILESYSTEM_CURSOR_H_

/** \brief Position en pixel du premier bloc. */
#define SCREENFILESYSTEM_FIRST_BLOCK_X 24
/** \brief Position en pixel du premier bloc d'action. */
#define SCREENFILESYSTEM_FIRST_ACTION_X 95
/** \brief Position en pixel du premier bloc. */
#define SCREENFILESYSTEM_FIRST_BLOCK_Y 16

/** \brief Position en pixel du dernier bloc. */
#define SCREENFILESYSTEM_LAST_BLOCK_Y 135
/** \brief Hauteur d'un block en pixels. */
#define SCREENFILESYSTEM_BLOCK_SIZE_Y 8

/** \brief Position actuelle du curseur d'action. */
u8 FAT_screenFilesystem_action_cursorX;
/** \brief Position actuelle du curseur. */
u8 FAT_screenFilesystem_cursorY;
/** \brief Numéro de ligne actuellement sélectionnée. */
u8 FAT_screenFilesystem_currentSelectedLine;


void FAT_screenFilesystem_initCursor();
void FAT_screenFilesystem_moveCursorDown();
void FAT_screenFilesystem_moveCursorUp();
void FAT_screenFilesystem_commitCursorMove();

/**
 * \brief Initialisation du curseur (position uniquement), remise à zéro de la ligne
 * sélectionnée. 
 */
void FAT_screenFilesystem_initCursor() {
    FAT_screenFilesystem_cursorY = SCREENFILESYSTEM_FIRST_BLOCK_Y - 1;
    FAT_screenFilesystem_currentSelectedLine = 0;
}

/**
 * \brief Cette fonction permet de valider le déplacement du curseur sur l'écran. 
 */
void FAT_screenFilesystem_commitCursorMove() {
    hel_ObjSetXY(FAT_cursor8_obj, SCREENFILESYSTEM_FIRST_BLOCK_X, FAT_screenFilesystem_cursorY+1);
    hel_ObjSetXY(FAT_cursor2_obj, SCREENFILESYSTEM_FIRST_ACTION_X, FAT_screenFilesystem_cursorY);
}

/**
 * \brief Déplace le curseur vers le bas. 
 */
void FAT_screenFilesystem_moveCursorDown() {
    if (FAT_screenFilesystem_currentSelectedLine < SCREENFILESYSTEM_NB_LINES_ON_SCREEN) {
        if (!(FAT_screenFilesystem_cursorY >= SCREENFILESYSTEM_LAST_BLOCK_Y - 1)) {
            FAT_screenFilesystem_cursorY += SCREENFILESYSTEM_BLOCK_SIZE_Y;
            FAT_screenFilesystem_currentSelectedLine++;
            FAT_screenFilesystem_printInfos();
        }
    }
}

/**
 * \brief Déplace le curseur vers le haut. 
 */
void FAT_screenFilesystem_moveCursorUp() {

    if (FAT_screenFilesystem_currentSelectedLine > 0) {
        if (!(FAT_screenFilesystem_cursorY <= SCREENFILESYSTEM_FIRST_BLOCK_Y - 1)) {
            FAT_screenFilesystem_cursorY -= SCREENFILESYSTEM_BLOCK_SIZE_Y;
            FAT_screenFilesystem_currentSelectedLine--;
            FAT_screenFilesystem_printInfos();
        }
    }

}


#endif	/* SCREEN_FILESYSTEM_CURSOR_H */

