#include "mylib.h"

estados_t *(f_espera_puente)(void) {
    if (leer_switch_subir()) { //revisar si el switch de "subir" está activado
        return (estados_t *)elevando;
    } else {
        _delay_ms(3000);
        return (estados_t *)espera;
    }
}

estados_t *(f_elevando_puente)(void) {
    int i;
    bajar_barrera();
    activar_motor_subir();
    while (leer_sensor()!=ALTURA_MAX) {
        leer_sensor();
    }
    return (estados_t *)elevado;
}

estados_t *(f_elevado_puente)(void) {

    //revisar si el switch de "bajar" está activado
    if ((leer_switch_subir())==1) {
        return (estados_t *)bajando;
    } else {
        return (estados_t *)elevado;
    }
}

estados_t *(f_bajando_puente)(void) {
    int i;
    activar_motor_bajar();
    while (leer_sensor()!=ALTURA_MIN) {
        leer_sensor();
    }
    bajar_barrera();
    apagar_motor();
    
    return (estados_t *)espera;
}