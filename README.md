# Sistema controlador para puente levadizo

## Memoria descriptiva

El sistema de control del puente levadizo simula el funcionamiento de un puente que puede elevarse y bajarse para permitir el paso de embarcaciones y vehículos. El sistema opera a través de una máquina de estados que gestiona las transiciones entre tres estados principales: Espera, Elevando, y Bajando

### Estado Inicial: Espera

En este estado, el sistema está en reposo, a la espera de una señal para iniciar el movimiento del puente. El puente puede estar en posición horizontal (permitiendo el paso de vehículos) o en posición elevada (permitiendo el paso de embarcaciones). Dependiendo de la señal recibida, el sistema activará los mecanismos para elevar el puente (pasando al estado "Elevando") o para bajarlo (pasando al estado "Bajando").

### Elevando

Al recibir la señal de apertura, el sistema baja las barreras de seguridad para detener el tráfico vehicular y comienza a elevar el puente. Este estado continúa hasta que el puente alcanza la altura máxima establecida. Una vez que se alcanza esta altura, el sistema queda en espera de una nueva señal, generalmente para iniciar el descenso.

### Bajando

Tras recibir la señal de cierre, el sistema activa los mecanismos para bajar el puente de manera controlada. Las barreras de seguridad permanecen bajas durante este proceso para evitar el paso de vehículos. El estado "Bajando" se mantiene hasta que el puente alcanza la posición horizontal. Al finalizar, las barreras de seguridad se levantan, permitiendo nuevamente el paso de vehículos, y el sistema regresa al estado de "Espera".

## Maquina de estados

A continuacion, se presenta un modelo de maquina de estados para el proyecto.

![](./state_machine.png)
