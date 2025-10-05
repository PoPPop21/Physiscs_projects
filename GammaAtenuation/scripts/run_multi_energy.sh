#!/bin/bash

echo "=== Multi-Energy Analysis ==="
echo "Análisis de coeficientes de atenuación másicos vs energía"

# Crear directorio de resultados si no existe
mkdir -p results/multi_energy

# Paso 1: Compilar análisis ROOT
echo "Paso 1: Compilando análisis multi-energía..."
cd /home/isabel/Physiscs_projects/GammaAtenuation
g++ -o analysis/multi_energy_analysis analysis/multi_energy_analysis.C \
    -std=c++17

if [ $? -eq 0 ]; then
    echo "✓ Compilación exitosa"
else
    echo "✗ Error en compilación"
    exit 1
fi

# Paso 2: Ejecutar análisis
echo "Paso 2: Ejecutando análisis multi-energía..."
./analysis/multi_energy_analysis

if [ $? -eq 0 ]; then
    echo "✓ Análisis completado"
else
    echo "✗ Error en análisis"
    exit 1
fi

echo "=========================================="
echo "  ANÁLISIS MULTI-ENERGÍA COMPLETADO"
echo "=========================================="
echo ""
echo "Datos generados en:"
echo "  /home/isabel/Physiscs_projects/GammaAtenuation/results/multi_energy/"
echo ""
echo "Para generar las gráficas, ejecuta:"
echo "  source GA/bin/activate"
echo "  python analysis/plot_multi_energy.py"
echo ""