# Scripts de Análisis - GammaAtenuation

Esta carpeta contiene los scripts de análisis ROOT para el proyecto de simulación de atenuación gamma.

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
   ./gammaAtt ../mac/temp_water.mac
   ./gammaAtt ../mac/temp_muscle.mac
   ./gammaAtt ../mac/temp_bone.mac
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