/******************************************************************************
 *
 * Copyright (c) 1994-2002 PalmSource, Inc. All rights reserved.
 *
 * File: KeyMgr.h
 *
 * Release: Palm OS 5 SDK (68K) R2.
 *
 * Description:
 *		Include file for Key manager
 *
 *****************************************************************************/

#ifndef __KEYMGR_H__
#define __KEYMGR_H__

// Pilot common definitions
#include <PalmTypes.h>
#include <CoreTraps.h>


/********************************************************************
 * Definition of bit field returned from KeyCurrentState
 ********************************************************************/
#define	keyBitPower			0x0001		// Power key
#define	keyBitPageUp		0x0002		// Page-up
#define	keyBitPageDown		0x0004		// Page-down
#define	keyBitHard1			0x0008		// App #1
#define	keyBitHard2			0x0010		// App #2
#define	keyBitHard3			0x0020		// App #3
#define	keyBitHard4			0x0040		// App #4
#define	keyBitCradle		0x0080		// Button on cradle
#define	keyBitAntenna		0x0100		// Antenna "key" <chg 3-31-98 RM>
#define	keyBitContrast		0x0200		// Contrast key

#define	keyBitJogUp			0x00001000	// jog wheel up
#define	keyBitJogDown		0x00002000	// jog wheel down
#define	keyBitJogPress		0x00004000	// press/center on jog wheel
#define	keyBitJogBack		0x00008000	// jog wheel back button

#define keyBitRockerUp		0x00010000	// 5-way rocker up
#define keyBitRockerDown	0x00020000	// 5-way rocker down
#define keyBitRockerLeft	0x00040000	// 5-way rocker left
#define keyBitRockerRight	0x00080000	// 5-way rocker right
#define keyBitRockerCenter	0x00100000	// 5-way rocker center/press

#define	keyBitsAll			0xFFFFFFFF	// all keys


#define slowestKeyDelayRate	0xff
#define slowestKeyPeriodRate	0xff


/********************************************************************
 * Key manager Routines
 ********************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// Set/Get the auto-key repeat rate
Err 		KeyRates(Boolean set, UInt16 *initDelayP, UInt16 *periodP, 
						UInt16 *doubleTapDelayP, Boolean *queueAheadP)
							SYS_TRAP(sysTrapKeyRates);
							
// Get the current state of the hardware keys
// This is now updated every tick, even when more than 1 key is held down.
UInt32	KeyCurrentState(void)
							SYS_TRAP(sysTrapKeyCurrentState);
							
// Set the state of the hardware key mask which controls if the key
// generates a keyDownEvent
UInt32	KeySetMask(UInt32 keyMask)
							SYS_TRAP(sysTrapKeySetMask);
							
#ifdef __cplusplus
}
#endif

	
#endif	//__KEYMGR_H__