#!/bin/bash

echo "=== Multi-Energy Analysis ==="
echo "Análisis de coeficientes de atenuación másicos vs energía"

# Crear directorio de resultados si no existe
mkdir -p results/multi_energy

# Paso 1: Ejecutar análisis ROOT
echo "Paso 1: Ejecutando análisis ROOT..."
cd /home/isabel/Physiscs_projects/GammaAtenuation

root -l -b -q "analysis/multi_energy_analysis.C"

if [ $? -eq 0 ]; then
    echo "✓ Análisis ROOT completado"
else
    echo "✗ Error en análisis ROOT"
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