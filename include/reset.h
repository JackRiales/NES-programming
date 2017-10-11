#ifndef _RESET_H_
#define _RESET_H_

extern u8 FrameCount;
#pragma zpsym("FrameCount");

extern u8 InputPort1;
#pragma zpsym("InputPort1");

extern u8 InputPort1Prev;
#pragma zpsym("InputPort1Prev");

extern u8 InputPort2;
#pragma zpsym("InputPort2");

extern u8 InputPort2Prev;
#pragma zpsym("InputPort2Prev");

void WaitFrame(void);

#endif