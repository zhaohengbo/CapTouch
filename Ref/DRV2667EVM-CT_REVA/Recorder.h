/*
 * Recorder.h
 *
 *  Created on: Jul 25, 2012
 *      Author: Brian
 */

#ifndef RECORDER_H_
#define RECORDER_H_

#include "CapTouchBoard.h"
#include <stdint.h>

void Recorder_Init();

uint8_t Recorder_IsRecording();

void Recorder_RecordToggle();

void Recorder_Record();

void Recorder_Playback();

void Recorder_Reset();

void Recorder_PlayToggle();

#endif /* RECORDER_H_ */
