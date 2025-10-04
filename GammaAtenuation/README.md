# Gamma Attenuation Simulation

Simulación de atenuación de rayos gamma usando GEANT4 con análisis comparativo multi-material y estudios de variación de espesor.

## Descripción del Proyecto

Este proyecto simula la atenuación de rayos gamma de 662 keV (Cs-137) en diferentes materiales biológicos usando GEANT4 v11.3.2 y ROOT v6.36.04.

### Materiales Analizados:
- **Agua** (tejido blando) - Densidad: 1.0 g/cm³
- **Músculo** esquelético - Densidad: 1.05 g/cm³  
- **Hueso** compacto - Densidad: 1.85 g/cm³

## Instalación y Compilación

### Requisitos:
- GEANT4 v11.3.2+
- ROOT v6.36.04+
- CMake 3.16+
- C++ Compiler con soporte C++17

### Compilación:
```bash
cd build/
cmake ..
make
```

## Flujos de Trabajo Disponibles

### FLUJO 1: Análisis Multi-Material (Principal)

#### 1. PREPARACIÓN (solo una vez)
```bash
cd /home/isabel/Physiscs_projects/GammaAtenuation/build
make
```

#### 2. GENERAR CONFIGURACIONES
```bash
root -q "../analysis/setup_multi.C"
```
- **Función**: Genera archivos `.mac` para los 3 materiales
- **Output**: `temp_water.mac`, `temp_muscle.mac`, `temp_bone.mac`

#### 3. EJECUTAR SIMULACIONES
```bash
./gammaAtt ../mac/temp_water.mac      # Simula agua
./gammaAtt ../mac/temp_muscle.mac     # Simula músculo  
./gammaAtt ../mac/temp_bone.mac       # Simula hueso
```
- **Eventos por simulación**: 100,000
- **Espesor**: 5.0 cm
- **Energía**: 662 keV (Cs-137)

#### 4. EJECUTAR ANÁLISIS
```bash
root -q "../analysis/water_histogram.C"      # → water_analysis.png
root -q "../analysis/analyze_attenuation.C"  # → attenuation_analysis.png
root -q "../analysis/multi_analysis.C"       # → 2 gráficas comparativas
```

### FLUJO 2: Estudio de Variación de Espesor (Automático)

#### Ejecución con un solo comando:
```bash
cd /home/isabel/Physiscs_projects/GammaAtenuation
bash scripts/thickness_study_auto.sh
```

**Qué hace automáticamente:**
- Genera 8 simulaciones con espesores: 0.5, 1.0, 2.0, 3.0, 5.0, 7.5, 10.0, 15.0 cm
- Crea macros automáticamente si no existen
- Ejecuta todas las simulaciones (800,000 eventos totales)
- Genera análisis de la Ley de Beer-Lambert
- Produce gráficas PNG/PDF y datos CSV
- Validación estadística completa

**Resultados generados:**
- `results/thickness_study.png` - Análisis completo (4 paneles)
- `results/thickness_study.pdf` - Para presentaciones
- `results/thickness_study_results.csv` - Datos tabulados

## Estructura del Proyecto

```
GammaAtenuation/
├── analysis/          # Scripts de análisis ROOT
│   ├── setup_multi.C           # Generador de configuraciones
│   ├── analyze_attenuation.C   # Análisis individual completo
│   ├── multi_analysis.C        # Análisis comparativo 3 materiales
│   ├── water_histogram.C       # Análisis detallado agua
│   ├── thickness_study.C       # Análisis variación de espesor
│   └── README.md              # Documentación de análisis
├── scripts/           # Scripts de automatización
│   └── thickness_study_auto.sh # Estudio automático de espesor
├── build/             # Compilación y ejecutables
├── include/           # Headers de GEANT4
├── src/              # Código fuente GEANT4
├── mac/              # Archivos de configuración GEANT4
├── results/          # Resultados y gráficas
└── README.md         # Este archivo
```

## Resultados Generados

### Archivos de Datos:
- `data_run0.root` - Archivo ROOT con tree de datos
- `results_summary.txt` - Resumen estadístico
- `attenuation_data.csv` - Datos tabulados
- `event_data.csv` - Datos evento por evento

### Gráficas PNG:
- `water_analysis.png` - Análisis histográfico detallado del agua
- `attenuation_analysis.png` - Análisis completo con 4 paneles
- `coeficientes_atenuacion.png` - Comparación de coeficientes μ
- `transmision_vs_densidad.png` - Correlación transmisión-densidad
- `thickness_study.png` - Estudio de variación de espesor (4 paneles)
- `thickness_study.pdf` - Versión para presentaciones

## Resultados Físicos Típicos

| Material | Densidad (g/cm³) | Transmisión (%) | μ (cm⁻¹) |
|----------|------------------|-----------------|-----------|
| Agua     | 1.0             | 84.4%           | 0.0339   |
| Músculo  | 1.05            | 83.9%           | 0.0351   |
| Hueso    | 1.85            | 74.4%           | 0.0590   |

### Validación Física:
- Coeficientes en rangos realistas para 662 keV  
- Seguimiento de ley Beer-Lambert: I = I₀e^(-μx)  
- Correlación correcta densidad-atenuación  
- Estadística robusta con 100,000 eventos  

### Validación de Beer-Lambert (Estudio de Espesor):
- Análisis logarítmico ln(I/I₀) vs espesor  
- Ajuste lineal con estadísticas χ²/ndf y R²  
- Comparación con valores de literatura  
- Cálculo de espesores de semirreducción  

## Documentación Adicional

- Ver `analysis/README.md` para detalles de cada script
- Los archivos `.mac` definen configuraciones de GEANT4
- Todos los outputs se guardan automáticamente en `results/`
- Los archivos `thickness_water_*.mac` se generan automáticamente y están en `.gitignore`

## Desarrollo

**Rama actual**: `isabel-root-integration`  
**Autor**: Isabel  
**Última actualización**: Octubre 2025  

Para contribuir, mantener la estructura de directorios y ejecutar pruebas antes de commit.