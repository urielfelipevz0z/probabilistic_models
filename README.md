# Modelos Probabilistas en C

Este proyecto implementa algoritmos de modelos probabilistas en lenguaje C, incluyendo **Redes Bayesianas** y **Modelos Ocultos de Markov (HMM)** con el algoritmo de Viterbi para predicción del clima.

## Características Principales

### ✅ Redes Bayesianas
- Construcción interactiva de redes bayesianas
- Definición de variables, conexiones y tablas de probabilidad
- Visualización de grafos en formato DOT
- Gestión de memoria dinámica

### ✅ Modelos Ocultos de Markov (HMM)
- **Implementación completa del algoritmo de Viterbi**
- **Predicción del clima basada en observaciones de comportamiento**
- **Dos modos de ejecución:**
  - **Modo básico**: Resultados finales únicamente
  - **Modo detallado**: Cálculos paso a paso con matrices completas
- **Validación robusta de datos y manejo de errores**
- **Memoria dinámica con liberación automática**

## Estructura del Proyecto

```
modelos-probabilistas/
├── src/
│   ├── main.c              # Programa principal con menú interactivo
│   ├── bayesian.h          # Definiciones para redes bayesianas
│   ├── bayesian.c          # Implementación de redes bayesianas
│   ├── hmm.h              # Definiciones para HMM y Viterbi
│   ├── hmm.c              # Implementación completa del algoritmo de Viterbi
│   ├── test_hmm_basic.c   # Test independiente modo básico
│   └── test_hmm_detailed.c # Test independiente modo detallado
├── clima_ejemplo.txt       # Archivo de datos para HMM
├── Makefile               # Sistema de compilación
└── README.md              # Esta documentación
```

## Algoritmo de Viterbi - Detalles Técnicos

### Modelo del Problema: Predicción del Clima

**Estados Ocultos:**
- `SUNNY(0)` - Soleado
- `CLOUDY(1)` - Nublado  
- `RAINY(2)` - Lluvioso

**Observaciones:**
- `UMBRELLA(0)` - Lleva paraguas
- `SUNGLASSES(1)` - Lleva gafas de sol
- `STAY_HOME(2)` - Se queda en casa

### Fases del Algoritmo

#### 1. **Inicialización (t=1)**
```c
δ₁(i) = π(i) × B(i, o₁)
ψ₁(i) = 0
```

#### 2. **Recursión (t=2 to T)**
```c
δₜ(i) = max[δₜ₋₁(j) × A(j,i)] × B(i,oₜ)
ψₜ(i) = argmax[δₜ₋₁(j) × A(j,i)]
```

#### 3. **Terminación**
```c
P* = max[δₜ(i)]
q*ₜ = argmax[δₜ(i)]
```

#### 4. **Backtracking**
```c
q*ₜ = ψₜ₊₁(q*ₜ₊₁) para t = T-1 hasta 1
```

### Matrices del Modelo

**Matriz de Transición A (3×3):**
```
        SUNNY   CLOUDY  RAINY
SUNNY   0.7     0.2     0.1
CLOUDY  0.3     0.4     0.3
RAINY   0.2     0.3     0.5
```

**Matriz de Emisión B (3×3):**
```
        UMBRELLA SUNGLASSES STAY_HOME
SUNNY   0.1      0.8        0.1
CLOUDY  0.3      0.4        0.3
RAINY   0.8      0.1        0.1
```

**Probabilidades Iniciales π:**
```
π = [0.6, 0.3, 0.1]  # [SUNNY, CLOUDY, RAINY]
```

**Secuencia de Observaciones:**
```
[SUNGLASSES, SUNGLASSES, UMBRELLA, STAY_HOME, UMBRELLA, UMBRELLA, SUNGLASSES]
```

## Compilación y Ejecución

### Prerrequisitos
- Compilador GCC
- Make
- Sistema operativo Linux/Unix

### Compilar el Proyecto

```bash
# Compilar programa principal
make all

# Compilar programa principal y tests
make all-tests

# Limpiar archivos compilados
make clean
```

### Ejecutar el Programa

#### Programa Principal (Interactivo)
```bash
make run
```

#### Tests Independientes
```bash
# Test modo básico de HMM
make run-test-basic

# Test modo detallado de HMM  
make run-test-detailed

# Ejecutar todos los tests
make run-all-tests
```

### Ejemplo de Salida - Modo Detallado

