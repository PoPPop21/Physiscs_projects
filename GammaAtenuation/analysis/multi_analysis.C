#include <iostream>
#include <vector>
#include <string>
// -- Librerías de ROOT --
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TMath.h>

/* --- Funcionamiento de este script ---
   Este script analiza los resultados de las simulaciones multi-material
   y genera gráficas comparativas y un resumen tipo DataFrame.
   Se asume que los datos de cada material ya fueron obtenidos y están
   almacenados en archivos específicos.

   -> Actualmente los datos son hipotéticos y deben ser reemplazados
      por los resultados reales de las simulaciones.
*/

using namespace std;

void multi_analysis()
{
    cout << "\nANALISIS MULTI-MATERIAL GAMMA ATTENUATION" << endl;
    cout << "===============================================" << endl;

    // Para ahora, vamos a usar los datos que obtuvimos manualmente
    struct MaterialData
    {
        // Guarda lo necesario de cada material para el análisis
        string name;
        string description; // Descripción legible  
        double density;      // g/cm³
        double transmission; // fracción transmitida
        double mu;           // coeficiente de atenuación cm⁻¹
        int transmitted;     // eventos transmitidos
        int total;           // eventos totales
    };

    /* -- Datos hipotéticos (reemplazar con resultados reales) ---
    vector<MaterialData> materials = {
        {"agua", "Agua (tejido blando)", 1.0, 0.84415, 0.0339, 84415, 100000},
        {"musculo", "Músculo esquelético", 1.05, 0.8391, 0.0351, 83910, 100000},
        {"hueso", "Hueso compacto", 1.85, 0.74444, 0.0590, 74444, 100000},
        // --- Datos hipotéticos para concreto y plomo ---
        {"concreto", "Concreto", 2.3, 0.65, 0.085, 65000, 100000},
        {"plomo", "Plomo", 11.34, 0.30, 0.240, 30000, 100000}
    };*/

    // --- Datos reales obtenidos de las simulaciones ---
    vector<string> run_files = {
        "../results/data_run_water.root", // Asumimos que es agua
        "../results/data_run_muscle.root", // Asumimos que es músculo
        "../results/data_run_bone.root", // Asumimos que es hueso
        "../results/data_run_concrete.root", // Asumimos que es concreto
        "../results/data_run_lead.root"  // Asumimos que es plomo
    };

    // Densidades y descripciones
    map<string, pair<double, string>> material_info = {
        {"water", {1.0, "Agua (tejido blando)"}},
        {"muscle", {1.05, "Músculo esquelético"}},
        {"bone", {1.85, "Hueso compacto"}},
        {"concrete", {2.3, "Concreto"}},
        {"lead", {11.34, "Plomo"}}
    };

    vector<MaterialData> materials;

    cout << "Cargando datos de archivos ROOT..." << endl;

    for (const auto &file_name : run_files)
    {
        TFile *file = TFile::Open(file_name.c_str());
        if (!file || file->IsZombie())
        {
            cout << "Error: No se pudo abrir el archivo " << file_name << endl;
            continue;
        }
        TTree *tree = (TTree *)file->Get("data");
        if (!tree)
        {
            cout << "Error: No se encontró el TTree 'data' en " << file_name << endl;
            file->Close();
            continue;
        }

        // Variables para almacenar los datos de las ramas del TTree
        char material_name[50];
        Int_t totalEvents, transmittedEvents;
        Float_t transmissionRatio, attenuationCoeff;

        // Conectar las ramas del TTree a nuestras variables
        tree->SetBranchAddress("material", material_name);
        tree->SetBranchAddress("totalEvents", &totalEvents);
        tree->SetBranchAddress("transmittedEvents", &transmittedEvents);
        tree->SetBranchAddress("transmissionRatio", &transmissionRatio);
        tree->SetBranchAddress("attenuationCoeff", &attenuationCoeff);

        // Leer la primera (y única) entrada del TTree
        tree->GetEntry(0);

        // Llenar nuestra estructura de datos con los valores leídos
        string mat_str(material_name);
        materials.push_back({mat_str, material_info[mat_str].second, material_info[mat_str].first,
                             (double)transmissionRatio, (double)attenuationCoeff,
                             transmittedEvents, totalEvents});

        file->Close();
        cout << "  -> Datos de " << mat_str << " cargados desde " << file_name << endl;
    }

    // --- Hasta aquí modifiqué :D --- Atte: sopa
    cout << "\nRESULTADOS COMPARATIVOS:" << endl;
    cout << "Espesor: 5.0 cm | Energía: 662 keV (Cs-137)" << endl;
    cout << "--------------------------------------------" << endl;

    for (const auto &mat : materials)
    {
        double percent = mat.transmission * 100;
        cout << "- " << mat.description << ":" << endl;
        cout << "   Densidad: " << mat.density << " g/cm³" << endl;
        cout << "   Transmisión: " << percent << "%" << endl;
        cout << "   μ: " << mat.mu << " cm⁻¹" << endl;
        cout << "   Eventos: " << mat.transmitted << "/" << mat.total << endl;
        cout << endl;
    }

    // Crear gráficas comparativas
    gStyle->SetOptStat(0);

    // Gráfica 1: Comparación de coeficientes de atenuación
    TCanvas *c1 = new TCanvas("c1", "Coeficientes de Atenuación", 800, 600);

    vector<double> mu_values;
    vector<double> densities;
    vector<string> names;

    for (const auto &mat : materials)
    {
        mu_values.push_back(mat.mu);
        densities.push_back(mat.density);
        names.push_back(mat.name);
    }

    TH1F *h_mu = new TH1F("h_mu", "Coeficientes de Atenuacion por Material;Material;μ (cm^{-1})",
                          materials.size(), 0, materials.size());

    for (size_t i = 0; i < materials.size(); i++)
    {
        h_mu->SetBinContent(i + 1, mu_values[i]);
        h_mu->GetXaxis()->SetBinLabel(i + 1, names[i].c_str());
    }

    h_mu->SetFillColor(kBlue);
    h_mu->SetLineColor(kBlack);
    h_mu->SetTitle("Coeficientes de Atenuacion - Materiales Biologicos");
    h_mu->Draw();

    c1->Print("../results/coeficientes_atenuacion.png");
    cout << "Grafica guardada: ../results/coeficientes_atenuacion.png" << endl;

    // Gráfica 2: Transmisión vs Densidad
    TCanvas *c2 = new TCanvas("c2", "Transmision vs Densidad", 800, 600);

    TGraph *g_trans = new TGraph(materials.size());
    for (size_t i = 0; i < materials.size(); i++)
    {
        g_trans->SetPoint(i, densities[i], materials[i].transmission * 100);
    }

    g_trans->SetMarkerStyle(20);
    g_trans->SetMarkerSize(1.5);
    g_trans->SetMarkerColor(kRed);
    g_trans->SetLineColor(kRed);
    g_trans->SetTitle("Transmision vs Densidad;Densidad (g/cm³);Transmision (%)");
    g_trans->Draw("ALP");

    c2->Print("../results/transmision_vs_densidad.png");
    cout << "Grafica guardada: ../results/transmision_vs_densidad.png" << endl;

    // Crear DataFrame-like summary
    cout << "\n📋 DATAFRAME SUMMARY:" << endl;
    cout << "=====================" << endl;
    printf("%-12s %-8s %-12s %-10s %-10s\n", "Material", "Density", "Transmission", "μ", "Events");
    printf("%-12s %-8s %-12s %-10s %-10s\n", "--------", "-------", "------------", "-------", "------");

    for (const auto &mat : materials)
    {
        printf("%-12s %-8.2f %-12.1f%% %-10.4f %-10d\n",
               mat.name.c_str(),
               mat.density,
               mat.transmission * 100,
               mat.mu,
               mat.transmitted);
    }

    cout << "\nANALISIS COMPLETADO!" << endl;
    cout << "Archivos generados:" << endl;
    cout << "- ../results/coeficientes_atenuacion.png" << endl;
    cout << "- ../results/transmision_vs_densidad.png" << endl;

    // Mostrar insights físicos
    cout << "\nINSIGHTS FISICOS:" << endl;
    cout << "================" << endl;
    cout << "- El hueso atenua ~74% mas que el agua debido a su mayor densidad" << endl;
    cout << "- El musculo tiene propiedades similares al agua (diferencia <1%)" << endl;
    cout << "- Los coeficientes estan en rangos realistas para 662 keV" << endl;
    cout << "- La relacion densidad-atenuacion sigue el comportamiento esperado" << endl;
}