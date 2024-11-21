#include "mylib.h"

estados_t *(f_espera_puente)(void) {
    if (SWITCH_UP==0) { //revisar si el switch de "subir" está activado
        return elevando;
    } else {
        return espera;
    }
}

estados_t *(f_elevando_puente)(void) {
    int i;
    bajar_barrera();
    activar_motor_subir();
    /*while (leer_sensor()!=ALTURA_MAX) {
        leer_sensor();
    }*/
    apagar_motor();
    return elevado;
}

estados_t *(f_elevado_puente)(void) {

    //revisar si el switch de "bajar" está activado
    if (SWITCH_DOWN==0) {
        return bajando;
    } else {
        return elevado;
    }
}

estados_t *(f_bajando_puente)(void) {
    int i;
    activar_motor_bajar();
    /*while (leer_sensor()!=ALTURA_MIN) {
        leer_sensor();
    }*/
    subir_barrera();
    apagar_motor();
    
    return espera;
}
