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
#include "avr_api/avr_api.h"

#define ALTURA_MAX 127U
#define ALTURA_MIN 0

//definiciones para los pulsadores de subir y bajar
#define SWITCH_PORT avr_GPIO_A
#define SWITCH_DOWN_PIN avr_GPIO_PIN_1
#define SWITCH_UP_PIN avr_GPIO_PIN_0
#define SWITCH_UP avr_GPIOA_IN_0
#define SWITCH_DOWN avr_GPIOA_IN_1

// Definiciones de puertos de salida
#define PWM_PORT avr_GPIO_B
#define OUT_PORT avr_GPIO_D
#define LED_ROJO_PIN avr_GPIO_PIN_0
#define LED_VERDE_PIN avr_GPIO_PIN_1
#define ENA_PIN avr_GPIO_PIN_5
#define IN1_PIN avr_GPIO_PIN_2
#define IN2_PIN avr_GPIO_PIN_3
#define LED_ROJO avr_GPIOD_OUT_0
#define LED_VERDE avr_GPIOD_OUT_1
#define IN1 avr_GPIOD_OUT_2
#define IN2 avr_GPIOD_OUT_3
#define ENA avr_GPIOB_OUT_5
#define ADC0 avr_GPIOF_IN_0

typedef enum {
    espera = 0,
    elevando = 1,
    elevado = 2,
    bajando = 3
} estados_t;

//prototipos estados
estados_t f_espera_puente(void);
estados_t f_elevando_puente(void);
estados_t f_elevado_puente(void);
estados_t f_bajando_puente(void);

//prototipos funciones
void init_puente(void);
void subir_barrera(void);
void bajar_barrera(void);
void activar_motor_subir(void);
unsigned int leer_sensor(void);
void activar_motor_bajar(void);
void systick_led(void);
void stop(void);
void run(void);

#endif
```
### estados.c

```c
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
```

### funciones.c

```c
#include "mylib.h"

volatile unsigned int led_time=0;
PWMInitStructure_AVR pwm_config;

void init_puente(void) {
   GpioInitStructure_AVR salida, entrada, pwm, adc;
   AdcInitStructure_AVR adc_config;
   SystickInitStructure_AVR systick_conf;
   
   //configuracion de pines de salida
   salida.port = OUT_PORT;
   salida.modo = avr_GPIO_mode_Output;
   salida.pines = LED_ROJO_PIN | LED_VERDE_PIN | ENA_PIN | IN1_PIN | IN2_PIN;
   init_gpio(salida);
   
   pwm.port = PWM_PORT;
   pwm.modo = avr_GPIO_mode_Output;
   pwm.pines = ENA_PIN;
   init_gpio(pwm);
   
   adc.port = avr_GPIO_F;
   adc.modo = avr_GPIO_mode_Input;
   adc.pines = avr_GPIO_PIN_0;
   init_gpio(adc);
    
   //configuracion de pines de entrada para los switch
   entrada.port = SWITCH_PORT;
   entrada.modo = avr_GPIO_mode_Input; 
   entrada.pines = SWITCH_DOWN_PIN | SWITCH_UP_PIN; //input SUBIR / BAJAR
   init_gpio(entrada);

    //Configuracion del ADC
    adc_config.mode = avr_ADC_MODE_Single_Conversion; 
    adc_config.prescaler = avr_ADC_Prescaler_64;      // Divisor del reloj: 64
    adc_config.channel = avr_ADC_canal0;             // Canal ADC0 (PF0)
    adc_config.resolution = avr_ADC_RES_8Bit;       // mapeo de 8 bits
    adc_config.reference = avr_ADC_REF_AVcc;         // referencia AVcc
    init_adc(adc_config);

    
   LED_VERDE = 1; 
   //configuracion del timer
   systick_conf.timernumber = avr_TIM0;
   systick_conf.time_ms = 1;
   systick_conf.avr_systick_handler = systick_led;
   init_Systick_timer(systick_conf); 
    
   pwm_config.timernumber = avr_TIM1;  
   pwm_config.ClockSource = avr_TIM_Clock_NoClockSource;  
   pwm_config.output_Type = avr_TIM1_Out_Clear_OC1A; 
   pwm_config.dutyA = 45;  
   pwm_config.dutyB = 0;   
   pwm_config.dutyC = 0;  
   init_Fast_PWm_timer(pwm_config);
   sei();
   return;
}

void bajar_barrera(void) {
    int ciclos=0; 
    LED_VERDE=0; 
    while (ciclos < 5) {
        if (led_time > 500) {
            LED_ROJO ^= 1;
            led_time = 0;
            ciclos += 1;
        }
    }
    return;
}

void activar_motor_subir(void) {
    IN1 = 1;   // IN1=1
    IN2 = 0; //IN2=0
    run();
    return;
}

unsigned int leer_sensor(void) {
      return leer_ADC(avr_ADC_canal0);
}

void activar_motor_bajar(void) {
   //configuracion para descenso
    IN1 = 0;  
    IN2 = 1;
    run();
    return;
}

void subir_barrera(void) {
    int ciclos=0;
    while (ciclos < 5) {
        if (led_time > 500) {
            LED_ROJO ^= 1;
            led_time = 0;
            ciclos += 1;
        }
    }
    LED_VERDE=1;
    return;
}

void systick_led(void) {
    led_time++;
    return;
}

void stop(void) { 
   IN1=0;
   IN2=0;
   pwm_config.ClockSource = avr_TIM_Clock_NoClockSource;  
   init_Fast_PWm_timer(pwm_config);
   sei();
   return;
}

void run(void) {
   pwm_config.ClockSource = avr_TIM_Clock_SystemClockPrescalingx64;
   init_Fast_PWm_timer(pwm_config);
   sei();
   return;
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

## Funcionamiento del puente levadizo (video)
[![Título del Video](https://youtu.be/T_--LmrTiuc)]