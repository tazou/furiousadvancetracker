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
 * \file yesno_dialog.h 
 * \brief Ce fichier définit toutes les fonctions utiles pour gérer toutes les boites
 * de dialogue de type "OUI ou NON". (L = NON, R = OUI).
 */

#ifndef _YESNO_DIALOG_H_
#define	_YESNO_DIALOG_H_

#include "data.h"


#define DIALOG_LAYER 0

#define DIALOG_SAVE 0
#define DIALOG_LOAD 1
#define DIALOG_SORRY_SAVE 2
#define DIALOG_KEYBOARD 3
#define DIALOG_NEW 4

// Prototypes
void FAT_yesno_close ();

/** \brief Variable pour stocker un premier paramètre (variable) pour une boite de dialogue. */
u8 param1;

/**
 * \brief Fonction principale de la boite de dialog SAVE
 */
void FAT_yesno_dialogSave_mainFunc() {
    if (mutex) {
        ham_CopyObjToOAM();
        if (iCanPressAKey) {
            
            if (F_CTRLINPUT_L_PRESSED){
                FAT_yesno_close();
            }
            
            else if (F_CTRLINPUT_R_PRESSED){
                FAT_filesystem_saveRaw(param1);
                FAT_yesno_close();
                ham_DrawText(24, 16, "SAVED ");
            }
        }
    }
}

/**
 * \brief Fonction principale de la boite de dialog LOAD
 */
void FAT_yesno_dialogLoad_mainFunc() {
    if (mutex) {
        ham_CopyObjToOAM();
        if (iCanPressAKey) {
            
            if (F_CTRLINPUT_L_PRESSED){
                FAT_yesno_close();
            }
            
            else if (F_CTRLINPUT_R_PRESSED){
                FAT_filesystem_loadRaw(param1);
                FAT_currentScreen = SCREEN_PROJECT_ID;
                FAT_yesno_close();
                ham_DrawText(24, 16, "LOADED");
            }
        }
    }
}

/**
 * \brief Fonction principale de la boite de dialog KEYBOARD
 */
void FAT_yesno_dialogKeyboard_mainFunc() {
    if (mutex) {
        ham_CopyObjToOAM();
        if (iCanPressAKey) {
            
            if (F_CTRLINPUT_L_PRESSED){
                FAT_yesno_close();
            }
            
            else if (F_CTRLINPUT_R_PRESSED){
                FAT_yesno_close ();
            }
        }
    }
}

/**
 * \brief Fonction principale de la boite de dialog NEW PROJECT.
 */
void FAT_yesno_dialogNewProject_mainFunc() {
    if (mutex) {
        ham_CopyObjToOAM();
        if (iCanPressAKey) {
            
            if (F_CTRLINPUT_L_PRESSED){
                FAT_yesno_close();
            }
            
            else if (F_CTRLINPUT_R_PRESSED){
                FAT_data_initData();
                FAT_yesno_close();
                ham_DrawText(24, 16, "GOGOGO");
            }
        }
    }
}

/**
 * \brief Fonction principale pour une boite ou il suffit de répondre "OUI" 
 */
void FAT_onlyyes_dialog_mainFunc (){
    if (mutex) {
        ham_CopyObjToOAM();
        if (iCanPressAKey) {          
            if (F_CTRLINPUT_R_PRESSED){
                FAT_yesno_close();
            }
        }
    }
}

/**
 * \brief Ferme la boite de dialogue et reinitialise la popup (qui est sur le même layer). 
 */
void FAT_yesno_close (){
    ham_StopIntHandler(INT_TYPE_VBL);
    FAT_popup_init();
    FAT_switchToScreen(FAT_currentScreen);
}

/**
 * \brief Ouvre la boite de dialogue (modale) permettant la sauvegarde du projet.
 */
void FAT_yesno_dialogSave (){
 
    ham_bg[DIALOG_LAYER].ti = ham_InitTileSet((void*) screen_dialog_save_Tiles, SIZEOF_16BIT(screen_dialog_save_Tiles), 1, 1);
    ham_bg[DIALOG_LAYER].mi = ham_InitMapSet((void *) screen_dialog_save_Map, 1024, 0, 0);
    ham_InitBg(DIALOG_LAYER, 1, 0, 0);
    
    ham_StopIntHandler(INT_TYPE_VBL);
    ham_StartIntHandler(INT_TYPE_VBL, (void*) &FAT_yesno_dialogSave_mainFunc);
    
}

