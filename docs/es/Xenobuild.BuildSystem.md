

# Introducción

El ecosistema de herramientas para el lenguaje C++ es uno maduro, y con gran variedad. Sin embargo, en el área de los Sistemas de Construcción (Build Systems), es uno que palidece con respecto a las opciones que podemos encontrar en otros ecosistemas de lenguajes, como Gradle o Node Package Manager.
Los principales problemas que surgen a partir de los build system tradicionales son:
- Bajo nivel de abstracción: Proveen una poca capacidad de abstracción sobre el proceso de compilación. Esto provoca que el desarrollador tenga que invertir grandes cantidades de tiempo para corregir los archivos de definición.
- Poca o nula gestión de paquetes: Esto provoca que el desarrollador tenga que descargar, compilar e instalar paquetes externos de librerías de forma separada.
- Poco soporte para múltiples compiladores y targets: En CMake, el desarrollador debe configurar (llámese, ubicar dependencias externas, establecer opciones de compilación, etc) varias veces la compilación de un mismo paquete cada vez que quiere y/o debe usar un compilador distinto, o el mismo compilador con diferentes configuraciones.
Es por esto que nace “borc”, una alternativa que toma inspiración de las mejores características de los gestores de compilación de otras plataformas para remediar la situación antes descrita.
Problemas de otras herramientas de construcción a Resolver
Los problemas que se encuentran en estos build systems son:
1.	Los DSL’s para definir los modulos a compilar, son por definición turing-complete: Esto, si bien permite una gran flexibilidad, en la práctica conduce a complejos archivos para definir proyectos no-triviales.
2.	Gestión de las dependencias: CMake tiene un sistema de librerias/paquetes, que solo está definido para referenciar dependencias locales. Los proyectos deben definir muchas veces procesos manuales para “automatizar” en parte este problema. Boost, por ejemplo, debe ser compilado y descargado de forma manual.
3.	Builds manuales: Para agregar un nuevo compilador / tipo de construcción, en CMake, se debe reproducir el proceso de configuración desde 0, definiendo manualmente los paths hacia las dependencias externas. Peor aún si un proyecto está definiendo multiples arquitecturas y compilación.
4.	Pipelines personalizados de compilación: Algunos proyectos (como videojuegos, o interfaces gráficas como Qt), requieren pipelines de compilación personalizados para compilar archivos fuente que no corresponden a los estándares de C++. Estos frameworks proveen compiladores adicionales para que se puedan conectar al pipeline de compilación. Sin embargo, para crear estos complementos, es necesario introducirse en la arcana definición para crear reglas de compilación.
5.	Dificultad/manualidad durante el proceso de desarrollo de los componentes: Durante los procesos iterativos de desarrollo, se van creando diversos módulos que, posteriormente, es necesario refactorizar código existente en nuevos módulos con funcionalidades más refinadas. Esto implica un cierto esfuerzo de trabajo en crear y mantener las reglas de definición de los componentes cuando podría ser automatizado. Lo mismo aplica para crear proyectos desde 0: Es necesario crear la estructura de los proyectos desde 0 cuando podría ser generada automáticamente en base ciertos parámetros base que ingresa el usuario (de forma similar a como lo hace el tooling de Angular)
6.	Interactividad en lenguajes cross-lenguaje: Crear proyectos cross-lenguaje se torna complicado cuando el sistema de construcción base no entiende su lenguaje primario, debiendo crear hacks y forzando la compilació.
7.	Incapacidad de ejecutar componentes que dependen de bibliotecas dinamicas: En CMake, es muy engorroso ejecutar componentes que dependen de librerias dinamicas dentro del mismo proyecto, por lo que es necesario realizar un proceso manual con las variables de entorno PATH para lograrlo.
8.	Integración con sistemas de construcción pre-existentes: CMake es el estándar defacto para las herramientas de construcción de aplicaciones C++ en la actualidad.
# Modelo de Construcción del lenguaje C/C++
El modelo de construcción de componentes para el lenguaje C/C++ es uno que tiene, generalmente, dos etapas:
- Compilación: Los archivos source (C Source Files y C++ Source Files, con extensión .c y .cpp, respectivamente), son compilados usando un comando de compilación por línea de comandos (cc, gcc, clang o cl, entre otros), y éstos generan un archivo .o ó .obj (dependiendo de la plataforma), denominado código objeto. El código objeto contiene las instrucciones precompiladas del archivo source correspondiente, en lenguaje intermedio. A pesar de que este código intermedio aún no es ejecutable, los archivos de código objeto son útiles para fragmentar lógicamente un componente en C++. La correspondiencia entre archivos source y archivos de código objeto es uno a uno. Además de la compilación de archivos C/C++ para generar código objeto, también tenemos otros tipos de archivos fuente lo que generan, tales como archivos ASM (específicos para ciertas arquitecturas de instrucciones, como x86, ), y archivos de recursos (.rc en Windows).
- Vinculación: El proceso de vinculación se encarga de tomar como entrada todos los archivos de código objeto relacionados con un componente en particular, y de éstos genera un componente ejecutable, para una plataforma específica (ya sea Linux, Windows, macOS, o bien la web).
Resumiendo: el proceso genérico para generar un componente ejecutable, en base a lo recientemente expuesto, es el siguiente:
- C/C++ Source → Object File (mapeo de uno a uno).
- Assembly Source → Object File (mapeo de uno a uno).
- Object File (s) → Component Executable (mapeo de muchos a uno).
Hasta ahora, este proceso es bastante simple. Iteraremos sobre este modelo para incorporar nuevas aristas para llevarlo a algo más concreto.
# Tipos de Componentes
Para obtener un componente ejecutable el Vinculador no lo puede saber a partir únicamente del código objeto: necesita una opción, suministrada por el build system durante el tiempo de construcción, que indica qué tipo de componente generar. Estos tipos son los siguientes:
- Aplicación de Consola: AKA aplicación por línea de comandos.
- Aplicación de Escritorio: Aplicación con interfaz gráfica de usuario.
- Librería Estática: Colección de código objeto precompilado, el cual se “inyecta” al componente y dejar de ser una dependencia en tiempo de ejecución, para pasar a ser sólo una dependencia en tiempo de compilación.
- Librería Dinámica. Librería de enlace dinámico, usada para ser compartida entre distintos componentes para reducir uso de recursos en el sistema. Como dependencia, es una tanto una en tiempo de compilación como de ejecución.
Al incluir estas variables, el proceso se complica un poco, considerando que:
- Windows: la compilación de una librería dinámica genera dos archivos: la DLL (el componente reutilizable en tiempo de ejecución) y la import library (requerido para enlazar durante la generación del ejecutable). NOTA: por defecto ningún símbolo es expuesto en la librería de enlace dinámico, por lo que es necesario decorar los elementos a exponer usando declaraciones del tipo __declspec(dllexport) al compilar, y __declspec(dllimport) al momento de usar la librería.
- Linux: la compilación de una librería dinámica genera un único archivo: el shared object (.so). Este shared object es usado tanto como dependencia en tiempo de ejecución como de compilación.
En base a lo anterior, el mapeo quedaría así:
- (C/C++ Source | Assembly Source) → Object File (mapeo de uno a uno).
- (Object File *) → ( Executable | Static Library | (Dynamic Library | Import Library[Windows] ) )
# Dependencias
¿Qué pasa cuando se necesita incorporar una librería externa? Tradicionalmente, éstas se obtenían en dos formas distintas, con una tercera comenzando a ganar gran popularidad en el último tiempo, precisamente porque la comunidad reconoce la falta de un buen sistema de construcción genérico y flexible:
- Dependencia en Código Fuente: Es una dependencia que distribuye su código fuente íntegramente para ser compilada de forma local. Tiene la gran ventaja de que es posible que sea configurada para que pueda incluir ciertas características que una dependencia precompilada no pueda suministrar. La desventaja recae en que es necesario que el desarrollador invierta tiempo en descargarla, desempacarla, compilarla e instalarla de forma manual antes de siquiera empezar a integrarla con su build system.
- Dependencia precompilada: Es una dependencia que ya se distribuye preparada para ser utilizada nada más descargada, por lo que es bastante conveniente en este sentido, ahorrando el tiempo de compilación. Sin embargo, si se requiere trabajar con una plataforma y/o arquitectura distintas con las que fue preparada esta dependencia, no será posible usarla, a menos que se cambien tal plataforma o tal arquitectura.
- Dependencia header-only: Es una dependencia que se distribuye únicamente con el header correspondiente. Es muy conveniente, ya que únicamente se debe referenciar desde el build system correspondiente para empezar a ser utilizada.
Sin importar el tipo de dependencia, éstas proveen de los siguientes elementos a los componentes que las necesiten para que se pueda realizar la integración. ya sea en tiempo de ejecución o de compilación:
- Cabeceras: Proveen de APIs (en forma de funciones, clases, variables globales, entre otras), para habilitar la reutilización de código.
- Definiciones de Preprocesador: Usadas para configurar la compilación, y definir ciertas interfaces dependeniendo (entre otras variables), de la plataforma o de la arquitectura.
- Librerías (estáticas, dinámicas): Usadas para proveer código precompilado, o compilado dentro del contexto actual (se clarificará este punto durante la especificación de la solución).
Los toolchains para el lenguaje C++ proveen, en general, el siguiente tipo de opciones para permitir el uso de los elementos de las dependencias expuesto más arriba:
- Include Path: Ruta adicional dentro del sistema de archivos que usa el compilador para ubicar cabeceras referenciadas dentro del código fuente.
- Library Path: Ruta adicional dentro del sistema de archivos que usa el vinculador para ubicar las librerías referenciadas por los componentes ejecutables.
Considerando lo anterior, el proceso de compilación quedaría así:
- C/C++ Source (depends on: Preprocessor Defines, IncludePath) → Object File (1:1)
- Assembly Source (depends on: Preprocessor Defines, IncludePath) → Object File (1:1).
- (Object File *) → ( Executable | (Dynamic Library | Import Library[Windows] ) ) (depends on: LibraryPath, LinkLibraryFile)
- (Object File *) → (Static Library). (Las librerías estáticas se usan en la vinculación final).
# Plataforma y Arquitectura
Fue expuesto hace un momento de forma dispersa, pero se consolidará en este apartado.
Existen componentes cuyo código fuente está diseñado para ser ejecutados sobre una plataforma y arquitectura específica, o sin importar la plataforma y la arquitectura en que se está ejecutando. Para el caso de las fuentes en lenguaje ensamblador es un poco especial, debido a que están escritas para una arquitectura específica, es necesario especificar a qué arquitectura corresponden, qué dialecto usan, y cuál es su ensablador correspondiente. Tenemos como ejemplo a NASM, GAS, YASM, MASM, entre otros.
# Enlace de la Runtime Library
Algunos compiladores proveen distintas versiones de la librería en tiempo de ejecución del lenguaje. En el caso de Visual C++, por ejemplo, tenemos las siguientes versiones de su Visual C++ Runtime Library:
- Static Debug Multithreaded
- Static Release Multithreaded
- Dynamic Debug Multithreaded
- Dynamic Release Multithreaded