```
VITERBI ALGORITHM - WEATHER PREDICTION
=====================================

INPUT PARAMETERS:
States: SUNNY(0), CLOUDY(1), RAINY(2)
Observations: UMBRELLA(0), SUNGLASSES(1), STAY_HOME(2)
Sequence: [SUNGLASSES, SUNGLASSES, UMBRELLA, STAY_HOME, UMBRELLA, UMBRELLA, SUNGLASSES]

TRANSITION MATRIX A:
        SUNNY   CLOUDY  RAINY
SUNNY   0.700   0.200   0.100
CLOUDY  0.300   0.400   0.300
RAINY   0.200   0.300   0.500

EMISSION MATRIX B:
        UMBRELLA SUNGLASSES STAY_HOME
SUNNY   0.100    0.800      0.100
CLOUDY  0.300    0.400      0.300
RAINY   0.800    0.100      0.100

INITIALIZATION (t=1, obs=SUNGLASSES):
δ₁(SUNNY)  = π(SUNNY)  × B(SUNNY,SUNGLASSES)  = 0.600 × 0.800 = 0.480
δ₁(CLOUDY) = π(CLOUDY) × B(CLOUDY,SUNGLASSES) = 0.300 × 0.400 = 0.120
δ₁(RAINY)  = π(RAINY)  × B(RAINY,SUNGLASSES)  = 0.100 × 0.100 = 0.010

[... cálculos detallados paso a paso ...]

FINAL RESULTS:
Optimal state sequence: [0, 0, 1, 1, 2, 2, 0]
Translation: [SUNNY, SUNNY, CLOUDY, CLOUDY, RAINY, RAINY, SUNNY]
Maximum probability: 0.0000297271
```

## Características Técnicas

### Gestión de Memoria
- Allocación dinámica con `malloc()`/`free()`
- Validación de todas las asignaciones de memoria
- Liberación automática en orden inverso
- Manejo robusto de fallos de memoria

### Validación de Datos
- ✅ Verificación de dimensiones de matrices
- ✅ Validación de probabilidades [0,1]
- ✅ Verificación de que las filas sumen 1.0
- ✅ Validación de secuencias de observación
- ✅ Manejo de errores de archivo

### Estándares de Código
- ✅ Estándar C99 con flags `-Wall -Wextra`
- ✅ Comentarios exhaustivos con fórmulas matemáticas
- ✅ Nomenclatura según literatura académica (δ, ψ, π)
- ✅ Modularidad con separación clara de responsabilidades
- ✅ Funciones de debugging y verificación

## Archivos de Configuración

### `clima_ejemplo.txt`
```
3                    # N: Número de estados
3                    # M: Número de observaciones
7                    # T: Longitud de secuencia
0.7 0.2 0.1         # Matriz de transición A fila 1
0.3 0.4 0.3         # Matriz de transición A fila 2
0.2 0.3 0.5         # Matriz de transición A fila 3
0.1 0.8 0.1         # Matriz de emisión B fila 1
0.3 0.4 0.3         # Matriz de emisión B fila 2
0.8 0.1 0.1         # Matriz de emisión B fila 3
0.6 0.3 0.1         # Probabilidades iniciales π
1 1 0 2 0 0 1       # Secuencia de observaciones
```

## Comandos Make Disponibles

```bash
make help                 # Mostrar ayuda
make all                  # Compilar programa principal
make all-tests           # Compilar programa y tests
make main                # Compilar solo programa principal
make test-hmm-basic      # Compilar test básico
make test-hmm-detailed   # Compilar test detallado
make clean               # Limpiar archivos compilados
make run                 # Ejecutar programa principal
make run-test-basic      # Ejecutar test básico de HMM
make run-test-detailed   # Ejecutar test detallado de HMM
make run-all-tests       # Ejecutar todos los tests
```

## Resultados Esperados

La implementación del algoritmo de Viterbi produce los siguientes resultados para el ejemplo del clima:

- **Secuencia óptima de estados**: [SUNNY, SUNNY, CLOUDY, CLOUDY, RAINY, RAINY, SUNNY]
- **Probabilidad máxima**: 0.0000297271
- **Interpretación**: El algoritmo encuentra la secuencia más probable de estados del clima que explica las observaciones de comportamiento

## Autor

Implementación completa del algoritmo de Viterbi para Modelos Ocultos de Markov con predicción del clima, siguiendo estándares académicos y buenas prácticas de programación en C.

---

**Nota**: Esta implementación está diseñada para propósitos educativos y de investigación, demostrando una implementación profesional y completa del algoritmo de Viterbi en C.
├── docs/                  # Documentación
├── tests/                 # Pruebas unitarias
└── scripts/               # Scripts de utilidad
```

## Compilación

Para compilar los proyectos en C:

```bash
gcc -o proyecto1 proyecto1.c -lm
```

## Uso

Describe aquí cómo usar tus modelos probabilistas.

## Contribuciones

Las contribuciones son bienvenidas. Por favor, sigue las mejores prácticas de programación en C.

## Licencia

[Especifica la licencia aquí]
