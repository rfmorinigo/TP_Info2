#include "mylib.h"

volatile unsigned int led_time=0;

int main() {
    estados_t estado_actual = espera;
    estados_t (*vectorEstados[])(void) = {f_espera_puente, f_elevando_puente, f_elevado_puente, f_bajando_puente};

    init_puente();    

    while (1) {
        estado_actual = vectorEstados[estado_actual]();
    }
 return 0;
}