# Visión
BORC debe ser una herramienta que permite reducir al máximo el tiempo y esfuerzo que el desarrollador invierte en el sistema de construcción/compilación de sus soluciones de software, facilitando, automatizando y acelerando (según donde sea aplicable) las siguientes actividades (que mapean a funcionalidades del Producto):
- Definición de las reglas de construcción.
- Refactorización de la estructura física de la solución de software.
- Ubicación, descarga y construcción de las dependencias.
- Interoperación con otras sistemas de construcción (CMake es una prioridad!!).
- Personalización/extensibilidad del proceso de construcción
- Generación de Código (clases, patrones de diseño?).
# Criterios de diseño:
## Criterio general de diseño
Facilitar, automatizar y acelerar las actividades relacionadas con la definición y construcción de una solución de software, de tal forma que el tiempo que se quita el foco en el software sea mínimo (en todo sentido).
## Reglas de Construcción
- Simple e intuitivas: Una persona que no conoce la sintaxis de las reglas de construcción debe ser capaz de poder entenderlas inmediatamente.
- Preferir el estilo Declarativo sobre el Imperativo: Un estilo declarativo tiene la ventaja de ser más fácil de usar. Uno imperativo provee más flexibilidad.
- Proveer un nivel de abstracción superior con respecto a los otros sistemas de construcción.
Metodología de Desarrollo
Estas son algunas directrices a seguir a rajatabla en el proceso de diseño y desarrollo. El primer MVP (o versión 1.0) debe ser capaz de:
1.	Auto-hostearse!
2.	Compilar paquetes que contengan aplicaciones de consola y librerías, usando gcc y Visual C++, sobre Windows y Linux
3.	Descargar dependencias via HTTP y desde Git.
## Partir Monolítico, refactorizar gradualmente
La idea acá es cumplir con los focos en cada iteración de desarrollo. El board inicial contemplará las HdU que permitirán acercarnos a la versión 1.0. Al llegar a la versión 1.0 deberíamos tener varios módulos (a lo menos dos), los cuales se compongan por librerías y un ejecutable.

