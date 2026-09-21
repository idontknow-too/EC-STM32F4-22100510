/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Lee Seonyu
Created          : 09-17-2026
Modified         : 09-17-2026
Language/ver     : C++ in VS Code

Description      : GPIO Library. Distributed to Students for LAB_7-segment
/----------------------------------------------------------------*/

// FND for JKIT board 
extern int numberFND[12][8];
void FND_init(PinName_t *pinFND);
void FND_select_init(PinName_t *selectFND);
void FND_display(uint8_t num);
void FND_select(uint8_t digit);

