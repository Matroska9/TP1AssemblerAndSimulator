### TP1AssemblerAndSimulator
### Trabajo práctico 1 - Ensamblador y simulador de procesador

El trabajo está hecho en Qt.

#### Arquitectura
Nuestro procesador es de 32 bits, big endian. El primer byte tiene el código de instrucción, y los tres bytes restantes el operando asociado a la instrucción. Si la instrucción no tiene operandos, los últimos tres bytes deben ir en cero. El procesador sólo tiene un registro, el acumulador, y una salida a pantalla LED que puede mostrar números.

#### Instrucciones:
- **LOAD** → `0x01`: Carga un valor desde la memoria en un registro u otro lugar de almacenamiento.
- **PRINT** → `0x02`: Imprime el valor del acumulador en un LCD.
- **ADD** → `0x03`: Suma dos valores y almacena el resultado en un registro.
- **SUB** → `0x04`: Resta un valor de otro y almacena el resultado en un registro.
- **MULT** → `0x05`: Multiplica dos valores y almacena el resultado en un registro.
- **DIV** → `0x06`: Divide un valor entre otro y almacena el cociente en un registro.
- **RSHF** → `0x07`: Desplaza bits hacia la derecha.
- **LSHF** → `0x08`: Desplaza bits hacia la izquierda.
- **AND** → `0x09`: Realiza una operación AND entre dos valores y almacena el resultado en un registro.
- **OR** → `0x0A`: Realiza una operación OR entre dos valores y almacena el resultado en un registro.
- **JMP** → `0x0B`: Salta a una ubicación específica en el código.
   
Recomiendo usar los .txt de la carpeta "ejemplos".
