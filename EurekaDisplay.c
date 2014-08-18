#include <stdlib.h>

#define sizearray(a)  (sizeof(a) / sizeof((a)[0]))

typedef struct {
	int a;
	int b;
} MV_PE_Handle;

int MV_OSAL_Init (void);
int MV_OSAL_Exit (void);
int MV_OSAL_Task_Sleep (int delay);

int MV_PE_Init (MV_PE_Handle **h);
int MV_PE_Remove (MV_PE_Handle *h);
int MV_PE_VOutSetEnable (MV_PE_Handle *h, int cpcb_id, int enabled);
int MV_PE_VOutSetCPCBResolutionBDEx (MV_PE_Handle *h, int cpcb_id, unsigned char res_id, unsigned char bit_depth, int a);
int MV_PE_VOutHDMISetVideoFormat (MV_PE_Handle *h, int a, int b, int c, int d);
int MV_PE_VOutHDMISetAudioFormat (MV_PE_Handle *h, int a, int b, int c, int d);
int MV_PE_VOutSetVisible (MV_PE_Handle *h, int a, int b);
int MV_PE_VOutHDMIGetSinkCaps (MV_PE_Handle *h, int a, void *caps);

//Function to compare 2 character arrays for string comparison
int compStr (const char *s1, char *s2, size_t sz) {
    while (sz != 0) {
        // At end of both strings, equal.
        if ((*s1 == '\0') && (*s2 == '\0')) break;

        // Treat spaces at end and end-string the same.
        if ((*s1 == '\0') && (*s2 == ' ')) { s2++; sz--; continue; }
        if ((*s1 == ' ') && (*s2 == '\0')) { s1++; sz--; continue; }

        // Detect difference otherwise.
        if (*s1 != *s2) return 0;
        s1++; s2++; sz--;
    }
    return 1;
}

int main (int argc, char *argv[] ) {

	if ( argc < 3 ) /* no additional arguments, present app information */
    	{
		/* Print application details */
		printf( "\n###########################################################\n" );
		printf( "# EurekaDisplay v1.0                                      #\n" );
		printf( "# Team Eureka [www.Team-Eureka.com]                       #\n" );
		printf( "###########################################################\n" );
		printf( "# Licensed under GPLv3                                    #\n" );
		printf( "# Uses Marvell libraries                                  #\n" );
		printf( "# 					                   #\n" );
		printf( "#                                                         #\n" );
		printf( "###########################################################\n" );
		printf( "Usage: %s resolution [720p|1080p] \n", argv[0] );
		printf( "       %s turn [on|off] \n\n", argv[0] );
		return 0;
    	}
	

	MV_PE_Handle *h;

	MV_OSAL_Init();
	MV_PE_Init(&h);

	void *caps = calloc(1, 1508);
	MV_PE_VOutHDMIGetSinkCaps(h, 1, caps);
	free(caps);

	MV_PE_VOutSetEnable(h, 0, 0);
	MV_OSAL_Task_Sleep(100);

	if(compStr("resolution", argv[1], sizearray(argv[1]))){
		if(compStr("720p", argv[2], sizearray(argv[2]))){
			printf( "\n\nChanging Resolution to 720p\n\n" );
			MV_PE_VOutSetCPCBResolutionBDEx(h, 0, 15, 2, 0);
			MV_PE_VOutHDMISetVideoFormat(h, 1, 1, 2, 1);
		}
		if(compStr("1080p", argv[2], sizearray(argv[2]))){
			printf( "\n\nChanging Resolution to 1080p\n\n" );
			MV_PE_VOutSetCPCBResolutionBDEx(h, 0, 25, 2, 0);
			MV_PE_VOutHDMISetVideoFormat(h, 1, 1, 2, 1);
		}
	}

	if(compStr("turn", argv[1], sizearray(argv[1]))){
		if(compStr("on", argv[2], sizearray(argv[2]))){
			printf( "\n\nTurning on display\n\n" );
			MV_PE_VOutHDMISetVideoFormat(h, 1, 1, 2, 1);
		}
		if(compStr("off", argv[2], sizearray(argv[2]))){
			printf( "\n\nTurning off display\n\n" );
			MV_PE_VOutHDMISetVideoFormat(h, 1, 1, 3, 1);

		}
	}

	MV_PE_VOutHDMISetAudioFormat(h, 1, 15, 1, 0);

	MV_PE_VOutSetEnable(h, 0, 1);
	MV_OSAL_Task_Sleep(100);

	MV_PE_VideoSetVisible(h, 3, 1);

	MV_PE_Remove(h);
	MV_OSAL_Exit();

	return EXIT_SUCCESS;
}
