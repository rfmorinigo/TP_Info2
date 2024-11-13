#include "mylib.h"

int main(void) {
    // Declarar e inicializar la estructura del puente
    puente_t *puente;
    estados_t estado_actual = espera;

    // Configuración inicial de los parámetros del puente
    puente->altura_actual = 0.0;
    puente->altura_maxima = 3.0;
    puente->altura_minima= 0.0;

    //estructura de inicialización GPIO para AVR
    GpioInitStructure_AVR salida;
    GpioInitStructure_AVR entrada;

    //configuración de pines de salida para LEDs y motor
    salida.port = avr_GPIO_D;
    salida.modo = avr_GPIO_mode_Output;
    salida.pines = avr_GPIO_PIN_0 |  // LED Rojo
        avr_GPIO_PIN_1 |  // LED Verde
        avr_GPIO_PIN_2 |  // Motor IN1
        avr_GPIO_PIN_3 |  // Motor IN2
        avr_GPIO_PIN_4;   // Motor ENA
    init_gpio(salida);
    
    //condiguracion de pines de entrada para los switch
    entrada.port = avr_GPIO_A;
    entrada.modo = avr_GPIO_mode_Input; 
    entrada.pines = avr_GPIO_PIN_0 | avr_GPIO_PIN_1; //input SUBIR / BAJAR
    init_gpio(entrada);

    //asegurar que el motor esté apagado al inicio
    clear_pin(avr_GPIOD_OUT,avr_GPIO_PIN_4);
    clear_pin(avr_GPIOD_OUT,avr_GPIO_PIN_2);
    clear_pin(avr_GPIOD_OUT,avr_GPIO_PIN_3);
    //switch en alto
    set_pin(avr_GPIOA_IN,avr_GPIO_PIN_0;)
    set_pin(avr_GPIOA_IN,avr_GPIO_PIN_1;)

    while (1) {
        switch (estado_actual) {
            case espera:
                estado_actual = f_espera_puente(puente);
                break;
            case elevando:
                estado_actual = f_elevando_puente(puente);
                break;
            case elevado:
                estado_actual = f_elevado_puente(puente);
                break;
            case bajando:
                estado_actual = f_bajando_puente(puente);
                break;
        }
    }
}
