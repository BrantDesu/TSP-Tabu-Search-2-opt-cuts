Implementacion Tabu Search + Greedy para TSP, entrega 3 IAA.

Cristian Chamorro Brant 201673506-2

Mejoras: 

- Nuevo movimiento implementado, 2-opt-cuts.
- Sintonizador TLLengthMultiplier.

Consideraciones:

- Para obtener registro de los outputs, la carpeta "Outputs_TSP" debe estar creada.

- Algunas instancias no coinciden en formato, dado que estaba corto de tiempo preferí modificar directamente los archivos de estas, el problemas al que me refiero es que 3 de ellas en la descripcion de las primeras lineas utiliza un espacio antes de ':' quedando como un string distinto al parsearlo (e.g. NAME : d198 ) el resto de ellas (2) no utilizan ese espacio, por lo que decidí dejarlas todas con la separación. Si se prueban otras instancias y tienen conflictos con el formato, el programa se caerá, para probar estas nuevas instancias, basta con incluirlas en la carpeta de "Instancias_TSP".

- Para esta entrega se implementó un sintonizador simple para probar valores del parámetro TLLengthMultiplier, los valores que se probarán son controlados por los parámetros {intervalMin, intervalMax, intervalLength}. Una descripción de estos parámetros se encuentra al final del documento.

- Dentro del makefile se incluyeron distintas variables para pasar cada uno de los parámetros del programa por consola, el formato de los parámetros es el siguiente: 

      maxIterations maxTime intervalMin intervalMax intervalLength SEEDS

**maxIterations**: Iteraciones fallidas máximas que ejecutará el algoritmo antes de detener la prueba.  
**maxTime**: Tiempo máximo que se ejecutará el algoritmo antes de detener la prueba.  
**intervalMin**: Valor mínimo del intervalo a entregar al sintonizador.
**intervalMax**: Valor máximo del intervalo a entregar al sintonizador.
**intervalLength**: Cantidad de valores que tendrá el itervalo.
