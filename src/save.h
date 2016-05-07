#ifndef _SAVE_H_
#define _SAVE_H_
#include <SDL.h>
#include "dict.h"

/** 
* Based on code provided by DJ Kehoe
*/

/**
* @brief converts a dictionary into a json readable format and outputs it into a save file. Save file may be overwriten
* Note: overwrites file at filepath
* @param dict the dictionary that is converted to json
* @param filepath path to the file to save to
*/
void save_dict_as_json(Dict *dict, char *filepath);



#endif