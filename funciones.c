#include "mylib.h"

volatile unsigned int led_time=0;

void init_puente(void) {
    GpioInitStructure_AVR salida;
    GpioInitStructure_AVR entrada;
    //AdcInitStructure_AVR adc_config;
    SystickInitStructure_AVR systick_conf;

    //configuracion de pines de salida
    salida.port = OUT_PORT;
    salida.modo = avr_GPIO_mode_Output;
    salida.pines = LED_ROJO_PIN | LED_VERDE_PIN | ENA_PIN | IN1_PIN | IN2_PIN;
    init_gpio(salida);
    
    //configuracion de pines de entrada para los switch
    entrada.port = SWITCH_PORT;
    entrada.modo = avr_GPIO_mode_Input; 
    entrada.pines = SWITCH_DOWN_PIN | SWITCH_UP_PIN; //input SUBIR / BAJAR
    init_gpio(entrada);

    /*Configuracion del ADC
    adc_config.mode = avr_ADC_MODE_Single_Conversion; 
    adc_config.prescaler = avr_ADC_Prescaler_64;      // Divisor del reloj: 64
    adc_config.channel = avr_ADC_canal0;             // Canal ADC0 (PF0)
    adc_config.resolution = avr_ADC_RES_8Bit;       // mapeo de 8 bits
    adc_config.reference = avr_ADC_REF_AVcc;         // referencia AVcc
    init_adc(adc_config);
    /*/
    // Inicializar el ADC
    //if ( init_adc(*adc_config)!= avr_ADC_OK) {
     //   return -1;
    //}
    
    LED_VERDE = 1; 
    SWITCH_DOWN = 1; 
    SWITCH_UP = 1; 
    
    //configuracion del timer
    systick_conf.timernumber = avr_TIM0;
    systick_conf.time_ms = 1;
    systick_conf.avr_systick_handler = systick_led;
    init_Systick_timer(systick_conf);
    
    return;
}

void bajar_barrera(void) {
    int ciclos=0; 
    while (ciclos < 4) {
        if (led_time > 500) {
            LED_ROJO ^= 1;
            led_time = 0;
            ciclos += 1;
        }
    }
    LED_VERDE=0; 
    return;
}

void activar_motor_subir(void) {
    ENA = 1;   //activar el motor ENA=1
    // Rotacion en direccion de elevacion
    IN1 = 1;   // IN1=1
    IN2 = 0; //IN2=0
    return;
}

unsigned int leer_sensor(void) {
      return leer_ADC(avr_ADC_canal0);
}

void activar_motor_bajar(void) {
    ENA = 1;    //activar el motor
    //configuraciÃ³n para rotacion en direccion de descenso
    IN1 = 0;  
    IN2 = 1;
    return;
}

void subir_barrera(void) {
    int ciclos=0;
    activar_motor_bajar();
    apagar_motor();
    while (ciclos < 4) {
        if (led_time > 500) {
            LED_ROJO ^= 1;
            led_time = 0;
            ciclos += 1;
        }
    }
    LED_VERDE=1; //apagar LED verde
    return;
}

void apagar_motor(void) {
    ENA = 0; 
    return;
}

void systick_led(void) {
    led_time++;
    return;
}