/******************************************************************************
 * LibraryModes.h
 *
 * Created on: February 17, 2013
 * Board: DRV2667EVM-CT
 * Author: a0866685
 *
 * Desc: This file contains the button functions to access the DRV2667 RAM
 * 		waveforms.  Include this file to compile with the DRV2667 RAM library
 * 		modes.
 *
 * 		Mode 0 - 31 are the library effects.
 *
 ******************************************************************************/

#ifndef LIBRARYMODES_H_
#define LIBRARYMODES_H_

#define LIBRARYMODESLEVEL		1 	// Sets the depth of the library modes

/**
 * LibraryModes - function containing the entire DRV2605 mode button functions and effects
 */
void LibraryModes(void);

#endif /* LIBRARYMODES_H_ */
