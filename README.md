# Sistema controlador para puente levadizo

## Memoria descriptiva

El sistema de control del puente levadizo simula el funcionamiento de un puente que puede elevarse y bajarse para permitir el paso de embarcaciones y vehículos. El sistema opera a través de una máquina de estados que gestiona las transiciones entre cuatro estados principales: Espera, Elevando, Elevado y Bajando.

### Estado Inicial: Espera
En este estado, el sistema está en reposo, a la espera de una señal para iniciar el movimiento de apertura del puente. El puente se encuentra en posición horizontal permitiendo el paso de vehículos. Tras la señal de subir, el sistema activará los mecanismos para elevar el puente (pasando al estado "Elevando"). Las barreras de seguridad permanecen en posición alta para garantizar la seguridad del tráfico vehicular.

### Elevando
Al recibir la señal de apertura, el sistema baja las barreras de seguridad para detener el tráfico vehicular (led rojo) y comienza a elevar el puente. Este estado continúa hasta que el puente alcanza la altura máxima establecida. Durante todo el proceso, las barreras permanecen en posición baja, evitando el paso de vehículos. Una vez que se alcanza la altura máxima, el sistema queda en espera de una nueva señal, generalmente para iniciar el descenso.

### Elevado
En el estado "Elevado", el puente ha alcanzado su posición máxima, permitiendo el paso seguro de embarcaciones por debajo. Durante este estado, el sistema permanece en espera de una señal de cierre para iniciar el descenso. Las barreras de seguridad para vehículos permanecen bajas, evitando el tráfico sobre el puente hasta que el proceso de bajada haya concluido. Este estado asegura que el puente se mantenga elevado de manera estable y sin movimiento hasta que se indique el siguiente cambio.

### Bajando
Tras recibir la señal de cierre, el sistema activa los mecanismos para bajar el puente de manera controlada. Las barreras de seguridad permanecen bajas durante todo este proceso, evitando el paso de vehículos hasta que el puente haya descendido completamente. El estado "Bajando" se mantiene hasta que el puente alcanza la posición horizontal. Al finalizar, las barreras de seguridad se levantan (led verde), permitiendo nuevamente el paso de vehículos, y el sistema regresa al estado de "Espera".


## Maquina de estados

A continuacion, se presenta un modelo de maquina de estados para el proyecto.

![](./maquina.png)


## Presentacion en Proteus

Se utiliza para el proyecto un microcontrolador ATMega 128, un puente H L298 para controlar el motor, dos leds (rojo y verde) para simular la barrera, un motor DC de 12v, y un potenciometro para leer la altura del puente levadizo.

![](./proteus.png)

## Firmware

### mylib.h

```c

#ifndef MYLIB_H
#define MYLIB_H
#include <avr/io.h>
#include <avr_api/avr_api.h>
#include <util/delay.h>

#define ALTURA_MAX 127
#define ALTURA_MIN 0

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


```
### estados.c

```c
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

```

### funciones.c

```c
#include "mylib.h"

int init_puente() {
    GpioInitStructure_AVR salida;
    GpioInitStructure_AVR entrada;
    AdcInitStructure_AVR adc_config;
    SystickInitStructure_AVR systick_1;
    //configuración de pines de salida para LEDs y motor
    salida.port = OUT_PORT;
    salida.modo = avr_GPIO_mode_Output;
    salida.pines = LED_ROJO_PIN | LED_VERDE_PIN | ENA_PIN | IN1_PIN | IN2_PIN;
    init_gpio(salida);
    
    //condiguracion de pines de entrada para los switch
    entrada.port = SWITCH_PORT;
    entrada.modo = avr_GPIO_mode_Input; 
    entrada.pines = SWITCH_DOWN_PIN | SWITCH_UP_PIN; //input SUBIR / BAJAR
    init_gpio(entrada);

    // Configuración del ADC
    adc_config.mode = avr_ADC_MODE_Single_Conversion; // Conversión única
    adc_config.prescaler = avr_ADC_Prescaler_64;      // Divisor del reloj: 64
    adc_config.channel = avr_ADC_canal0;             // Canal ADC0 (PF0)
    adc_config.resolution = avr_ADC_RES_8Bit;       // Resolución de 8 bits
    adc_config.reference = avr_ADC_REF_AVcc;         // Referencia AVcc
    init_adc(adc_config);
    // Inicializar el ADC
    //if ( init_adc(*adc_config)!= avr_ADC_OK) {
     //   return -1;
    //}

    //configuracion del timer
    systick_1.timernumber = avr_TIM0;
    systick_1.time_ms = 1;
    systick_1.avr_systick_handler = systick_led;
    init_Systick_timer(systick_1);

    LED_VERDE = 1; //encender led verde
    SWITCH_DOWN = 1; //pulsador de subir en alto
    SWITCH_UP = 1; //pulsador de bajar en alto
    return;
}

void bajar_barrera(void) {
    int i, ciclos=0; 
    while (ciclos < 5) {
        if (led_time > 1000) {
            LED_ROJO ^= 1;
            led_time = 0;
            ciclos += 1;
        }
    }
    LED_VERDE=0; //apagar LED verde
}

void activar_motor_subir(void) {
    ENA = 1;   //activar el motor ENA=1
    // Rotación en dirección de elevación
    IN1 = 1;   // IN1=1
    IN2 = 0; //IN2=0
}

unsigned int leer_sensor(void) {
      return leer_ADC(avr_ADC_canal0);
}

void activar_motor_bajar(void) {
    ENA = 1;    //activar el motor
    //configuración para rotación en dirección de descenso
    IN1 = 0;  
    IN2 = 1;
}

void subir_barrera(void) {
    int i, ciclos=0;
    activar_motor_bajar;
    apagar_motor;
    while (ciclos < 5) {
        if (led_time > 1000) {
            LED_ROJO ^= 1;
            led_time = 0;
            ciclos += 1;
        }
    }
    LED_VERDE=1; //apagar LED verde
}

void apagar_motor(void) {
    ENA = 0; //apagar el motor
}

void systick_led(void) {
    led_time++;
}
```
### main.c
```c
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
```