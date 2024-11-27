#ifndef MYLIB_H
#define MYLIB_H
#include <avr/io.h>
#include "avr_api/avr_api.h"

#define ALTURA_MAX 127
#define ALTURA_MIN 0

//definiciones para los pulsadores de subir y bajar
#define SWITCH_PORT avr_GPIO_A
#define SWITCH_DOWN_PIN avr_GPIO_PIN_1
#define SWITCH_UP_PIN avr_GPIO_PIN_0
#define SWITCH_UP avr_GPIOA_IN_0
#define SWITCH_DOWN avr_GPIOA_IN_1

// Definiciones de puertos de salida
#define OUT_PORT avr_GPIO_D
#define LED_ROJO_PIN avr_GPIO_PIN_0
#define LED_VERDE_PIN avr_GPIO_PIN_1
#define ENA_PIN avr_GPIO_PIN_4
#define IN1_PIN avr_GPIO_PIN_3
#define IN2_PIN avr_GPIO_PIN_4
#define LED_ROJO avr_GPIOD_OUT_0
#define LED_VERDE avr_GPIOD_OUT_1
#define IN1 avr_GPIOD_OUT_2
#define IN2 avr_GPIOD_OUT_3
#define ENA avr_GPIOD_OUT_4

typedef enum {
    espera = 0,
    elevando = 1,
    elevado = 2,
    bajando = 3
} estados_t;

//prototipos estados
estados_t *(f_espera_puente)(void);
estados_t *(f_elevando_puente)(void);
estados_t *(f_elevado_puente)(void);
estados_t *(f_bajando_puente)(void);

//prototipos funciones
void init_puente(void);
void subir_barrera(void);
void bajar_barrera(void);
void activar_motor_subir(void);
unsigned int leer_sensor(void);
void activar_motor_bajar(void);
void apagar_motor(void);
void systick_led(void);

#endif
