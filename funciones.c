#include "mylib.h"

estados_t f_espera_puente(puente_t *puente) {
    set_pin(avr_GPIOD_OUT, avr_GPIO_PIN_1); //encender led verde
    if ((avr_GPIOA_IN & (1 << 0))==0) { //revisar si el switch de "subir" está activado
        return elevando;
    } else {
        return espera;
    }
}

estados_t f_elevando_puente(puente_t *puente) {
    int i;
    clear_pin(avr_GPIOD_OUT, avr_GPIO_PIN_1); //apagar LED verde
    //subir barrera
    for (i = 0; i < 2; i++) {
        clear_pin(avr_GPIOD_OUT, avr_GPIO_PIN_0);
        _delay_ms(1000); 
        set_pin(avr_GPIOD_OUT, avr_GPIO_PIN_0);
    }
    set_pin(avr_GPIOD_OUT, avr_GPIO_PIN_4);   //activar el motor ENA=1
    // Rotación en dirección de elevación
    set_pin(avr_GPIOD_OUT, avr_GPIO_PIN_2);   // IN1=1
    clear_pin(avr_GPIOD_OUT, avr_GPIO_PIN_3); //IN2=0

    //simulación de elevación
    while (puente->altura_actual < puente->altura_maxima) {
        puente->altura_actual += 1;
        _delay_ms(1000);
    }

    clear_pin(avr_GPIOD_OUT, avr_GPIO_PIN_4); //apagar el motor
    return elevado;
}

estados_t f_elevado_puente(puente_t *puente) {

    //revisar si el switch de "bajar" está activado
    if ((avr_GPIOA_IN & (1 << 1))==0) {
        return bajando;
    } else {
        return elevado;
    }
}

estados_t f_bajando_puente(puente_t *puente) {
    int i;
    set_pin(avr_GPIOD_OUT, avr_GPIO_PIN_4);    // Activar el motor
    clear_pin(avr_GPIOD_OUT, avr_GPIO_PIN_2);  // Configuración para rotación en dirección de descenso
    set_pin(avr_GPIOD_OUT, avr_GPIO_PIN_3);

    //simulación de descenso
    while (puente->altura_actual > puente->altura_minima) {
        puente->altura_actual -= 1;
        _delay_ms(1000);
    }
    //bajar barrera
    for (i = 0; i < 3; i++) {
        clear_pin(avr_GPIOD_OUT, avr_GPIO_PIN_0);
        _delay_ms(1000); 
        set_pin(avr_GPIOD_OUT, avr_GPIO_PIN_0);
    }
    clear_pin(avr_GPIOD_OUT, avr_GPIO_PIN_4);      //apagar el motor
    clear_pin(avr_GPIOD_OUT, avr_GPIO_PIN_0); //apagar LED rojo
    
    return espera;
}
