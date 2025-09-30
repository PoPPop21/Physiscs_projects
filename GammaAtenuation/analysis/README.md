# Scripts de Análisis - GammaAtenuation

Scripts de análisis ROOT optimizados para simulación de atenuación gamma multi-material.

## 📋 **FLUJO DE TRABAJO COMPLETO**

### **1. PREPARACIÓN (solo una vez)**

Se debe crear la carpeta build para compilar, esta es ignorada por el gitignore. 

```bash
cd /Physiscs_projects/GammaAtenuation/build
make
```

### **2. GENERAR CONFIGURACIONES** 🎀

El archivo setup_multi.C, lo que hace es generar las gráficas, el archivo que se debe compilar posteriormente es multi_analysis.C, para visualizar las gráficas de coeficiente de atenuación lineal e histogramas.

Esto se debe compilar en la carpeta de analysis. 

```bash
root -q "../analysis/setup_multi.C"
```
- **Función**: Genera archivos `.mac` para los 3 materiales
- **Output**: `temp_water.mac`, `temp_muscle.mac`, `temp_bone.mac` en `../mac/`

### **3. EJECUTAR SIMULACIONES** 🎀

Esto se ejecuta en la carpeta de build, antes de compilar el multi_analysis.C, ya que crea los macros para poder visualizar las gráficas. 

```bash
./gammaAtt ../mac/temp_water.mac      # Simula agua
./gammaAtt ../mac/temp_muscle.mac     # Simula músculo  
./gammaAtt ../mac/temp_bone.mac       # Simula hueso
```
Estos son los parámetros de la simulación especificados en setup_multi.C.

- **Eventos por simulación**: 100,000
- **Espesor**: 5.0 cm  
- **Energía**: 662 keV (Cs-137)

### **4. EJECUTAR ANÁLISIS (generan PNGs automáticamente)**

Se ejecuta en la carpeta de analysis. 👀

```bash
root -q "../analysis/water_histogram.C"      # → water_analysis.png
root -q "../analysis/analyze_attenuation.C"  # → attenuation_analysis.png
root -q "../analysis/multi_analysis.C"       # → 2 gráficas comparativas
```

---

## 📊 **Descripción de Scripts**

### `setup_multi.C`
**Generador de configuraciones automatizadas**
- ✅ Crea archivos `.mac` para agua, músculo, hueso
- ✅ Configura parámetros: espesor, eventos, energía de fuente
- ✅ **Se ejecuta PRIMERO** para preparar simulaciones

### `analyze_attenuation.C`
**Análisis completo individual**
- ✅ Lee datos de `../results/data_run0.root`
- ✅ Genera análisis de 4 paneles con validación Beer-Lambert
- ✅ **Output**: `attenuation_analysis.png` + `attenuation_analysis.pdf`

### `multi_analysis.C` 
**Análisis comparativo de 3 materiales**
- ✅ Usa datos hardcodeados de simulaciones previas
- ✅ Genera gráficas de coeficientes μ y transmisión vs densidad
- ✅ **Output**: `coeficientes_atenuacion.png` + `transmision_vs_densidad.png`
- ✅ DataFrame summary en terminal

### `water_histogram.C`
**Análisis detallado específico del agua**
- ✅ Histogramas de eventos transmitidos/atenuados
- ✅ Validación experimental vs Beer-Lambert teórica
- ✅ Panel de 4 gráficas con estadísticas completas
- ✅ **Output**: `water_analysis.png`

---

## 📈 **Resultados Generados**

### **Datos** (en `../results/`):
- `data_run0.root` - Tree con datos de simulación
- `results_summary.txt` - Resumen estadístico 
- `attenuation_data.csv` - Datos tabulados
- `event_data.csv` - Datos evento por evento

### **Gráficas PNG** (en `../results/`):
- `water_analysis.png` - Análisis histográfico agua (4 paneles)
- `attenuation_analysis.png` - Análisis individual completo (4 paneles)  
- `coeficientes_atenuacion.png` - Barras comparativas μ por material
- `transmision_vs_densidad.png` - Correlación transmisión-densidad

---

## ⚙️ **Detalles Técnicos**

### **Materiales Simulados:**
| Material | G4_Name | Densidad | μ típico | Transmisión típica |
|----------|---------|----------|----------|--------------------|
| Agua     | G4_WATER| 1.0 g/cm³| 0.0339  | 84.4%             |
| Músculo  | muscle  | 1.05 g/cm³| 0.0351 | 83.9%             |
| Hueso    | bone    | 1.85 g/cm³| 0.0590 | 74.4%             |

### **Parámetros de Simulación:**
- **Fuente**: Cs-137 (662 keV)
- **Geometría**: Detector plano
- **Estadística**: 100,000 eventos por material
- **Física**: Procesos electromagnéticos estándar

### **Validaciones Incluidas:**
- ✅ Ley Beer-Lambert: I = I₀e^(-μx)
- ✅ Correlación densidad-atenuación
- ✅ Rangos físicos realistas para 662 keV
- ✅ Estadística robusta (>100k eventos)

---

## 🚫 **Archivos Eliminados**
- `quick.C` - Eliminado por ser redundante con `analyze_attenuation.C`

**Nota**: Todos los scripts se ejecutan desde `build/` y guardan automáticamente en `../results/`

## Archivos incluidos:

### `setup_multi.C`
Script para generar configuraciones automatizadas de simulación multi-material.
- Genera archivos `.mac` para agua, músculo y hueso en `../mac/`
- Configura parámetros de simulación (espesor, eventos, energía)
- Uso: `root -l -b setup_multi.C`

### `multi_analysis.C`
Análisis comparativo de los tres materiales biológicos.
- Genera gráficas comparativas de coeficientes de atenuación
- Crea visualización de transmisión vs densidad
- Produce salida tipo DataFrame con estadísticas
- Uso: `root -l -b multi_analysis.C`
- Salida: `../results/coeficientes_atenuacion.png`, `../results/transmision_vs_densidad.png`

### `water_histogram.C`
Análisis detallado específico para el material agua.
- Genera histogramas de eventos transmitidos/atenuados
- Validación experimental vs teórica (Beer-Lambert)
- Panel multi-gráfico con estadísticas completas
- Uso: `root -l -b water_histogram.C`
- Salida: `../results/water_analysis.png`

## Flujo de trabajo recomendado:

1. **Preparación de simulaciones:**
   ```bash
   cd /path/to/GammaAtenuation/analysis
   root -l -b setup_multi.C
   ```

2. **Ejecutar simulaciones:**
   ```bash
   cd ../build
   ./gammaAtt ../mac/temp_water.mac && ./gammaAtt ../mac/temp_muscle.mac && ./gammaAtt ../mac/temp_bone.mac && ./gammaAtt ../mac/temp_concrete.mac && ./gammaAtt ../mac/temp_lead.mac
   ```

3. **Generar análisis comparativo:**
   ```bash
   cd ../analysis
   root -l -b multi_analysis.C
   ```

4. **Análisis específico de agua:**
   ```bash
   root -l -b water_histogram.C
   ```

## Notas importantes:
- Los scripts son independientes del directorio `build/`
- Se pueden ejecutar desde cualquier ubicación dentro de `analysis/`
- Los archivos de salida (PNG) se generan en `../results/`
- Los archivos .mac se generan en `../mac/`
- Requieren que las simulaciones hayan sido ejecutadas previamente