# Especificación Funcional
Esta publicación describe las interacciones entre el usuario y el sistema de software que nos encontramos construyendo. Recordar que BORC debe solucionar el proceso de construcción de paquetes y la gestión de la instalación, desinstalación y actualización sus dependencias.
## Análisis Requerimientos Funcionales
### Interfaz de Usuario
Recordemos que un paquete borc se define por un conjunto de reglas que describen la estructura de sus componentes y sus dependencias. Ahora bien, el usuario puede elegir con cuales toolchains construir, cuales son las arquitecturas y plataforma objetivos a las que puede apuntar (cross-compiling), y cual es el build-type actual.
Es por esto que se debe diseñar una estructura de datos en la que almacenar las referencias a estos elementos, dentro del repositorio (ya que esto es particular para cada usuario que haga uso del paquete).
El usuario interactuará con la aplicación por una interfaz de línea de comandos, llamada “borc”, y que pondrá a disposición ciertos subcomandos, con los cuales se administrará indirectamente la estructura de datos antes dicha. Cada subcomando debe ser una acción, ya que el desarrollador siempre trabaja enfocado en un workflow dentro de un tiempo determinado. Entre los comandos identificados tenemos:
- configure
Establece ciertas variables requeridas tanto por el paquete (para configurar su construcción con ciertas opciones), así como también las opciones requeridas por alguna dependencia directa (o posiblemente, indirecta, sujeto a evaluación funcional).

