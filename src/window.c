#include "window.h"
#include "simple_logger.h"
#include "sprite.h"
#include "load.h"

Bool window_load_data_from_def(char * filename, void *data);

void window_init()
{
	int i;
	slog("Window init");
	


}

Bool window_load_data_from_def(char *filename, void *data)
{
	if(!filename || !data)
	{
		slog("No filename or data pointer passed");
		return Bool_False;
	}


}