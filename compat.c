#include "includes.h"
RCSID("$Id$");

#include "ssh.h"

int compat13 = 0;

void 
enable_compat13(void)
{
	verbose("Enabling compatibility mode for protocol 1.3");
	compat13 = 1;
}
