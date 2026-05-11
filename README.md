# Documentación Técnica del Sistema de Gestión de Listas de Kill Team

## 1. Introducción General del Proyecto

El presente proyecto consiste en el desarrollo de una aplicación de consola escrita en lenguaje C, destinada a la gestión digital de listas de equipo (rosters) para el juego de escaramuzas tácticas _Warhammer 40,000: Kill Team_. El sistema permite construir y mantener una base de datos maestra de operativos (unidades) disponibles, para posteriormente componer, editar, consultar y exportar listas de equipo que cumplan con una serie de reglas de negocio derivadas tanto de la mecánica del juego como de requisitos funcionales específicos definidos para el alcance académico del proyecto.

El dominio del problema se fundamenta en la información contenida en los documentos de reglas de facción proporcionados, los cuales detallan los operativos, estadísticas, armas, equipamiento y restricciones de composición para cuatro facciones jugables: _Angels of Death_ (Space Marines), _Kommandos_ (Orks), _Mandrakes_ (Drukhari) y _Void-Dancer Troupe_ (Harlequins Aeldari), además de un compendio de equipamiento universal aplicable a todas las facciones. El sistema no busca replicar la totalidad de las reglas de juego de _Kill Team_, sino digitalizar específicamente los procesos de catalogación de unidades, armado de listas, asignación de equipamiento y generación de reportes.

La solución se construye bajo un paradigma estrictamente procedural, utilizando estructuras de datos compuestas, listas simplemente enlazadas abiertas y persistencia mediante archivos binarios, con énfasis en la optimización del consumo de memoria y la robustez ante entradas de usuario inválidas.

---

## 2. Objetivos y Alcance

### 2.1 Objetivo Principal

Desarrollar un sistema integral que permita capturar, almacenar, consultar, modificar y eliminar unidades de un catálogo maestro, para luego utilizar dicho catálogo como fuente de datos en la composición de listas de equipo personalizadas, respetando reglas de formación y equipamiento.

### 2.2 Alcance Funcional

- **Gestión de Catálogo Maestro**: Creación, lectura, actualización y eliminación (CRUD) de registros de unidades en una base de datos persistente basada en archivos binarios.
- **Gestión de Equipamiento**: Diferenciación entre equipamiento universal (accesible a cualquier facción) y equipamiento de facción (restringido a operativos de una facción específica). Asignación de un único equipamiento por operativo, con un límite máximo de cuatro equipamientos por lista de equipo completa.
- **Composición de Listas de Equipo**: Selección de operativos desde el catálogo maestro para conformar un roster jugable. Toda lista debe contener obligatoriamente un comandante (líder), complementado por especialistas y variantes de soldado.
- **Persistencia de Listas**: Almacenamiento de múltiples listas de equipo generadas por el usuario, identificables por nombre, fecha y hora de creación, o un identificador único.
- **Consulta y Reportes**: Visualización de listas almacenadas y exportación de una lista seleccionada a un archivo de texto plano (`.txt`).
- **Edición de Listas**: Modificación de una lista existente mediante la eliminación de un operativo y su sustitución por otro del catálogo maestro, manteniendo la vigencia de las reglas de composición.
- **Búsqueda Avanzada**: Filtrado de unidades en el catálogo maestro por nombre de facción (equipo), nombre de unidad o arma de rango.
- **Interfaz de Usuario**: Sistema de navegación por menús en consola con ayudas visuales que mejoren la legibilidad y usabilidad.

### 2.3 Alcance No Funcional

- Optimización del tamaño en memoria de las estructuras de datos para minimizar el consumo de bytes.
- Validación exhaustiva de entradas para prevenir comportamientos indefinidos ante datos incorrectos (caracteres alfabéticos donde se esperan numéricos, desbordamiento de límites, etc.).
- Registro de fecha y hora del sistema en el momento de creación de cada lista.
- Ordenamiento alfabético de unidades dentro de las listas o del catálogo.

### 2.4 Alcance Excluido

- El sistema no implementa la simulación de partidas, resolución de combate ni cálculo de daño.
- No se utiliza Programación Orientada a Objetos (POO).
- No se desarrolla una interfaz gráfica de usuario (GUI); la interacción es exclusivamente mediante línea de comandos (CLI).
- No se contempla conectividad de red ni bases de datos relacionales.

---

## 3. Arquitectura General del Sistema

El sistema se organiza en una arquitectura por capas de tipo procedural, donde la separación de responsabilidades se logra mediante la agrupación lógica de funciones en módulos temáticos. La comunicación entre capas se realiza a través de llamadas a funciones con paso de parámetros por valor y por referencia, utilizando estructuras de datos compuestas como contenedores de información.

### 3.1 Capas Arquitectónicas

**Capa de Presentación (UI)**
Responsable de la interacción con el usuario final. Gestiona el renderizado de menús jerárquicos, formularios de captura, tablas de visualización y mensajes de retroalimentación. Opera exclusivamente con datos primitivos y estructuras ya materializadas, sin acceso directo a la lógica de persistencia.

**Capa de Aplicación (Controladores)**
Actúa como intermediaria entre la presentación y el dominio. Coordina la secuencia de operaciones para cada caso de uso (por ejemplo, "armar lista" implica validar facción, seleccionar líder, seleccionar tropas, asignar equipo y guardar). Esta capa orquesta las llamadas a los validadores, gestores de lista y servicios de persistencia.

