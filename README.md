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

```
### estados.c

```c
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
```

### funciones.c

```c
#include "mylib.h"

int init_puente() {
    GpioInitStructure_AVR salida;
    GpioInitStructure_AVR entrada;
    AdcInitStructure_AVR adc_config;
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

    REGBIT(avr_GPIOD_OUT, 1) = 1; //encender led verde
    REGBIT(avr_GPIOA_IN,0) = 1; //pulsador de subir en alto
    REGBIT(avr_GPIOA_IN, 1) = 1; //pulsador de bajar en alto
    return;
}

void bajar_barrera(void) {
    int i; 
    _delay_ms(2000);
    REGBIT(avr_GPIOD_OUT, 1)=0; //apagar LED verde
    //led rojo intermitente mientras se baja la barrera
    for (i = 0; i < 3; i++) {
        REGBIT(avr_GPIOD_OUT, 0) = 1;
        _delay_ms(1000); 
        REGBIT(avr_GPIOD_OUT, 0) = 0;
        _delay_ms(1000);
    }
    REGBIT(avr_GPIOD_OUT, 0) = 1; //encender LED rojo permanentemente
}

void activar_motor_subir(void) {
    REGBIT(avr_GPIOD_OUT, 4) = 1;   //activar el motor ENA=1
    // Rotación en dirección de elevación
    REGBIT(avr_GPIOD_OUT, 2) = 1;   // IN1=1
    REGBIT(avr_GPIOD_OUT, 3) = 0; //IN2=0
}

int leer_switch_subir(void) {
    if (avr_GPIOA_IN_0 == 0) {
        return 1;
    } else {
        return 0;
    }   
}

unsigned int leer_sensor(void) {
    _delay_ms(1000);
      return leer_ADC(avr_ADC_canal0);
}

int leer_switch_bajar(void) {
    if ((avr_GPIOA_IN & (1 << 1))==0) {
        return 1;
    } else {
        return 0;
    }   
}

void activar_motor_bajar(void) {
    REGBIT(avr_GPIOD_OUT, 4) = 1;    // Activar el motor
    REGBIT(avr_GPIOD_OUT, 2) = 0;  // Configuración para rotación en dirección de descenso
    REGBIT(avr_GPIOD_OUT, 3) = 1;
}

void subir_barrera(void) {
    int i; 

    for (i = 0; i < 3; i++) {
        clear_pin(avr_GPIOD_OUT, avr_GPIO_PIN_0);
        _delay_ms(1000); 
        set_pin(avr_GPIOD_OUT, avr_GPIO_PIN_0);
    }
}

void apagar_motor(void) {
    clear_pin(avr_GPIOD_OUT, avr_GPIO_PIN_4); //apagar el motor
}

```
### main.c
```c
#include "mylib.h"

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