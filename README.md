# Gamma Attenuation Simulation

Simulación de atenuación de rayos gamma usando GEANT4 con análisis comparativo multi-material.

## 📋 Descripción del Proyecto

Este proyecto simula la atenuación de rayos gamma de 662 keV (Cs-137) en diferentes materiales biológicos usando GEANT4 v11.3.2 y ROOT v6.36.04.

### Materiales Analizados:
- **Agua** (tejido blando) - Densidad: 1.0 g/cm³
- **Músculo** esquelético - Densidad: 1.05 g/cm³  
- **Hueso** compacto - Densidad: 1.85 g/cm³

## 🚀 Instalación y Compilación

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

## 📊 **FLUJO DE TRABAJO COMPLETO**

### **1. PREPARACIÓN (solo una vez)**
```bash
cd /Physiscs_projects/GammaAtenuation/build
make
```

### **2. GENERAR CONFIGURACIONES**
```bash
root -q "../analysis/setup_multi.C"
```
- **Función**: Genera archivos `.mac` para los 3 materiales
- **Output**: `temp_water.mac`, `temp_muscle.mac`, `temp_bone.mac`

### **3. EJECUTAR SIMULACIONES**
ro
```
- **Eventos por simulación**: 100,000
- **Espesor**: 5.0 cm
- **Energía**: 662 keV (Cs-137)

### **4. EJECUTAR ANÁLISIS (generan PNGs automáticamente)**
```bash
root -q "../analysis/water_histogram.C"      # → water_analysis.png
root -q "../analysis/analyze_attenuation.C"  # → attenuation_analysis.png
root -q "../analysis/multi_analysis.C"       # → 2 gráficas comparativas
```

## 📁 Estructura del Proyecto

```
GammaAtenuation/
├── analysis/          # Scripts de análisis ROOT
│   ├── setup_multi.C       # Generador de configuraciones
│   ├── analyze_attenuation.C  # Análisis individual completo
│   ├── multi_analysis.C      # Análisis comparativo 3 materiales
│   ├── water_histogram.C     # Análisis detallado agua
│   └── README.md            # Documentación de análisis
├── build/             # Compilación y ejecutables
├── include/           # Headers de GEANT4
├── src/              # Código fuente GEANT4
├── mac/              # Archivos de configuración GEANT4
├── results/          # Resultados y gráficas
└── README.md         # Este archivo
```

## 📈 Resultados Generados

### **Archivos de Datos:**
- `data_run0.root` - Archivo ROOT con tree de datos
- `results_summary.txt` - Resumen estadístico
- `attenuation_data.csv` - Datos tabulados
- `event_data.csv` - Datos evento por evento

### **Gráficas PNG:**
- `water_analysis.png` - Análisis histográfico detallado del agua
- `attenuation_analysis.png` - Análisis completo con 4 paneles
- `coeficientes_atenuacion.png` - Comparación de coeficientes μ
- `transmision_vs_densidad.png` - Correlación transmisión-densidad

## 🔬 Resultados Físicos Típicos

| Material | Densidad (g/cm³) | Transmisión (%) | μ (cm⁻¹) |
|----------|------------------|-----------------|-----------|
| Agua     | 1.0             | 84.4%           | 0.0339   |
| Músculo  | 1.05            | 83.9%           | 0.0351   |
| Hueso    | 1.85            | 74.4%           | 0.0590   |

### **Validación Física:**
✅ Coeficientes en rangos realistas para 662 keV  
✅ Seguimiento de ley Beer-Lambert: I = I₀e^(-μx)  
✅ Correlación correcta densidad-atenuación  
✅ Estadística robusta con 100,000 eventos  

## 📖 Documentación Adicional

- Ver `analysis/README.md` para detalles de cada script
- Los archivos `.mac` definen configuraciones de GEANT4
- Todos los outputs se guardan automáticamente en `results/`
