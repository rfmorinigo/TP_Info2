#ifndef MYLIB_H
#define MYLIB_H
#include <avr/io.h>
#include <avr_api/avr_api.h>
#include <util/delay.h>

// Definición de los estados del puente levadizo
typedef enum {
    espera = 0,
    elevando = 1,
    elevado = 2,
    bajando = 3
} estados_t;

// Definición de la estructura que contiene los datos del puente levadizo
typedef struct {
    float altura_actual;      
    float altura_maxima;      
    float altura_minima;   
} puente_t;

// Declaraciones de funciones
estados_t f_espera_puente(puente_t *puente);
estados_t f_elevado_puente(puente_t *puente);
estados_t f_elevando_puente(puente_t *puente);
estados_t f_bajando_puente(puente_t *puente);

#endif
