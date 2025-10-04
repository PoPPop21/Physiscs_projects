#!/bin/bash

# Script automático y amigable para estudio de variación de espesor
# Autor: @isabelnieto900, @PoPPop21
# Fecha: Octubre 2025

echo "ESTUDIO AUTOMÁTICO DE VARIACIÓN DE ESPESOR"
echo "=============================================="
echo ""
echo "Este script ejecutará automáticamente:"
echo "   • 8 simulaciones con diferentes espesores (0.5 - 15.0 cm)"
echo "   • Análisis estadístico completo"
echo "   • Generación de gráficas PNG/PDF"
echo "   • Validación de la Ley de Beer-Lambert"
echo ""
echo "Tiempo estimado: ~15-20 minutos"
echo ""

read -p "¿Continuar? (y/n): " -n 1 -r
echo ""

if [[ $REPLY =~ ^[Yy]$ ]]; then
    echo "Iniciando proceso automático..."
    echo ""

    # Configuración
    MATERIAL="water"
    THICKNESS_VALUES=(0.5 1.0 2.0 3.0 5.0 7.5 10.0 15.0)
    BUILD_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)/build"
    BASE_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

    cd "$BUILD_DIR"

    echo "Ejecutando simulaciones para ${#THICKNESS_VALUES[@]} espesores diferentes..."
    echo ""

    # Loop para cada espesor
    for thickness in "${THICKNESS_VALUES[@]}"; do
        echo ">>> Procesando espesor: ${thickness} cm"

        MAC_FILE="../mac/thickness_${MATERIAL}_${thickness}.mac"

        # Si el macro no existe, crearlo automáticamente
        if [ ! -f "$MAC_FILE" ]; then
            echo "Macro $MAC_FILE no encontrado. Creando automáticamente..."
            cat > "$MAC_FILE" <<EOF
# Macro generado automáticamente para ${MATERIAL} con espesor ${thickness} cm
/run/initialize
/control/verbose 1
/run/verbose 1
/tracking/verbose 0
/detector/setMaterial ${MATERIAL}
/detector/setThickness ${thickness} cm
/run/beamOn 100000
EOF
        fi

        # Ejecutar simulación
        echo "    Ejecutando simulación..."
        ./gammaAtt "$MAC_FILE" > /dev/null 2>&1

        if [ $? -eq 0 ]; then
            echo "    Simulación completada"

            # Renombrar archivo de salida
            SOURCE_FILE="../results/data_run_${MATERIAL}.root"
            TARGET_FILE="../results/data_thickness_${MATERIAL}_${thickness}.root"

            if [ -f "$SOURCE_FILE" ]; then
                mv "$SOURCE_FILE" "$TARGET_FILE"
                echo "    Archivo guardado como: data_thickness_${MATERIAL}_${thickness}.root"
            else
                echo "    ERROR: No se encontró el archivo de salida"
            fi
        else
            echo "    ERROR en la simulación"
        fi

        echo ""
    done

    echo "===== TODAS LAS SIMULACIONES COMPLETADAS ====="
    echo ""
    echo "Archivos generados:"
    ls -la ../results/data_thickness_*.root 2>/dev/null || echo "No se encontraron archivos de salida"

    echo ""
    echo "Ejecutando análisis automático..."
    root -l -b -q "../analysis/thickness_study.C" > /dev/null 2>&1

    if [ $? -eq 0 ]; then
        echo "Análisis completado exitosamente"
        echo ""
        echo "Resultados generados:"
        echo "   results/thickness_study.png"
        echo "   results/thickness_study.pdf"
        echo "   results/thickness_study_results.csv"
        echo ""
        echo "Para ver las gráficas:"
        echo "   xdg-open results/thickness_study.png"
    else
        echo "Error en el análisis"
    fi

    echo ""
    echo "PROCESO COMPLETADO"
else
    echo "Proceso cancelado"
fi