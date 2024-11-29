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
   adc.pines = avr_GPIO_PIN_0;;
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
   
   pwm_config.timernumber = avr_TIM1;  
   pwm_config.ClockSource = avr_TIM_Clock_NoClockSource;  
   pwm_config.output_Type = avr_TIM1_Out_Clear_OC1A; 
   pwm_config.dutyA = 32;  
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
    run();
    // Rotacion en direccion de elevacion
    IN1 = 1;   // IN1=1
    IN2 = 0; //IN2=0
    if (leer_sensor == ALTURA_MAX) {
      stop();
    }
    return;
}

unsigned int leer_sensor(void) {
      unsigned l;
      l=(unsigned int)leer_ADC(avr_ADC_canal0);
      return l;
}

void activar_motor_bajar(void) {
    run();
   //configuracion para descenso
    IN1 = 0;  
    IN2 = 1;
    if (leer_sensor == ALTURA_MIN) {
      stop();
    }
    return;
}

void subir_barrera(void) {
    int ciclos=0;
    activar_motor_bajar();
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
   pwm_config.ClockSource = avr_TIM_Clock_NoClockSource;  
   init_Fast_PWm_timer(pwm_config);
   sei();
   return;
}

void run (void) {
   pwm_config.ClockSource = avr_TIM_Clock_SystemClockPrescalingx64;
   init_Fast_PWm_timer(pwm_config);
   sei();
   return;
}
