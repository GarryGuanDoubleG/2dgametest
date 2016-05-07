#include "sara.h"
#include "json_parse.h"
#include "simple_logger.h"
#include <stdio.h>

#define MGLSAVEMAJOR 1
#define MGLSAVEMINOR 0
#define MGLSAVEKEY   4564564487
#define MGLSAVEOFFSET 100

typedef struct
{
    Uint32 key;    /**<unique key meant to verify file type*/
    Uint32 major;  /**<major version of the file type*/
    Uint32 minor;  /**<minor version of the file type*/
    size_t  size;   /**<size of the header*/
}SaveHeader;

void save_dict_as_json(Dict *dict, char *filepath)
{
	char *json;
	FILE *file;

	if(!dict)
	{
		slog("Could not save. No dict provided");
	}
	file = fopen(filepath, "w");

	if(!file)
	{
		slog("Failed to open file %s for writing", filepath);
	}
	json = json_convert_dict_to_string(dict);

	if(!json)
	{
		slog("Failed to convert dict to json");
		fclose(file);
		return;
	}
	fprintf(file, "%s", json);
	fclose(file);
	free(json);
}

SaveHeader *save_new_header()
{
    SaveHeader *header;
    header = (SaveHeader *)malloc(sizeof(SaveHeader));
    if (!header)return NULL;
    memset(header,0,sizeof(SaveHeader));
    header->major = MGLSAVEMAJOR;
    header->minor = MGLSAVEMINOR;
    header->size = sizeof(SaveHeader);
    header->key = MGLSAVEKEY;
    return header;
}