**Capa de Dominio (Reglas de Negocio)**
Contiene la lógica específica del juego y las restricciones del proyecto. Incluye el validador de composición de listas, el verificador de restricciones de equipamiento por facción, y las reglas que determinan si una lista es legal según la configuración requerida (presencia de comandante, límites de operativos, etc.).

**Capa de Persistencia**
Administra todas las operaciones de entrada/salida sobre el sistema de archivos. Se encarga de la serialización y deserialización de estructuras de datos hacia y desde archivos binarios, así como de la generación de archivos de texto para reportes. Implementa mecanismos para lectura secuencial, escritura apendicular y, como requisito extendido, edición directa en posiciones específicas del archivo mediante manejo de desplazamientos (offsets).

**Capa de Datos en Memoria**
Implementada mediante listas simplemente enlazadas abiertas que residen en el heap durante la ejecución. Cada nodo contiene una instancia de una estructura de dominio y un puntero al siguiente nodo. Esta capa proporciona la volatilidad necesaria para las operaciones de consulta y manipulación antes de la persistencia definitiva en disco.

### 3.2 Flujo de Control General

Al iniciar, el sistema carga el catálogo maestro desde el archivo binario hacia una lista enlazada en memoria. El usuario navega por los menús, ejecutando operaciones que modifican la lista en memoria y, eventualmente, disparan operaciones de escritura en disco. Las listas de equipo (rosters) se gestionan de manera similar, cargándose y guardándose en archivos binarios independientes. Al cerrar el programa, se libera la memoria dinámica asignada.

---

## 4. Componentes Principales

### 4.1 Base de Datos Maestra de Unidades

Repositorio central que contiene la totalidad de los operativos disponibles para la composición de listas. Cada registro representa una entidad única del juego (por ejemplo, _Assault Intercessor Sergeant_, _Kommando Boss Nob_, _Mandrake Nightfiend_) con sus atributos completos. Este componente es de solo lectura durante el armado de listas, pero de lectura/escritura durante la administración del catálogo.

### 4.2 Módulo de Equipamiento

Gestiona el catálogo de ítems disponibles para equipar a los operativos. Se divide conceptualmente en dos subconjuntos disjuntos:

- **Equipamiento Universal**: Items disponibles para cualquier facción (por ejemplo, barricadas, minas, granadas de humo).
- **Equipamiento de Facción**: Items exclusivos que solo pueden ser asignados a operativos pertenecientes a una facción determinada (por ejemplo, _Purity Seals_ para _Angels of Death_, _Choppas_ para _Kommandos_).

### 4.3 Módulo de Composición de Listas (Roster Builder)

Motor de construcción de ejércitos. Permite al usuario seleccionar una facción base y elegir operativos del catálogo maestro filtrados por dicha facción. Controla la inclusión obligatoria de un comandante y la distribución de especialistas y soldados. Aplica restricciones de cantidad (por ejemplo, no más de una copia de un especialista determinado, salvo excepciones explícitas como los _Warriors_).

### 4.4 Motor de Validaciones

Subsistema transversal que verifica la integridad de los datos en múltiples niveles:

- **Validación de entrada**: Tipo de dato, rango numérico, longitud de cadenas.
- **Validación de dominio**: Corrección de estadísticas, armas válidas para el operativo.
- **Validación de negocio**: Reglas de composición de lista, restricciones de equipamiento, límites de facción.

### 4.5 Sistema de Persistencia Binaria

Encargado de la durabilidad de la información. Opera con archivos binarios de acceso secuencial y, opcionalmente, aleatorio. Mantiene la integridad estructural entre lo almacenado en disco y lo cargado en memoria.

### 4.6 Generador de Reportes

Transforma las estructuras binarias en representaciones textuales legibles. Genera archivos `.txt` formateados que pueden consultarse fuera de la aplicación.

### 4.7 Interfaz de Consola (UI/UX Textual)

Sistema de presentación que utiliza caracteres ASCII para dibujar marcos, tablas, encabezados y separadores, facilitando la comprensión visual del estado del sistema y de los datos mostrados.

---

## 5. Flujo Completo del Sistema

### 5.1 Inicialización del Sistema

1. El programa verifica la existencia de los archivos binarios de catálogo maestro y de listas de equipo en el directorio de trabajo.
2. Si existen, procede a cargar su contenido en listas enlazadas en memoria mediante lectura secuencial.
3. Si no existen, inicializa listas vacías y prepara los archivos para su primera escritura.
4. Presenta el menú principal de navegación.

### 5.2 Flujo de Administración del Catálogo (CRUD de Unidades)

- **Alta**: El usuario selecciona la opción de captura. El sistema solicita atributos uno a uno (nombre, facción, tipo, estadísticas, armas). El motor de validaciones verifica cada entrada. Una vez completada la captura, la unidad se inserta en la lista enlazada del catálogo y se persiste en el archivo binario maestro.
- **Consulta**: El usuario accede al submenú de búsqueda, selecciona un criterio (nombre de facción, nombre de unidad o arma de rango) y proporciona el término de búsqueda. El sistema recorre la lista enlazada aplicando el filtro y presenta los resultados.
- **Edición**: Tras una búsqueda, el usuario selecciona una unidad. El sistema permite modificar sus campos. Si la unidad está referenciada en listas de equipo existentes, el sistema debe decidir si propaga los cambios o invalida las listas afectadas (esta decisión de diseño queda a criterio de implementación).
- **Eliminación**: El usuario selecciona una unidad para borrar. El sistema la remueve de la lista enlazada y reconstruye el archivo binario sin el registro eliminado.

