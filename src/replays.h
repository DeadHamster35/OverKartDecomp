#ifndef STAFF_GHOSTS_H
#define STAFF_GHOSTS_H

#include <PR/os.h>
#include <common_structs.h>

void norm_data_control(void);
void DMAROMGhost(void);
void replay_ghost(void);
void read_ghost(void);
void check_romghost(void);
s32 make_press_ghost(void);
void make_melt_ghost(void);
void swap_ghostbuffer(void);
void initial_ghost(void);
void replayread_key_data(void);
void romread_key_data(void);
void read_key_data(void);
void func_8000599C(void);
void kill_ghost(Player*);
void replay_data_control(void);
void key_data_control(void);

// mi0decode

extern s32 encpass2(s32 input, s32, s32);
extern s32 encpass1(void*, s32, s32);

extern s32 read_pointer;
extern s32 write_pointer;
extern u16 ghost_status;
extern u16 romghost_status;
extern u16 replayghost_status;
extern s32 D_80162E00;
extern s32 ghost_kart;
extern s32 romghost_kart;
extern s32 replayghost_kart;
extern s32 ghost_write;
extern s32 lost_ghost;

#endif /* STAFF_GHOSTS_H */