/**
 * \brief Ouvre la boite de dialogue (modale) pour le chargement du projet.
 */
void FAT_yesno_dialogLoad (){
 
    ham_bg[DIALOG_LAYER].ti = ham_InitTileSet((void*) screen_dialog_load_Tiles, SIZEOF_16BIT(screen_dialog_load_Tiles), 1, 1);
    ham_bg[DIALOG_LAYER].mi = ham_InitMapSet((void *) screen_dialog_load_Map, 1024, 0, 0);
    ham_InitBg(DIALOG_LAYER, 1, 0, 0);
    
    ham_StopIntHandler(INT_TYPE_VBL);
    ham_StartIntHandler(INT_TYPE_VBL, (void*) &FAT_yesno_dialogLoad_mainFunc);
    
}

/**
 * \brief Boite de dialogue indiquant que seule 3 chansons sont dispos pour la version actuelle. 
 * 
 * \todo Supprimer cette boite de dialogue dès que FAT est capable de gérer la compression.
 */
void FAT_yes_dialogSorrySave (){
    ham_bg[DIALOG_LAYER].ti = ham_InitTileSet((void*) screen_dialog_sorrysave_Tiles, SIZEOF_16BIT(screen_dialog_sorrysave_Tiles), 1, 1);
    ham_bg[DIALOG_LAYER].mi = ham_InitMapSet((void *) screen_dialog_sorrysave_Map, 1024, 0, 0);
    ham_InitBg(DIALOG_LAYER, 1, 0, 0);
    
    ham_StopIntHandler(INT_TYPE_VBL);
    ham_StartIntHandler(INT_TYPE_VBL, (void*) &FAT_onlyyes_dialog_mainFunc);
}

/**
 * \brief Boite de dialogue indiquant pour la confirmation d'un nouveau projet. 
 * 
 */
void FAT_yesno_dialogNewProject (){
    ham_bg[DIALOG_LAYER].ti = ham_InitTileSet((void*) screen_dialog_new_Tiles, SIZEOF_16BIT(screen_dialog_new_Tiles), 1, 1);
    ham_bg[DIALOG_LAYER].mi = ham_InitMapSet((void *) screen_dialog_new_Map, 1024, 0, 0);
    ham_InitBg(DIALOG_LAYER, 1, 0, 0);
    
    ham_StopIntHandler(INT_TYPE_VBL);
    ham_StartIntHandler(INT_TYPE_VBL, (void*) &FAT_yesno_dialogNewProject_mainFunc);
}

/**
 * \brief Boite de dialogue pour le clavier. 
 * 
 */
void FAT_yesno_dialogKeyboard (){
    ham_bg[DIALOG_LAYER].ti = ham_InitTileSet((void*) screen_dialog_keyboard_Tiles, SIZEOF_16BIT(screen_dialog_keyboard_Tiles), 1, 1);
    ham_bg[DIALOG_LAYER].mi = ham_InitMapSet((void *) screen_dialog_keyboard_Map, 1024, 0, 0);
    ham_InitBg(DIALOG_LAYER, 1, 0, 0);
    
    ham_DrawText (6, 5, "A");    
    
    ham_StopIntHandler(INT_TYPE_VBL);
    ham_StartIntHandler(INT_TYPE_VBL, (void*) &FAT_yesno_dialogKeyboard_mainFunc);
    
    
}

/**
 * \brief Fonction wrapper permettant l'ouverture de toutes les boites de dialogues définies.
 *
 * @param idDialog l'id de la boite de dialogue
 */
void FAT_yesno_show (u8 idDialog, ... ){
    
    va_list params;
    va_start (params, idDialog);
    
    if (ham_bg[POPUP_LAYER].ti) {
        ham_DeInitTileSet(ham_bg[POPUP_LAYER].ti);
        ham_DeInitMapSet(ham_bg[POPUP_LAYER].mi);
    }
    
    switch (idDialog){
        case DIALOG_SAVE:
            param1 = va_arg(params, int);
            FAT_yesno_dialogSave();
            break;
        case DIALOG_LOAD:
            param1 = va_arg(params, int);
            FAT_yesno_dialogLoad();
            break;
        case DIALOG_SORRY_SAVE:
            FAT_yes_dialogSorrySave ();
            break;
        case DIALOG_KEYBOARD:
            FAT_yesno_dialogKeyboard();
            break;
        case DIALOG_NEW:
            FAT_yesno_dialogNewProject();
            break;
    }
    
    va_end(params);
}

#endif	/* YESNO_DIALOG_H */