### 5.3 Flujo de Armado de Lista de Equipo

1. El usuario inicia la creación de una nueva lista.
2. El sistema asigna automáticamente un identificador único y captura el timestamp del sistema.
3. El usuario selecciona la facción. El sistema filtra el catálogo maestro para mostrar solo operativos de esa facción.
4. El usuario debe seleccionar obligatoriamente un comandante (líder) de los disponibles.
5. El usuario selecciona especialistas y soldados hasta completar la lista, respetando los límites de facción inferidos de los documentos de reglas.
6. Para cada operativo añadido, el sistema permite asignar un equipamiento (universal o de facción). Se valida que el equipamiento sea compatible y que no se exceda el límite de cuatro equipamientos totales por lista.
7. Al finalizar, el sistema valida la integridad de la lista y la persiste en el archivo binario de rosters.

### 5.4 Flujo de Edición de Lista Existente

1. El usuario selecciona una lista previamente guardada (por nombre, ID o fecha).
2. El sistema carga la lista en memoria.
3. El usuario elige eliminar un operativo específico.
4. El sistema verifica que la eliminación no viole reglas mínimas (por ejemplo, no se puede eliminar el único comandante sin reemplazo).
5. El usuario selecciona un nuevo operativo del catálogo maestro como reemplazo.
6. El sistema revalida la composición completa y actualiza la persistencia.

### 5.5 Flujo de Exportación

1. El usuario selecciona una lista guardada.
2. El sistema formatea los datos en una representación textual estructurada.
3. Se genera un archivo `.txt` con el nombre de la lista o su ID, conteniendo el desglose de operativos, estadísticas, armas y equipamiento asignado.

---

## 6. Estructura Lógica del Proyecto

### 6.1 Entidades del Dominio

**Unidad (Operativo)**
Representa una figura individual del juego. Conceptualmente contiene:

- Identificador único.
- Denominación del operativo (por ejemplo, _Intercessor Sergeant_).
- Facción a la que pertenece (enumeración o categoría).
- Tipo de rol: Comandante, Especialista o Soldado (Warrior).
- Estadísticas numéricas del juego: APL (Action Point Limit), MOVE (desplazamiento), SAVE (salvación), WOUNDS (heridas).
- Conjunto de armas disponibles, diferenciadas entre armas de rango y armas de combate cuerpo a cuerpo. Cada arma tiene nombre, ataque, impacto, daño y reglas especiales.
- Indicadores de reglas especiales del operativo (habilidades, tácticas de capítulo, etc.).
- Campo para equipamiento asignado (referencia o nombre del ítem).

**Equipamiento (Item)**
Representa un objeto utilizable. Contiene:

- Identificador único.
- Nombre del equipamiento.
- Clasificación: Universal o Facción-específico.
- Facción asociada (en caso de ser restrictivo).
- Descripción funcional o efecto.

**Lista de Equipo (Roster)**
Representa una agrupación de operativos lista para jugar. Contiene:

- Identificador único de lista.
- Nombre descriptivo asignado por el usuario.
- Fecha y hora de creación (timestamp).
- Facción de la lista.
- Colección de referencias a operativos seleccionados desde el catálogo maestro.
- Registro de equipamientos asignados a cada operativo.

**Arma**
Sub-entidad vinculada a un operativo. Contiene:

- Nombre del arma.
- Tipo: Rango o Cuerpo a Cuerpo.
- Estadísticas de combate: ATK, HIT, DMG (daño normal/crítico).
- Reglas de arma especiales (por ejemplo, _Piercing_, _Brutal_, _Torrent_).

### 6.2 Relaciones entre Entidades

- Una **Unidad** pertenece a exactamente una **Facción**.
- Una **Unidad** posee una o más **Armas**.
- Una **Lista de Equipo** está compuesta por múltiples **Unidades** (generalmente entre 1 y 10, dependiendo de la facción).
- Una **Unidad** dentro de una **Lista** puede tener asignado cero o un **Equipamiento**.
- Una **Lista de Equipo** puede contener hasta cuatro **Equipamientos** en total, distribuidos entre sus operativos.
- Un **Equipamiento** puede ser utilizado por cualquier facción (universal) o estar restringido a una facción específica.

### 6.3 Ambigüedades en el Modelo de Datos

