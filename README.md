# Proyectos de Física Computacional

Este repositorio alberga proyectos de física computacional desarrollados para presentaciones en congresos y eventos científicos.

## Proyecto: Simulación de Atenuación Gamma (GammaAtenuation)

### Descripción
Simulación Monte Carlo desarrollada con GEANT4 para estudiar la atenuación de rayos gamma en diferentes materiales biológicos. El proyecto incluye análisis comparativo de coeficientes de atenuación y visualizaciones interactivas con ROOT.

### Características Principales
- Simulación Monte Carlo con GEANT4 v11.3.2
- Análisis de atenuación para materiales biológicos (agua, músculo, hueso)
- Integración con ROOT v6.36.04 para análisis de datos
- Gráficas comparativas

### Estructura del Proyecto
```
GammaAtenuation/
├── src/                    # Código fuente GEANT4
├── include/               # Archivos de cabecera
├── build/                 # Archivos compilados y scripts de análisis
├── mac/                   # Archivos de macros de configuración
└── scripts/              # Scripts auxiliares de análisis
```

### Resultados Validados
- Agua (tejido blando): 84.4% transmisión, μ = 0.0339 cm⁻¹
- Músculo esquelético: 83.9% transmisión, μ = 0.0351 cm⁻¹
- Hueso compacto: 74.4% transmisión, μ = 0.0590 cm⁻¹

### Tecnologías Utilizadas
- GEANT4 11.3.2 (simulación Monte Carlo)
- ROOT 6.36.04 (análisis de datos y visualización)
- C++ (desarrollo principal)
- CMake (sistema de construcción)
- Git (control de versiones)

### Estado del Desarrollo
El proyecto cuenta con funcionalidad completa para simulaciones básicas y análisis comparativo. 

**Funcionalidades Implementadas:**
- Simulación de atenuación gamma para materiales predefinidos
- Generación automática de configuraciones multi-material
- Análisis estadístico con validación física
- Visualizaciones comparativas exportables

**Mejoras Pendientes:**
- Optimización de persistencia de datos entre simulaciones
- Refinamiento de parámetros de simulación para mayor precisión
- Implementación de manejo de errores robusto
- Extensión a materiales adicionales
- Análisis de dependencia energética

### Instalación y Uso

#### Prerrequisitos
- GEANT4 v11.3.2 o superior
- ROOT v6.36.04 o superior
- CMake 3.16 o superior
- Compilador C++ compatible con C++17

#### Compilación
```bash
cd GammaAtenuation/build
cmake ..
make
```

#### Generación de Gráficas y Análisis

**1. Configuración Multi-Material Automatizada**
```bash
# Generar archivos de configuración para los tres materiales
root -l -b setup_multi.C

# Ejecutar simulaciones (puede tomar varios minutos)
./gammaAtt temp_water.mac
./gammaAtt temp_muscle.mac  
./gammaAtt temp_bone.mac
```

**2. Análisis Comparativo Multi-Material**
```bash
# Generar gráficas comparativas y DataFrame
root -l -b multi_analysis.C
```
Genera:
- coeficientes_atenuacion.png (comparación de coeficientes μ)
- transmision_vs_densidad.png (relación densidad-transmisión)
- Salida tipo DataFrame con estadísticas comparativas

**3. Análisis Detallado de Agua**
```bash
# Generar histogramas específicos para agua
root -l -b water_histogram.C
```
Genera:
- water_analysis.png (análisis completo con 4 paneles)
- Validación experimental vs teórica Beer-Lambert
- Estadísticas detalladas de transmisión

#### Archivos de Salida
- **Datos ROOT**: data_run0.root (datos de simulación)
- **Gráficas PNG**: Visualizaciones listas para presentaciones
- **Logs**: Resultados estadísticos en consola
- **Configuraciones**: temp_*.mac (archivos temporales de simulación)

