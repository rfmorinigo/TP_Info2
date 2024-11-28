#include "mylib.h"

estados_t *(f_espera_puente)(void) {
    if (SWITCH_UP==0) {
        return elevando;
    } else {
        return espera;
    }
}

estados_t *(f_elevando_puente)(void) {
       bajar_barrera();
       activar_motor_subir();
    return elevado;
}

estados_t *(f_elevado_puente)(void) {

    if (SWITCH_DOWN==0) {
        return bajando;
    } else {
        return elevado;
    }
}

estados_t *(f_bajando_puente)(void) {
    activar_motor_bajar();
    subir_barrera();
    
    return espera;
}