- **Reglas de Composición Específicas vs. Regla Genérica**: Los documentos de facción presentan reglas de selección muy específicas (por ejemplo, _Angels of Death_ permite 1 líder de entre tres opciones específicas y 5 operativos de una lista cerrada). El proyecto, sin embargo, enuncia una regla generalizada: "todos los equipos tienen un comandante y el resto son especialistas, y alguna variante de soldado". Esto representa una ambigüedad de diseño: el desarrollador debe decidir si implementa las restricciones exactas por facción (como aparecen en los datacards) o una regla genérica simplificada. La documentación recomienda implementar las reglas específicas por facción para mantener fidelidad al dominio, pero priorizando la estructura de comandante/especialista/soldado como marco base.
- **Definición de "Unidad Completa"**: El documento del proyecto no especifica exhaustivamente qué campos deben capturarse para una unidad. Se infiere que debe incluirse toda la información relevante de los datacards (estadísticas, armas, reglas especiales), pero el nivel de granularidad (por ejemplo, si se almacenan todas las opciones de armamento alternativas de un operativo o solo una configuración fija) no está definido.

---

## 7. Servicios, Módulos y Responsabilidades

### 7.1 Módulo de Gestión de Catálogo Maestro

**Responsabilidades**:

- Insertar nuevas unidades en la lista enlazada del catálogo.
- Eliminar unidades del catálogo, gestionando la correcta liberación de memoria y reconexión de nodos.
- Actualizar campos de una unidad existente.
- Buscar unidades por identificador, nombre o facción.
- Sincronizar el estado de la lista enlazada con el archivo binario maestro.

### 7.2 Módulo de Gestión de Equipamiento

**Responsabilidades**:

- Mantener el catálogo de equipamiento disponible.
- Validar la elegibilidad de un equipamiento para un operativo dado (verificar restricción de facción).
- Controlar el conteo total de equipamientos asignados en una lista (límite de 4).
- Asociar y desasociar equipamiento a operativos dentro de una lista.

### 7.3 Módulo de Composición de Listas

**Responsabilidades**:

- Inicializar nuevas listas con metadatos (ID, nombre, timestamp).
- Gestionar la selección de operativos desde el catálogo maestro hacia una lista activa.
- Asegurar la presencia de exactamente un comandante por lista.
- Validar que los operativos seleccionados pertenezcan a la misma facción declarada para la lista.
- Verificar límites de cantidad por tipo de operativo según las reglas de facción.
- Calcular y mostrar el estado actual de la lista (contadores de operativos, puntos o capacidad utilizada).

### 7.4 Módulo de Persistencia

**Responsabilidades**:

- Abrir archivos binarios en modos de lectura, escritura y actualización.
- Realizar la serialización de estructuras completas hacia el flujo de archivo.
- Realizar la deserialización desde el archivo hacia instancias de estructuras en memoria.
- Implementar la lógica de edición directa en archivo mediante cálculo de offsets basado en el tamaño fijo de los registros.
- Manejar la reconstrucción de archivos cuando un registro es eliminado (creación de archivo temporal y reemplazo).
- Generar archivos de texto plano con formato legible para exportación de listas.

### 7.5 Módulo de Validación de Entradas

**Responsabilidades**:

- Leer datos desde la entrada estándar de forma segura.
- Verificar que los valores numéricos ingresados estén dentro de rangos permitidos (por ejemplo, heridas entre 1 y 40, APL entre 1 y 4).
- Prevenir el desbordamiento de buffers en la lectura de cadenas.
- Limpiar el buffer de entrada cuando se detectan caracteres no esperados.
- Confirmar operaciones destructivas (eliminaciones) con el usuario antes de ejecutarlas.

### 7.6 Módulo de Utilidades y Ayudas Visuales

**Responsabilidades**:

- Dibujar marcos y separadores en consola para delimitar secciones de información.
- Formatear tablas de datos alineadas (por ejemplo, listado de unidades con columnas para nombre, facción, APL, heridas).
- Mostrar mensajes de error, advertencia y éxito con indicadores visuales claros.
- Implementar la lógica de pausa y limpieza de pantalla entre menús.

### 7.7 Módulo de Ordenamiento y Búsqueda

**Responsabilidades**:

- Implementar algoritmos de ordenamiento (por ejemplo, ordenamiento por inserción o quicksort adaptado) para ordenar las listas enlazadas alfabéticamente por nombre de unidad.
- Implementar búsqueda secuencial sobre listas enlazadas aplicando uno o múltiples criterios.
- Soportar búsquedas parciales (subcadenas) en campos de texto.

---

## 8. Dependencias y Tecnologías Utilizadas

### 8.1 Lenguaje y Paradigma

- **Lenguaje C**: Se utiliza el estándar de C (preferentemente C99) bajo un paradigma procedural estructurado.
- **Paradigma Procedural**: Toda la lógica se encapsula en funciones. No se utilizan clases ni objetos.

### 8.2 Bibliotecas Estándar

- **stdio.h**: Operaciones de entrada/salida, manejo de archivos (texto y binario).
- **stdlib.h**: Gestión de memoria dinámica, conversiones numéricas, utilidades generales.
- **string.h**: Manipulación de cadenas de caracteres (copia, concatenación, comparación).
- **time.h**: Captura del tiempo del sistema para la generación de timestamps en las listas de equipo.
- **ctype.h**: Funciones de clasificación de caracteres para validaciones de entrada.

### 8.3 Infraestructura

