
# Objetivo

Especificar las características que debe poseer una herramienta destinada a apoyar en la construcción de proyectos de software en lenguaje C++, para facilitar la gestión de los distintos aspectos en la generación, construcción y empaquetamiento de proyectos C/C++ en distintas plataformas host y target.

El objetivo es que hacer que la integración de proyectos C++ con librerías externas sea lo más placentero posible respecto de la forma en que se maneja actualmente.

## Misión
Desarrollar una herramienta transversal al desarrollo de software nativo, que integre compiladores, librerías, analizadores, sistemas de construccion, empaquetadores y depuradores, permitiendo a los desarrolladores enfocarse en el desarrollo pleno de su producto.

## Visión
Ser reconocida como la herramienta estándar en el desarrollo de software nativo en C++.

# Especificación Funcional
## Introduccion
Xenobuild debe ser una herramienta capaz de proveer ayudas para los siguientes ciclos de trabajo:

- Manejo automático de dependencias.
- Actividades locales de desarrollo.
- Soportar flujos de trabajo bajo ambientes **CI/CD**
- Interacción e integración con herramientas ya consolidadas en la industria.
- Apoyo en el empaquetado del producto final.
- Versatilidad en el desarrollo. Se deben soportar múltiples plataformas, sistemas operativos, compiladores y lenguajes.
- Optimización de tiempos de construcción.

Esto deriva en un resumen de las siguientes funcionalidades que deben ser desarrolladas, para atacar las necesidades anteriormente mencionadas:

## Gestión de Configuraciones

Con Gestión de configuración nos referimos al hecho de que, a partir de un árbol de código fuente, se pueden generar una o más configuraciones, que generan artefactos ejecutables (u otros) especializados, que van destinados para distintos sistemas operativos, plataformas, etc.

Por ejemplo, si se toma encuenta un juego desarrollado con la librería SDL, se pueden generar las siguientes configuraciones:

- Plataforma Linux, usando el compilador nativo del sistema.
- Plataforma Web, usando el compilador Emscripten.

Estas dos configuraciones de construcción son generadas a partir del mismo árbol de código fuente, usando un conjunto de toolchains distintos para cada una de ellas.

Algunas plataformas soportan más de una arquitectura de procesador. En el caso de plataformas Intel, se pueden soportar arquitecturas de 32 o 64 bits.

Por tanto, podemos decir que una **Configuración** es una selección de las propiedades finales de una compilación (o construcción) de código, usando un toolchain específico. Se compone de los siguientes elementos:

- **Target Platform**: Plataforma Destino. Es un *triplet* (3-tupla), que codifica el **Sistema Operativo** y **Arquitectura de CPU**.
- **Compiler**: Toolchain de compilación usado para generar el código para la plataforma de destino.
- **Build Type**: Corresponde a un Tipo de Construcción.

### Configuración de Targets
- Target plataforma Host 
- Target otras plataformas (cross-build).
### Configuración de BuildTypes
- Debug y Release
- Otros BuildTypes definidos por el usuario.
### Soporte para multiples Build-Systems
- Soporte para CMake.
- Soporte para Configure / Make.
- Soporte para Meson.

## Gestión de dependencias
### Tipos de dependencias admitidas.
- Librerías estáticas.
- Librerías dinámicas
- SDKs
    - SDKs pueden proveer librerías, y ejecutables que sirven como compiladores para otros tipos de archivos (como el SDK de Vulkan, por ejemplo).
### Ubicación de las dependencias
- Ruta de la dependencia
  -  Directorio Local
     - Se puede indicar una ruta absoluta
     - Se puede indicar una ruta relativa.
  - Repositorio de código Git
    - Se puede referenciar un {branch / tag / commit} especifico como parámetro de la descarga.
    - Se puede indicar una ruta relativa al mismo repositorio Git del proyecto. Esto es util en los repositorios con varios subproyectos (ej: Monorepo).
    - El repositorio Git puede requerir autenticación de Git
      - Autenticación por usuario / pass
      - Autenticación por clave ssh
      - Autenticación por Key (depende del proveedor?).
  - Repositorio de código SVN.
    - Se puede hacer referencia a un release especifico para la integración.
    - El repositorio SVN puede requerir autenticación.
      - Autenticación por usuario / pass.
  - Descarga de archivo por URL HTTP.
    - Se puede indicar una ruta parcial dentro del archivo comprimido. Esto es util para paquetes comprimidos que tengan multiples proyectos.
    - Servidor HTTP puede requerir autenticación.
    - Autenticación con usuario / password.
  - Descarga de archivo por Servidor FTP.
    - Se puede usar una ruta parcial para indicar el archivo a descargar.
    - Servidor FTP puede requerir autenticación:
    - Autenticación con usuario / password.
  - Consideración: Los paquetes comprimidos se pueden validar mediante el computo de un Hash.
  - Consideración: Los paquetes comprimidos requieren descompresión para obtener el código a compilar.
- Formatos de entrega de paquetes comprimidos (una vez obtenida la dependencia):
  - Formato tar.gz
  - Formato zip
  - Formato rar
  - Directorio
### Formas de integrar las dependencias, en etapa de construcción (Build):
- Dependencia es código fuente.
  - Código fuente que requiere ser compilado.
  - Código fuente que no requiere ser compilado (header-only).
- Dependencia es código precompilado.
  - Compilado para uno o mas Triplets
    - Compilador, Arquitectura, Plataforma.
- Enlazado automático de dependencias al momento de Construir, Ejecutar y Empaquetar, de acuerdo al tipo de dependencia.

## GESTION DE EJECUCIONES
## GESTION DE EMPAQUETAMIENTO
## GESTION DE PUBLICACIONES
## MISCELANEOS
- Wrappers de comandos (similar a npm run), para simplificar tareas rutinarias.
- Poder ejecutar una línea de comando especifica para ciertas tareas rutinarias, como linting, pruebas unitarias, análisis estático, etc.
