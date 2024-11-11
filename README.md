# TPE

# Instrucciones de Compilación y Ejecución:

Para compilar el kernel hay  que entrar en la carpeta x64barebones y ejecutar el programa compile.sh, este asume el nombre del contenedor con la imagen proporcionada por la cátedra, por lo que seguramente haya que modificarlo. Para ejecutarlo solo es necesario correr el programa run.sh.

Además, incorporamos la opción de poder compilar poniendo compile.sh buddy para que se utilice el buddy manager en lugar del memory manager que implementamos para la primer pre-entrega.

Para ejecutar junto con el GDB, hay que partir desde dos terminales distintas. En una se va a ejecutar el programa run.sh pasandole como argumento gdb (./run.sh gdb). En la otra terminal, es necesario correr el gdb. En nuestro caso, utilizamos el que ya viene en la imagen de docker proporcionada por la cátedra, por lo que solo hay que inicializar y ejecutar el contenedor, posicionarse dentro de root y ejecutar el gdb. Además, se utiliza el ip del guest en el gdbinit, el cual no esta incluido en el repositorio.

Para ejecutar los tests desde la shell, se puede usar el comando “help” para ver los nombres de los comandos específicos, se puede usar anteriormente el comando dec para que se pueda visualizar todo su contenido en pantalla al mismo tiempo.