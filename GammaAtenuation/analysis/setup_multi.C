#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

// Script para generar simulaciones de múltiples materiales
void setup_multi()
{

    cout << "\n============== GENERADOR MULTI-MATERIAL ==============" << endl;
    cout << "Generando datos para análisis comparativo" << endl;

    // Configuraciones para diferentes materiales
    struct MaterialConfig
    {
        string name;
        string g4_name;
        double density; // g/cm³
        string description;
    };

    vector<MaterialConfig> materials = {
        {"water", "water", 1.0, "Agua (tejido blando)"},
        {"muscle", "muscle", 1.05, "Músculo esquelético"},
        {"bone", "bone", 1.85, "Hueso compacto"}};

    double thickness = 5.0; // cm - espesor estándar
    int events = 100000;    // eventos por simulación

    cout << "Configuracion:" << endl;
    cout << "   - Espesor: " << thickness << " cm" << endl;
    cout << "   - Eventos: " << events << " por material" << endl;
    cout << "   - Energia: 662 keV (Cs-137)" << endl;
    cout << "=================================================" << endl;

    for (const auto &mat : materials)
    {
        cout << "\nProcesando: " << mat.description << endl;
        cout << "   Material: " << mat.g4_name << endl;
        cout << "   Densidad: " << mat.density << " g/cm³" << endl;

        // Crear archivo de configuración temporal
        string mac_file = "../mac/temp_" + mat.name + ".mac";
        ofstream mac(mac_file);

        mac << "# Configuración automática para " << mat.description << endl;
        mac << "/run/initialize" << endl;
        mac << endl;
        mac << "# Configurar detector" << endl;
        mac << "/detector/setMaterial " << mat.g4_name << endl;
        mac << "/detector/setThickness " << thickness << " cm" << endl;
        mac << endl;
        mac << "# Configurar fuente (Cs-137)" << endl;
        mac << "/gun/particle gamma" << endl;
        mac << "/gun/energy 662 keV" << endl;
        mac << "/gun/position 0 0 -10 cm" << endl;
        mac << "/gun/direction 0 0 1" << endl;
        mac << endl;
        mac << "# Ejecutar simulación" << endl;
        mac << "/run/beamOn " << events << endl;
        mac.close();

        cout << "    Archivo: " << mac_file << " creado" << endl;
        cout << "     Ejecutar con: ./gammaAtt ../mac/temp_" << mat.name << ".mac" << endl;
    }

    cout << "\n=============================================" << endl;
    cout << " Archivos de configuración generados!" << endl;
    cout << "\n INSTRUCCIONES:" << endl;
    cout << "  Ejecutar cada simulación:" << endl;

    for (const auto &mat : materials)
    {
        cout << "    ./gammaAtt ../mac/temp_" << mat.name << ".mac" << endl;
    }

    cout << "\n Ejecutar análisis comparativo:" << endl;
    cout << "    root multi_analysis.C" << endl;

    cout << "\n Limpiar archivos temporales:" << endl;
    cout << "    rm ../mac/temp_*.mac" << endl;

    cout << "\nListo para análisis multi-material" << endl;
}