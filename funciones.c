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