- **Sistema Operativo**: El sistema es portable entre sistemas operativos de tipo POSIX y Windows, siempre que se respeten las convenciones de manejo de archivos binarios del compilador objetivo.
- **Sistema de Archivos**: Dependencia directa del sistema de archivos local para la creación, lectura y escritura de archivos `.dat` (binarios) y `.txt` (reportes).
- **Compilador**: Requiere un compilador de C compatible (GCC, Clang, MSVC) configurado para la arquitectura objetivo (32 o 64 bits), lo cual afecta directamente al tamaño de los punteros y, consecuentemente, al diseño de las estructuras optimizadas.

---

## 9. Procesos Internos y Funcionamiento

### 9.1 Proceso de Captura y Alta de Unidad

El sistema presenta un formulario secuencial donde el usuario ingresa cada atributo de la unidad. Cada campo es validado inmediatamente. Una vez completados todos los campos obligatorios, se reserva memoria dinámica para un nuevo nodo de lista enlazada, se pobla con los datos capturados, y se inserta al final de la lista del catálogo. Posteriormente, el sistema abre el archivo binario maestro en modo apéndice y escribe el nuevo registro al final del archivo.

### 9.2 Proceso de Asignación de Equipamiento

Durante el armado o edición de una lista, el sistema presenta al usuario el catálogo de equipamiento filtrado: si el operativo es de facción X, se muestran los items universales más los items exclusivos de la facción X. El usuario selecciona un item. El sistema verifica que el operativo no tenga ya un equipamiento asignado y que el conteo total de la lista no supere cuatro. Si las validaciones pasan, se establece la relación de asignación.

### 9.3 Proceso de Validación de Composición de Lista

Antes de permitir guardar una lista, el motor de validaciones ejecuta una serie de comprobaciones:

- Existencia de exactamente un operativo marcado como Comandante.
- Todos los operativos pertenecen a la facción declarada de la lista.
- No se exceden los límites de cantidad por tipo de operativo (por ejemplo, máximo un _Eliminator Sniper_, máximo un _Death Jester_, salvo excepciones como _Warriors_ que pueden repetirse según los documentos de facción).
- El número total de equipamientos no excede cuatro.

### 9.4 Proceso de Edición Directa en Archivos Binarios

Como funcionalidad extendida, el sistema puede permitir modificar un registro directamente en el archivo sin reconstruir la lista completa. Este proceso implica:

1. Calcular el desplazamiento (offset) del registro objetivo multiplicando el índice del registro por el tamaño en bytes de la estructura (asumiendo registros de tamaño fijo).
2. Posicionar el puntero de archivo en dicho offset mediante funciones de búsqueda de posición.
3. Sobrescribir los bytes correspondientes con la nueva versión del registro.
   Este enfoque requiere que el tamaño de la estructura sea constante y conocido, y que la modificación no altere la longitud del registro.

### 9.5 Proceso de Ordenamiento Alfabético

El sistema implementa un algoritmo de ordenamiento sobre la lista enlazada simple. Dado que no se dispone de acceso aleatorio eficiente en una lista enlazada, se recomienda el uso de ordenamiento por inserción o el método de intercambio de punteros (reordenamiento de nodos) en lugar de intercambio de datos, para minimizar la copia de estructuras potencialmente grandes. El criterio de ordenamiento es la comparación lexicográfica del nombre de la unidad.

### 9.6 Proceso de Generación de Timestamp

Al crear una nueva lista de equipo, el sistema consulta la hora del sistema operativo mediante las utilidades de tiempo disponibles en la biblioteca estándar. Este valor se convierte a una estructura de fecha/hora legible (día, mes, año, hora, minuto) y se almacena como parte de los metadatos de la lista, sirviendo como criterio de búsqueda y auditoría.

### 9.7 Proceso de Exportación a Texto

El sistema recorre la lista enlazada de una lista de equipo seleccionada. Para cada operativo, formatea sus campos en líneas de texto, incluyendo nombre, estadísticas, desglose de armas y equipamiento asignado. El resultado se escribe en un archivo de texto plano con codificación estándar, utilizando saltos de línea y tabulaciones para la legibilidad.

---

## 10. Integraciones Externas

El sistema es autocontenido y no se comunica con servicios de red ni APIs externas. Sus únicas integraciones son con los recursos del sistema operativo anfitrión:

### 10.1 Sistema de Archivos

Integración de lectura y escritura sobre el almacenamiento local. El programa depende de permisos de archivo adecuados para crear, leer, modificar y eliminar archivos binarios y de texto en el directorio de ejecución o en rutas relativas predefinidas.

### 10.2 Reloj del Sistema

Integración con el reloj de hardware del equipo a través de la biblioteca estándar de tiempo. Utilizado exclusivamente para la adquisición del timestamp de creación de las listas de equipo. No se requiere sincronización de red ni zonas horarias complejas; se utiliza la hora local del sistema.

---

## 11. Flujos de Datos y Comunicación

### 11.1 Flujo de Lectura (Disco a Memoria)

Los archivos binarios actúan como origen de verdad. Al iniciar, el flujo de datos se describe como:
`Archivo Binario -&gt; Buffer de Lectura -&gt; Estructura Temporal -&gt; Nodo de Lista Enlazada -&gt; Lista en Memoria (Heap)`
Este flujo se ejecuta para el catálogo maestro y, bajo demanda, para las listas de equipo guardadas.

### 11.2 Flujo de Escritura (Memoria a Disco)

