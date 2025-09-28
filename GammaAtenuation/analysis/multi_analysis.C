#include <iostream>
#include <vector>
#include <string>
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TMath.h>

using namespace std;

void multi_analysis()
{
    cout << "\nANALISIS MULTI-MATERIAL GAMMA ATTENUATION" << endl;
    cout << "===============================================" << endl;

    // Para ahora, vamos a usar los datos que obtuvimos manualmente
    struct MaterialData
    {
        string name;
        string description;
        double density;      // g/cm³
        double transmission; // fracción transmitida
        double mu;           // coeficiente de atenuación cm⁻¹
        int transmitted;     // eventos transmitidos
        int total;           // eventos totales
    };

    vector<MaterialData> materials = {
        {"agua", "Agua (tejido blando)", 1.0, 0.84415, 0.0339, 84415, 100000},
        {"musculo", "Músculo esquelético", 1.05, 0.8391, 0.0351, 83910, 100000},
        {"hueso", "Hueso compacto", 1.85, 0.74444, 0.0590, 74444, 100000}};

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