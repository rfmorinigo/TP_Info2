#include "mylib.h"

volatile unsigned int led_time=0;

void init_puente(void) {
   GpioInitStructure_AVR salida;
   GpioInitStructure_AVR entrada;
   AdcInitStructure_AVR adc_config;
   SystickInitStructure_AVR systick_conf;
   PWMInitStructure_AVR pwm_config;

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

    //Configuracion del ADC
    adc_config.mode = avr_ADC_MODE_Single_Conversion; 
    adc_config.prescaler = avr_ADC_Prescaler_64;      // Divisor del reloj: 64
    adc_config.channel = avr_ADC_canal0;             // Canal ADC0 (PF0)
    adc_config.resolution = avr_ADC_RES_8Bit;       // mapeo de 8 bits
    adc_config.reference = avr_ADC_REF_AVcc;         // referencia AVcc
    init_adc(adc_config);
    /*/
     Inicializar el ADC
      if ( init_adc(*adc_config)!= avr_ADC_OK) {
        return -1;
    }
    */
   LED_VERDE = 1; 
   SWITCH_DOWN = 1; 
   SWITCH_UP = 1; 
    
    //configuracion del timer
   systick_conf.timernumber = avr_TIM0;
   systick_conf.time_ms = 1;
   systick_conf.avr_systick_handler = systick_led;
   init_Systick_timer(systick_conf);
    
   // Configurar el temporizador 1 para PWM
   pwm_config.timernumber = avr_TIM1;  
   pwm_config.ClockSource = avr_TIM_Clock_SystemClockPrescalingx64;  
   pwm_config.output_Type = avr_TIM1_Out_Clear_OC1A; 
   pwm_config.dutyA = 0;  
   pwm_config.dutyB = 32;   
   pwm_config.dutyC = 0;  

   //pwm_config.avr_pwm_handler = mipwm();

   init_Fast_PWm_timer(pwm_config);
   
   sei();
   return;
}

void bajar_barrera(void) {
    int ciclos=0; 
    LED_VERDE=0; 
    while (ciclos < 6) {
        if (led_time > 500) {
            LED_ROJO ^= 1;
            led_time = 0;
            ciclos += 1;
        }
    }
    return;
}

void activar_motor_subir(void) {
    OCR1A = 32;  
    // Rotacion en direccion de elevacion
    IN1 = 1;   // IN1=1
    IN2 = 0; //IN2=0
    if (leer_sensor == ALTURA_MAX) {
      apagar_motor();
    }
    return;
}

unsigned int leer_sensor(void) {
      return leer_ADC(avr_ADC_canal0);
}

void activar_motor_bajar(void) {
    OCR1A = 64;   //activar el motor
   //configuracion para descenso
    IN1 = 0;  
    IN2 = 1;
    return;
}

void subir_barrera(void) {
    int ciclos=0;
    activar_motor_bajar();
    apagar_motor();
    while (ciclos < 6) {
        if (led_time > 500) {
            LED_ROJO ^= 1;
            led_time = 0;
            ciclos += 1;
        }
    }
    LED_VERDE=1; 
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
