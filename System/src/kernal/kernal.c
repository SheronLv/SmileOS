#include "const/const.h"
#include "structure/structure.h"
#include "structure/algorithm.c"
#include "macro/macro.h"
#include "inOutput/inOutput.h"
#include "memory/memory.c"
#include "sheet/sheet.c"
#include "include/globalData.c"
#include "include/timing.c"
#include "include/mouse.c"
#include "include/harddisk.c"
#include "include/keyboard.c"
#include "include/descriptor.c"
#include "include/interrupt.c"
#include "include/peripheral.c"
#include "charset/charset.c"
#include "gui/image.c"
#include "graphics/graphics.c"
#include "window/window.c"
#include "desktop/desktop.c"
#include "background/background.c"

int mx, my;

MouseData mouseData;

KeyData keyData;

Sheet *window;

Sheet *mouse;

void initSystem(void)
{	
    mouseData.phase = 0;

	initInterruptHandler();	
	initQueueBufferData();
	initTimerManagement();
	initPeripheralStatus();

    initMemoryManagement();
    initSheetManagement();

    Sheet *background = prepareSheet();
    prepareBackgroundSheet(background);
    loadSheet(background, 0);

    window = prepareSheet();
    prepareWindowSheet(window);
    loadSheet(window, 1);

    mouse = prepareSheet();
    prepareMouseSheet(mouse);
    loadSheet(mouse, 2);

	Timer* time1 = requestTimer(1000, 1, &systemBuffer);
	Timer* time2 = requestTimer(1500, 2, &systemBuffer);
	Timer* time3 = requestTimer(5000, 3, &systemBuffer);
	Timer* time4 = requestTimer(8000, 4, &systemBuffer);
	

    mx = 640;
    my = 512;

	//showBufferInfo(window, (u8 *)0xe0000000);

    while(TRUE) {
        clearInterrupt();
		if (queueBufferStatus(&systemBuffer) == 0) {
			setupInterrupt();
		} else {
			u32 data = getQueueBuffer(&systemBuffer);
			if (data<1024) {
				if (data == 1) {
					showInfo(window, 100, 100, 11);			
				} else if (data == 2) {
					showInfo(window, 100, 160, 22);			
				} else if (data == 3) {
					showInfo(window, 100, 220, 33);			
				} else if (data == 4) {
					showInfo(window, 100, 280, 55);			
				}		
				setupInterrupt();
			} else if (data>=1024 && data<2048) {
				processKeyData(&keyData, data, window);			
			} else if (data>=2048) {
				processMouseData(&mouseData, data, mouse, &mx, &my);
			}
		}
    }
}

