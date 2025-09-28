#!/bin/bash

# Script para ejecutar simulaciones batch con diferentes materiales y espesores

echo "=== Iniciando simulaciones batch para atenuación gamma ==="

# Limpiar archivos anteriores
rm -f results_summary.txt attenuation_data.csv event_data.csv

# Crear header para CSV
echo "Material,Espesor_cm,Eventos_Totales,Eventos_Transmitidos,Transmision,Coef_Atenuacion" > attenuation_data.csv

# Arrays con parámetros
materials=("water" "muscle" "bone" "fat")
thicknesses=(1 2 3 4 5 7 10 15 20)  # en cm
eventos=1000

echo "Ejecutando ${#materials[@]} materiales x ${#thicknesses[@]} espesores = $((${#materials[@]} * ${#thicknesses[@]})) simulaciones"

contador=1
total=$((${#materials[@]} * ${#thicknesses[@]}))

for material in "${materials[@]}"; do
    for thickness in "${thicknesses[@]}"; do
        echo "[$contador/$total] Simulando: $material, ${thickness}cm"
        
        # Crear macro temporal
        cat > temp_macro.mac << EOF
# Configuración para $material - ${thickness}cm
/control/verbose 1
/run/verbose 1
/event/verbose 0
/tracking/verbose 0

# Configurar detector
/detector/setMaterial $material
/detector/setThickness ${thickness} cm

# Inicializar
/run/initialize

# Ejecutar simulación
/run/beamOn $eventos
EOF

        # Ejecutar simulación
        ./gammaAtt temp_macro.mac > log_${material}_${thickness}cm.txt 2>&1
        
        contador=$((contador + 1))
    done
done

# Limpiar archivos temporales
rm -f temp_macro.mac

echo "=== Simulaciones completadas ==="
echo "Resultados en: attenuation_data.csv y results_summary.txt"
echo "Para análisis con ROOT, ejecutar: root -l analyze_results.C"