- manage
Maneja las dependencias (descarga, incorporación, actualización, etc)versiones, las ubica en caso de ser externas), maneja los toolchains (agrega, elimina, comprueba, establece el actual), maneja los build-types (agrega, elimina debug, release, coverage).
build
Compila las dependencias, compila incrementalmente las fuentes asociadas a un paquete o modulo en particular de ese paquete.
NOTA: La compilación incremental compila únicamente una fuente si ésta o una de sus dependencias fue modificada después de la ultima compilación.
- run
Ejecuta una aplicación del paquete, estableciendo las variables de entorno apropiadas para ubicar sus dependencias en tiempo de ejecución (en caso de que use alguna librería dinámica), por ejemplo.
Comando “configure“
Comando “manage“
 
Comando “build“
 
Comando “run“
 
### DSL Declarativo
Se utilizará un DSL del tipo declarativo, para especificar componentes de software, llamados “paquetes”, que constan de un conjunto de módulos, sus fuentes y dependencias (ya sean éstas compilables o preconstruidas). Usar un DSL declarativo tiene las siguientes ventajas:
- Análisis estático.
- Comprensible a primera vista.
Este DSL declarativo está definido, por ahora, en JSON. A continuación, tenemos un archivo de definición del paquete:
 
Las reglas de construcción en Borc son “modulares” y explícitas. Se deja en claro, directamente por medio de sus rutas, y por reglas de carga, que el paquete contiene un componente, definido en la ruta relativa “modules/borc/module.borc”. Borc cargará recursivamente todas las sentencias “definition_files” para ir incorporando nuevos componentes dentro del paquete.
El archivo de reglas para “src/borc/module.borc” es el siguiente:
 
En este archivo de reglas, se dejan explícitas cuales son las dependencias para esta librería en particular.
TODO:
•	Definir pruebas automatizadas, header y modulos
•	Dependencias en tiempo de compilacion / en tiempo de ejecucion
•	Dependencias publicas y privadas.
•	Dependencias en librerias estáticas y dinamicas
•	En donde definir el tipo de linkae contra la libreria estandar
•	En donde definir el tipo de arquitectura y plataforma
# Análisis Requerimientos No Funcionales
## Lenguaje de las Reglas de Construcción
Las reglas de construcción se definirán usando un lenguaje declarativo, ya que con esto se facilita mucho la comprensión de los componentes de software. Entre los candidatos para esto tenemos:

 