Cuando el usuario confirma una operación de guardado:
`Lista en Memoria -&gt; Nodo Actual -&gt; Estructura de Datos -&gt; Buffer de Escritura -&gt; Archivo Binario`
Para altas, el archivo se abre en modo apéndice. Para modificaciones globales (como eliminaciones), puede requerirse un flujo de reconstrucción: `Lista en Memoria -&gt; Archivo Temporal -&gt; Reemplazo de Archivo Original`.

### 11.3 Flujo de Exportación (Memoria a Texto)

`Lista en Memoria -&gt; Formateador de Texto -&gt; Archivo .txt`
Este flujo es unidireccional y genera un artefacto legible por humanos y procesable por editores de texto externos.

### 11.4 Comunicación Inter-Modular

La comunicación entre los módulos del sistema se realiza exclusivamente mediante:

- **Paso de parámetros**: Estructuras completas (por valor, cuando son pequeñas) o punteros a estructuras (por referencia, para evitar copias costosas y permitir modificaciones).
- **Retorno de valores**: Códigos de estado numéricos que indican éxito o tipo de error, permitiendo al módulo llamador tomar decisiones de presentación.
- **Variables globales (restringidas)**: Se desaconseja el uso extensivo de variables globales; sin embargo, punteros a las cabeceras de las listas enlazadas principales pueden declararse en un ámbito accesible para simplificar la gestión del estado central del programa.

---

## 12. Consideraciones Técnicas Importantes

### 12.1 Optimización de Estructuras para Minimizar Bytes

Un requisito explícito del proyecto es que las estructuras de datos ocupen la menor cantidad de bytes posible. En el lenguaje C, el compilador introduce relleno (padding) entre los miembros de una estructura para alinearlos en memoria según sus requisitos de direccionamiento. Para minimizar el tamaño total, los campos deben declararse en orden descendente de tamaño (por ejemplo: punteros o enteros primero, luego shorts, luego chars), o agruparse de forma que se eliminen huecos innecesarios. Esta decisión de diseño impacta directamente en:

- La cantidad de memoria RAM consumida por la lista enlazada.
- El tamaño del archivo binario en disco.
- La posibilidad de realizar cálculos de offset precisos para la edición directa en archivo.

### 12.2 Manejo de Archivos Binarios y Portabilidad

Los archivos binarios generados por el sistema no son inherentemente portables entre arquitecturas de hardware o compiladores diferentes, debido a:

- Diferencias en el tamaño de tipos primitivos (por ejemplo, `int` de 16, 32 o 64 bits).
- Diferencias de endianness (orden de bytes).
- Alineamiento interno de estructuras determinado por el compilador.
  Se recomienda documentar explícitamente la arquitectura objetivo y, si es posible, utilizar tipos de tamaño fijo (como los definidos en `stdint.h`) para aumentar la previsibilidad del formato binario.

### 12.3 Edición Directa vs. Reconstrucción de Archivos

El requisito extendido de edición directa en archivos binarios introduce complejidad significativa. Para que sea viable:

- Los registros deben tener un tamaño fijo y conocido.
- No debe utilizarse asignación dinámica de memoria dentro de las estructuras que se serializan (por ejemplo, punteros a cadenas deben reemplazarse por arreglos de caracteres de tamaño fijo).
- Las operaciones de eliminación requieren una estrategia de marcado de registros como "eliminados" (borrado lógico) o la reconstrucción completa del archivo (borrado físico). El borrado lógico es más eficiente para archivos grandes pero requiere mecanismos de compactación periódica.

### 12.4 Gestión de Memoria Dinámica

El uso de listas simplemente enlazadas implica la reserva de memoria en el heap para cada nodo. Es imperativo que cada operación de inserción tenga su correspondiente liberación de memoria al eliminar un nodo o al cerrar el programa. La pérdida de referencias a nodos (memory leaks) o el acceso a memoria ya liberada (dangling pointers) son riesgos críticos que deben mitigarse mediante disciplina en el manejo de punteros.

### 12.5 Manejo de Cadenas de Caracteres

Dado el entorno de C, las cadenas se manejan como arreglos de caracteres terminados en nulo. En el contexto de archivos binarios, las cadenas deben almacenarse como arreglos de tamaño fijo dentro de las estructuras para garantizar que todos los registros ocupen el mismo espacio. Esto impone un límite máximo en la longitud de nombres y descripciones, que debe ser documentado y validado durante la captura.

---

## 13. Configuración Conceptual del Entorno

### 13.1 Organización del Código Fuente

Se propone una separación física del código en archivos fuente (`.c`) y archivos de encabezado (`.h`), agrupados por responsabilidad:

- Archivos de definición de estructuras y constantes globales.
- Archivos de lógica de persistencia.
- Archivos de lógica de listas enlazadas.
- Archivos de lógica de dominio (reglas de Kill Team).
- Archivos de interfaz de usuario y utilidades de consola.
- Archivo principal de entrada al programa.

### 13.2 Convenciones de Nomenclatura

- **Estructuras**: Nombres descriptivos en mayúsculas o notación PascalCase.
- **Funciones**: Prefijo por módulo seguido de verbo y objeto (por ejemplo, `catalogo_agregar_unidad`, `lista_imprimir`, `validar_entero_rango`).
- **Constantes**: Mayúsculas con guiones bajos para tamaños máximos de buffers, límites de equipo y códigos de error.
- **Variables**: Notación camelCase o snake_case, consistente en todo el proyecto.

