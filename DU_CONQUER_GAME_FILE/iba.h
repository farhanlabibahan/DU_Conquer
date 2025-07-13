#ifndef IBA_H
#define IBA_H

typedef enum {
    Dept_iba,
    Game_iba
} dept_state_iba;

void init_iba();
void logic_draw_iba();
void unload_iba();

extern dept_state_iba dept_status_iba;
#endif