## Modelo de Dominio de la Solución
La solución propuesta establece ciertas entidades que proveen un cierto nivel de abstracción sobre el modelo de construcción subyacente para un amplio tipo de aplicaciones.
Primero, se describirán a grandes rasgos los elementos, y luego, se concretizará a nivel de Entidades el modelo vía un diagrama, junto con sus propiedades y las relaciones entre ellos.
Tiempos
Los “tiempos” son instantes en que se produce un cierto “workflow” desde el punto de vista de un Paquete (descrito más adelante). Identificamos los siguientes:
- Tiempo de Construcción. Momento en que un paquete es construido.
- Tiempo de Ejecución. El tiempo de ejecución es el tiempo en que un componente “ejecutable” es ejecutado a petición del desarrollador.
## Estructura de Paquetes
La unidad de trabajo equivalente a lo que podríamos considerar Proyecto, Workspace, o Solución es el Paquete, que corresponde a una colección cohesiva de Componentes interrelacionados. Estos componentes pueden corresponder a librerías, ejecutables, u otros tipos dependiendo de otras variables. Para que BORC pueda construir estos artefactos, depende de un Toolchain, proveído directamente por el entorno de desarrollo local del usuario, el cual debe corresponder y/o ser compatibles con la tecnología/lenguaje/framework usado en esos componentes. Estos artefactos se componen de uno o varios archivos de código fuente, y pueden estar escritos en código portable, o usando código específico de ciertas plataformas y/o arquitecturas. En este último caso, se debe especificar a nivel del Build System este soporte.
### File
Archivo que al ser usado como entrada en un Pipe de compilación, para generar otro archivo. Los File’s pueden provenir desde el disco local, siendo referenciados desde una ruta relativa o absoluta.
### Component
Es un artefacto generado durante el proceso de construcción. Un artefacto se compone de uno o varios archivos de código fuente, escritos en uno o varios lenguajes de programación, y tiene una serie de meta-datos asociados, tales como:
- Nombre.
- Lenguaje(s) de programación usados.
- Listado de archivos de código fuente y/o directorios
- Tipo de Componente
- Dependencias (de paquetes y/o componentes)
### Package
Es una colección de componentes (ejecutables, librerías, etc), que conforman un paquete determinado.
### Dependencias
Para posibilitar el consumo de librerías externas, los artefactos pueden referenciar otros artefactos (librerías u otros). Estas dependencias externas pueden venir en las siguientes formas:
- Código fuente completo, usando BORC como Build System.
- Código fuente completo, usando otro Build System externo (por ejemplo, Makefiles, CMake, Meson, QMake)
- Paquete con artefactos precompilados, el cual se encuentra instalado previamente en el sistema de archivos local.
- Paquetes que proveen tanto librerías como ejecutables y que incorporan nuevos ajustes en la forma en que se construyen los paquetes. Estos pueden ser conocidos como Frameworks, se pueden importar otros componentes que provean un complemento externo al toolchain empleado (como ejemplo Qt, que tiene un compilador de interfaces de usuario y un Meta-Object-Compiler, para generar metadatos a partir de classes C++).
### Toolchain
- Tool: Herramienta de línea de comandos que se usa para proveer la transformación requerida por un pipe. Tiene parámetros de entrada para controlar parte del funcionamiento del proceso de construcción.
- Toolchain: Conjunto de Tools, que implementan todo (o una parte) de la infraestructura necesaria para construir paquetes.
Build Pipeline
El Build Pipeline provee una capa de abstracción para la construcción de paquetes. Describe en terminos generales las distintas tuberías (entradas y salidas) para irlas conectando dependiendo de la operación.
### FileType
Patrón de expresión regular que permite identificar el tipo de un SourceFile a partir de su propiedad Name. Por ejemplo:
- CMake Build Definition: CMakeLists.txt, *.cmake
- C Header File: .h
- C++ Header File: .h, .hh, .hxx, .h++, .hpp
- etc
### PipePin
TODO
Pipe
Regla de mapeo que indica cómo se transforman Files correspondiente a un cierto FileType en otro tipo de Files. Estas reglas se definen como:
- Input: Determina el tipo de archivo requerido para que este Pipe se pueda disparar y la cardinalidad.
- Tool: La herramienta de línea de comandos que se ejecutará para realizar la transformación. Esta herramienta puede ser proveída por una Dependencia.
- ArgumentList: Listado de argumentos, que influencian el proceso para generar la salida.
- Output: Determina el tipo de archivo de destino que se genera cuando el Pipe es ejecutado, junto con la cardinalidad.
### Pipeline
Conjunto de Pipes que, en cooperación complementaria entre ellos, permiten construir conjuntos de componentes de software.
# Especificación de la Solución Técnica.
Si bien BORC está siendo pensado para casos de uso de aplicaciones C++ y pipelines de assets para videojuegos, para cumplir con el requisito de la “flexibilidad”, se requiere de un modelo de dominio genérico para sistemas de construcción, o, en otras palabras, se requiere modelar un “build engine”. El build engine proveerá el Core necesario para soportar las operaciones de construcción de una solución de software.
