#ifndef MYLIB_H
#define MYLIB_H
#include <avr/io.h>
#include <avr_api/avr_api.h>
#include <util/delay.h>

#define ALTURA_MAX 127
#define ALTURA_MIN 0

// Definición de los estados del puente levadizo
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

//prototipos micro
int init_puente(void);
void subir_barrera(void);
int leer_switch_subir(void);
void bajar_barrera(void);
void activar_motor_subir(void);
unsigned int leer_sensor(void);
int leer_switch_bajar(void);
void activar_motor_bajar(void);
void apagar_motor(void);

#endif
