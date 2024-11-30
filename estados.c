#include "mylib.h"

estados_t f_espera_puente(void) {
    if (SWITCH_UP==0) {
	 bajar_barrera();
	 activar_motor_subir();
        return elevando;
    } else {
        return espera;
    }
}

estados_t f_elevando_puente(void) {
       if (leer_sensor() >= ALTURA_MAX) {
	 stop();
	 return elevado;
       } else {
	 return elevando;
       }
}

estados_t f_elevado_puente(void) {

    if (SWITCH_DOWN==0) {
        activar_motor_bajar();
        return bajando;
    } else {
        return elevado;
    }
}

estados_t f_bajando_puente(void) {

    if (leer_sensor() <= ALTURA_MIN) {
      stop();
      subir_barrera();
      return espera;
    } else {
      return bajando;
    }
}