### 13.3 Archivos de Datos

- **Catálogo Maestro**: Un archivo binario dedicado (`unidades.dat` o similar) que almacena el registro completo de cada operativo.
- **Listas de Equipo**: Un archivo binario dedicado (`listas.dat` o similar) que almacena los metadatos de cada lista y, potencialmente, los identificadores de los operativos que la componen, o bien registros embebidos de los operativos seleccionados.
- **Reportes Exportados**: Archivos de texto generados bajo demanda con extensión `.txt` y nombres derivados del identificador o nombre de la lista.

---

## 14. Buenas Prácticas y Lineamientos

### 14.1 Modularidad y Cohesión

Cada función debe tener una única responsabilidad bien definida. Las funciones de captura no deben realizar validaciones complejas de dominio (deben delegar al validador), y las funciones de dominio no deben interactuar directamente con el usuario.

### 14.2 Separación de Concerns

La capa de presentación no debe contener lógica de negocio. La capa de persistencia no debe realizar validaciones de reglas de juego. La capa de dominio no debe depender de detalles de implementación de archivos.

### 14.3 Manejo de Errores

Todas las funciones que interactúan con el sistema de archivos o con asignación de memoria deben retornar códigos de estado que permitan al llamador detectar fallos (apertura fallida, escritura incompleta, memoria insuficiente) y presentar mensajes apropiados al usuario.

### 14.4 Limpieza de Recursos

Todo bloque de memoria reservado con funciones de asignación dinámica debe tener un camino explícito de liberación. Los archivos abiertos deben cerrarse en el mismo nivel donde se abrieron, preferentemente en bloques de código opuestos.

### 14.5 Validaciones Defensivas

Nunca se debe asumir que la entrada del usuario es correcta. Todas las lecturas deben incluir verificación de límites, comprobación de tipos y saneamiento de buffers. Las operaciones destructivas (borrado de unidades o listas) deben requerir confirmación explícita.

### 14.6 Comentarios y Documentación Interna

Siguiendo la preferencia por comentarios mínimos y esenciales, el código debe ser autoexplicativo mediante nombres descriptivos. Los comentarios deben reservarse para explicar decisiones de diseño no obvias, algoritmos complejos o advertencias sobre efectos secundarios de funciones.

---

## 15. Riesgos, Limitaciones y Consideraciones

### 15.1 Fragilidad del Formato Binario

Cualquier cambio en la definición de una estructura (agregar, eliminar o reordenar campos) invalida los archivos binarios existentes. Es necesario establecer una versión de formato de archivo o aceptar que los archivos generados en una versión del programa pueden no ser compatibles con otra.

### 15.2 Escalabilidad Limitada por Estructuras de Datos Simples

Las listas simplemente enlazadas y la búsqueda secuencial ofrecen un rendimiento de orden lineal O(n) para todas las operaciones de consulta. Si bien esto es aceptable para el tamaño reducido de un catálogo de _Kill Team_ (decenas de unidades), no escalaría a colecciones masivas sin una reestructuración profunda.

### 15.3 Complejidad de la Edición In-Situ

La edición directa de registros en archivos binarios es técnicamente compleja y propensa a errores de corrupción si el tamaño del registro varía o si se producen interrupciones durante la escritura. Se recomienda implementar mecanismos de respaldo o transacciones simples (escritura en archivo temporal seguida de renombrado atómico).

### 15.4 Ambigüedades en los Requisitos de Dominio

Se identifican las siguientes ambigüedades que el desarrollador debe resolver durante el diseño detallado:

- **Reglas de Composición**: El proyecto enuncia una regla general de "1 comandante + especialistas + soldados", mientras que los documentos de facción adjuntos especifican listas cerradas con restricciones precisas por facción (cantidades exactas, opciones de líder limitadas, exclusividad de especialistas). El sistema debe decidir si implementa las reglas reales del juego o la simplificación del proyecto.
- **Campo "Nombre de Equipo" en Búsquedas**: El criterio de búsqueda "Nombre de equipo" puede interpretarse como el nombre de la facción (por ejemplo, "Kommandos") o como el nombre personalizado de una lista de equipo creada por el usuario. La documentación recomienda tratar "Nombre de equipo" como nombre de facción cuando se busca en el catálogo maestro, y como nombre de roster cuando se busca en listas guardadas.
- **Definición de "Unidad Completa"**: El proyecto no detalla explícitamente todos los campos que debe contener el registro de una unidad. Se infiere que deben incluirse atributos derivados de los datacards (estadísticas, armas, reglas), pero la granularidad y opcionalidad de cada campo no están definidas.
- **Límite de Equipamiento**: La restricción de "máximo 4 equipamientos por lista" y "1 por personaje" es una regla impuesta por el proyecto que puede no reflejar fielmente todas las combinaciones posibles dentro de las reglas oficiales de _Kill Team_.

### 15.5 Dependencia del Conocimiento del Dominio

El correcto funcionamiento del sistema depende de que el desarrollador o el usuario administrador comprendan las reglas de _Kill Team_ para poblar inicialmente el catálogo maestro con datos coherentes. El sistema valida la estructura, pero la semántica del juego (qué armas son válidas para qué operativo) depende de la correcta carga inicial de datos.

---

## 16. Glosario de Términos Técnicos

- **Archivo Binario**: Archivo que almacena datos en formato no textual, preservando la representación en memoria de las estructuras de datos para lectura y escritura directa.
- **APL (Action Point Limit)**: Estadística del juego que determina la cantidad de acciones que un operativo puede realizar por activación.
- **Arquetipo**: Clasificación de una lista de equipo según su estilo de juego (por ejemplo, Infiltración, Reconocimiento, Seguridad), utilizado en ciertos paquetes de misión.
- **Borrado Lógico**: Técnica de eliminación de registros donde se marca un registro como inactivo en lugar de removerlo físicamente del archivo.
- **Buffer**: Zona de memoria temporal utilizada para almacenar datos en tránsito entre la entrada/salida y las estructuras del programa.
- **Catálogo Maestro**: Base de datos central que contiene todas las unidades disponibles para la composición de listas.
- **Datacard**: Hoja de referencia que contiene las estadísticas, armas y reglas especiales de un operativo en el juego _Kill Team_.
- **EDD**: Estructura de Datos (contexto académico del proyecto).
- **Equipamiento de Facción**: Items exclusivos que solo pueden ser utilizados por operativos de una facción específica.
- **Equipamiento Universal**: Items disponibles para ser utilizados por operativos de cualquier facción.
- **Facción**: Grupo o raza del juego (por ejemplo, Space Marines, Orks, Drukhari, Harlequins).
- **Heap**: Región de memoria dinámica donde se reservan estructuras de datos en tiempo de ejecución.
- **Lista Enlazada Simple Abierta**: Estructura de datos lineal donde cada elemento (nodo) contiene un dato y un puntero al siguiente elemento; el último nodo apunta a NULL. No utiliza nodo centinela.
- **Offset**: Desplazamiento en bytes desde el inicio de un archivo hasta una posición específica de un registro.
- **Operativo (Unidad)**: Figura individual controlada por el jugador en _Kill Team_.
- **Padding (Relleno)**: Bytes insertados automáticamente por el compilador entre los miembros de una estructura para cumplir con requisitos de alineación de memoria.
- **POO**: Programación Orientada a Objetos. Paradigma explícitamente excluido del proyecto.
- **Roster (Lista de Equipo)**: Conjunto de operativos seleccionados por un jugador para una partida, cumpliendo las reglas de composición.
- **Serialización**: Proceso de convertir una estructura de datos en memoria en una secuencia de bytes para su almacenamiento o transmisión.
- **Timestamp**: Marca temporal que registra la fecha y hora exacta de un evento (en este caso, la creación de una lista).
- **Warrior**: Tipo de operativo básico en _Kill Team_, generalmente permitido en múltiples copias dentro de una lista, a diferencia de especialistas únicos.

---

## 17. Resumen Final y Entendimiento Global del Proyecto

El sistema de gestión de listas de _Kill Team_ es una aplicación de consola desarrollada en lenguaje C que digitaliza el proceso de catalogación de miniaturas y composición de ejércitos para el juego de escaramuzas de Games Workshop. Su propósito académico es demostrar el dominio de estructuras de datos fundamentales (estructuras compuestas y listas simplemente enlazadas), técnicas de persistencia avanzada (archivos binarios y edición directa mediante offsets) y principios de programación modular procedural.

El sistema se articula en torno a dos grandes subsistemas: el **Catálogo Maestro de Unidades**, que actúa como una base de datos de referencia de todos los operativos disponibles, y el **Constructor de Listas de Equipo**, que permite a los usuarios ensamblar rosters jugables seleccionando operativos del catálogo bajo restricciones de facción, roles (comandante, especialista, soldado) y equipamiento. La persistencia se logra mediante archivos binarios que reflejan fielmente el layout en memoria de las estructuras, con énfasis en la optimización del espacio mediante la eliminación de padding.

La arquitectura es explícitamente procedural y modular, dividida en capas de presentación, aplicación, dominio y persistencia. La comunicación entre estas capas se realiza mediante funciones con parámetros estrictamente tipados, evitando el uso de Programación Orientada a Objetos. El sistema incorpora validaciones defensivas exhaustivas para garantizar la robustez ante entradas erróneas, y utiliza utilidades de tiempo del sistema para enriquecer los metadatos de las listas creadas.

El proyecto presenta desafíos técnicos notables, incluyendo la gestión de memoria dinámica, la portabilidad de archivos binarios entre arquitecturas, la implementación de búsquedas y ordenamiento sobre listas enlazadas, y la edición directa de registros en archivos mediante cálculo de posiciones. Asimismo, existen ambigüedades conceptuales derivadas de la simplificación de las reglas complejas del juego _Kill Team_, las cuales deben ser abordadas mediante decisiones de diseño documentadas durante la implementación.

En esencia, el sistema busca ser una herramienta funcional y didáctica que bridge entre la teoría de estructuras de datos y su aplicación práctica en un dominio real y complejo, proporcionando una base sólida para el onboarding de desarrolladores en proyectos de sistemas de información con persistencia binaria y gestión de memoria manual.
