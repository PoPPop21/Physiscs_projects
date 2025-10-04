/* ------- ANÁLISIS DE VARIACIÓN DE ESPESOR -------
 * Estudio de la Ley de Beer-Lambert en función del espesor
 * Autor: @isabelnieto900, @PoPPop21
 * Fecha: Octubre 2025
 * ----------------------------------------------
 * Uso: root -l thickness_study.C
 */

void thickness_study() {
    // Configuración de estilo
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(1);
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);
    
    // Crear canvas
    TCanvas *c1 = new TCanvas("c1", "Estudio de Variación de Espesor", 1000, 800);
    c1->Divide(2, 2);
    
    // Para almacenar datos de diferentes espesores
    const int nPoints = 8;
    double thickness[nPoints] = {0.5, 1.0, 2.0, 3.0, 5.0, 7.5, 10.0, 15.0}; // en cm
    double transmission[nPoints] = {0};
    double lnTransmission[nPoints] = {0};
    double attCoeff[nPoints] = {0};
    
    // Datos teóricos para agua con 662 keV (valor de referencia: μ = 0.086 cm⁻¹)
    double mu_teorico = 0.086; // cm⁻¹
    
    // Arrays para datos teóricos
    double thickness_th[100];
    double transmission_th[100];
    double lnTransmission_th[100];
    
    // Generar curva teórica
    for (int i = 0; i < 100; i++) {
        thickness_th[i] = i * 0.2; // 0 a 20 cm
        transmission_th[i] = exp(-mu_teorico * thickness_th[i]);
        lnTransmission_th[i] = -mu_teorico * thickness_th[i]; // ln(I/I₀) = -μx
    }
    
    // Material: agua
    const char* material = "water";
    
    // Recolectar datos de archivos existentes o generar simulaciones nuevas
    bool datosExistentes = true;
    
    for (int i = 0; i < nPoints; i++) {
        // Nombre de archivo para este espesor
        TString filename = Form("../results/data_thickness_%s_%.1f.root", material, thickness[i]);
        
        // Verificar si el archivo existe
        if (gSystem->AccessPathName(filename)) {
            // Si no existe, marcar que necesitamos simulaciones nuevas
            datosExistentes = false;
            cout << "Archivo no encontrado: " << filename << endl;
            cout << "Se requiere generar simulaciones nuevas" << endl;
            break;
        }
        
        // Si existe, extraer datos
        TFile *f = TFile::Open(filename);
        if (!f) {
            datosExistentes = false;
            break;
        }
        
        TTree *tree = (TTree*)f->Get("data");
        if (!tree) {
            f->Close();
            datosExistentes = false;
            break;
        }
        
        // Leer variables
        float transmissionRatio, attenuationCoeff;
        tree->SetBranchAddress("transmissionRatio", &transmissionRatio);
        tree->SetBranchAddress("attenuationCoeff", &attenuationCoeff);
        
        // Obtener datos
        tree->GetEntry(0);
        transmission[i] = transmissionRatio;
        lnTransmission[i] = log(transmissionRatio);
        attCoeff[i] = attenuationCoeff;
        
        f->Close();
    }
    
    // Si no existen todos los datos, generar configuraciones
    if (!datosExistentes) {
        cout << "\n===== GENERANDO ARCHIVOS DE CONFIGURACIÓN =====\n" << endl;
        
        // Crear archivo para configuración de espesores
        ofstream outfile("../mac/thickness_study.mac");
        outfile << "# Macro generado automáticamente para estudio de espesores\n";
        outfile << "# Material: " << material << "\n\n";
        outfile << "/run/initialize\n";
        outfile << "/control/verbose 1\n";
        outfile << "/run/verbose 1\n";
        outfile << "/tracking/verbose 0\n\n";
        
        // Crear macros individuales para cada espesor
        for (int i = 0; i < nPoints; i++) {
            // Macro para este espesor
            TString macroName = Form("../mac/thickness_%s_%.1f.mac", material, thickness[i]);
            ofstream macroFile(macroName.Data());
            
            macroFile << "# Configuración para " << material << " con espesor " << thickness[i] << " cm\n";
            macroFile << "/run/initialize\n";
            macroFile << "/control/verbose 1\n";
            macroFile << "/run/verbose 1\n";
            macroFile << "/tracking/verbose 0\n\n";
            
            macroFile << "/att/det/setMaterial " << material << "\n";
            macroFile << "/att/det/setThickness " << thickness[i] << " cm\n";
            macroFile << "/run/beamOn 100000\n";
            
            macroFile.close();
            
            // Agregar instrucción para ejecutar este macro
            outfile << "# Espesor " << thickness[i] << " cm\n";
            outfile << "# Ejecutar: ./gammaAtt " << macroName.Data() << "\n";
            outfile << "# Renombrar: mv ../results/data_run_" << material << ".root ../results/data_thickness_" << material << "_" << thickness[i] << ".root\n\n";
        }
        
        outfile.close();
        
        cout << "Se han generado los archivos de configuración en mac/thickness_study.mac" << endl;
        cout << "Por favor, ejecute las simulaciones y renombre los archivos según las instrucciones" << endl;
        cout << "Luego vuelva a ejecutar este script de análisis" << endl;
        
        // Mostrar instrucciones completas
        cout << "\n===== INSTRUCCIONES DE EJECUCIÓN =====\n" << endl;
        cout << "Para generar todos los datos necesarios, ejecute estas instrucciones en orden:\n" << endl;
        
        for (int i = 0; i < nPoints; i++) {
            cout << "# Espesor " << thickness[i] << " cm" << endl;
            cout << "./gammaAtt ../mac/thickness_" << material << "_" << thickness[i] << ".mac" << endl;
            cout << "mv ../results/data_run_" << material << ".root ../results/data_thickness_" << material << "_" << thickness[i] << ".root" << endl;
            cout << endl;
        }
        
        return;
    }
    
    // Panel 1: Transmisión vs Espesor (gráfica normal)
    c1->cd(1);
    
    // Crear gráfico de transmisión vs espesor
    TGraph *gTransmission = new TGraph(nPoints, thickness, transmission);
    gTransmission->SetTitle("Transmisi#acute{o}n vs Espesor");
    gTransmission->SetMarkerStyle(20);
    gTransmission->SetMarkerColor(kRed);
    gTransmission->SetMarkerSize(1.2);
    gTransmission->GetXaxis()->SetTitle("Espesor (cm)");
    gTransmission->GetYaxis()->SetTitle("Transmisi#acute{o}n (I/I_{0})");
    gTransmission->GetXaxis()->SetRangeUser(0, 16);
    gTransmission->GetYaxis()->SetRangeUser(0, 1.05);
    gTransmission->Draw("AP");
    
    // Agregar curva teórica
    TGraph *gTransmissionTheory = new TGraph(100, thickness_th, transmission_th);
    gTransmissionTheory->SetLineColor(kBlue);
    gTransmissionTheory->SetLineWidth(2);
    gTransmissionTheory->SetLineStyle(2);
    gTransmissionTheory->Draw("L");
    
    // Leyenda
    TLegend *leg1 = new TLegend(0.6, 0.7, 0.88, 0.85);
    leg1->AddEntry(gTransmission, "Simulaci#acute{o}n GEANT4", "p");
    leg1->AddEntry(gTransmissionTheory, Form("Te#acute{o}rico (#mu=%.3f cm^{-1})", mu_teorico), "l");
    leg1->SetBorderSize(0);
    leg1->Draw();
    
    // Panel 2: ln(I/I₀) vs Espesor (debe ser línea recta)
    c1->cd(2);
    
    // Crear gráfico de ln(transmisión) vs espesor
    TGraph *gLnTransmission = new TGraph(nPoints, thickness, lnTransmission);
    gLnTransmission->SetTitle("ln(I/I_{0}) vs Espesor");
    gLnTransmission->SetMarkerStyle(20);
    gLnTransmission->SetMarkerColor(kRed);
    gLnTransmission->SetMarkerSize(1.2);
    gLnTransmission->GetXaxis()->SetTitle("Espesor (cm)");
    gLnTransmission->GetYaxis()->SetTitle("ln(I/I_{0})");
    gLnTransmission->GetXaxis()->SetRangeUser(0, 16);
    gLnTransmission->Draw("AP");
    
    // Ajuste lineal (Beer-Lambert: ln(I/I₀) = -μx)
    TF1 *fitBeer = new TF1("fitBeer", "[0]*x", 0, 16);
    fitBeer->SetParameter(0, -0.03); // Estimación inicial
    gLnTransmission->Fit("fitBeer", "R");
    
    // Obtener coeficiente de atenuación del ajuste
    double mu_fit = -fitBeer->GetParameter(0); // El signo negativo porque ln(I/I₀) = -μx
    
    // Leyenda
    TLegend *leg2 = new TLegend(0.15, 0.2, 0.50, 0.35);
    leg2->AddEntry(gLnTransmission, "Datos de simulaci#acute{o}n", "p");
    leg2->AddEntry(fitBeer, Form("Ajuste: #mu = %.4f cm^{-1}", mu_fit), "l");
    leg2->SetBorderSize(0);
    leg2->Draw();
    
    // Panel 3: Coeficiente de atenuación vs Espesor (debería ser constante)
    c1->cd(3);
    
    // Crear gráfico de coeficiente vs espesor
    TGraph *gCoeff = new TGraph(nPoints, thickness, attCoeff);
    gCoeff->SetTitle("Coeficiente de Atenuaci#acute{o}n vs Espesor");
    gCoeff->SetMarkerStyle(20);
    gCoeff->SetMarkerColor(kRed);
    gCoeff->SetMarkerSize(1.2);
    gCoeff->GetXaxis()->SetTitle("Espesor (cm)");
    gCoeff->GetYaxis()->SetTitle("Coeficiente de atenuaci#acute{o}n (cm^{-1})");
    gCoeff->GetXaxis()->SetRangeUser(0, 16);
    gCoeff->GetYaxis()->SetRangeUser(0, 0.15);
    gCoeff->Draw("AP");
    
    // Línea de referencia teórica
    TLine *line = new TLine(0, mu_teorico, 16, mu_teorico);
    line->SetLineColor(kBlue);
    line->SetLineWidth(2);
    line->SetLineStyle(2);
    line->Draw();
    
    // Línea de valor ajustado
    TLine *line_fit = new TLine(0, mu_fit, 16, mu_fit);
    line_fit->SetLineColor(kGreen+2);
    line_fit->SetLineWidth(2);
    line_fit->Draw();
    
    // Leyenda
    TLegend *leg3 = new TLegend(0.15, 0.7, 0.55, 0.85);
    leg3->AddEntry(gCoeff, "Valores individuales", "p");
    leg3->AddEntry(line, Form("Te#acute{o}rico (#mu=%.3f cm^{-1})", mu_teorico), "l");
    leg3->AddEntry(line_fit, Form("Ajuste global (#mu=%.4f cm^{-1})", mu_fit), "l");
    leg3->SetBorderSize(0);
    leg3->Draw();
    
    // Panel 4: Resumen y texto
    c1->cd(4);
    
    // Marco para información
    TPaveText *pt = new TPaveText(0.05, 0.05, 0.95, 0.95);
    pt->SetBorderSize(1);
    pt->SetFillColor(19);
    pt->SetTextAlign(12);
    pt->SetTextSize(0.04);
    
    // Título
    pt->AddText("Estudio de la Ley de Beer-Lambert");
    pt->AddText(Form("Material: %s", material));
    pt->AddText("");
    
    // Análisis de resultados
    double average_mu = 0;
    for (int i = 0; i < nPoints; i++) {
        average_mu += attCoeff[i];
    }
    average_mu /= nPoints;
    
    pt->AddText(Form("Coef. atenuación promedio: %.4f cm^{-1}", average_mu));
    pt->AddText(Form("Coef. atenuación por ajuste: %.4f cm^{-1}", mu_fit));
    pt->AddText(Form("Coef. atenuación teórico: %.3f cm^{-1}", mu_teorico));
    pt->AddText("");
    pt->AddText(Form("Diferencia con teórico: %.1f%%", fabs(mu_fit-mu_teorico)/mu_teorico*100));
    pt->AddText("");
    
    // Interpretación
    pt->AddText("Interpretación física:");
    pt->AddText(Form("- Espesor de semirreducción: %.2f cm", log(2)/mu_fit));
    pt->AddText(Form("- Espesor de reducción 1/10: %.2f cm", log(10)/mu_fit));
    pt->AddText("");
    pt->AddText("La ley de Beer-Lambert establece que la");
    pt->AddText("intensidad del haz decrece exponencialmente:");
    pt->AddText("I/I_{0} = e^{-#mu·x}");
    pt->AddText("donde #mu es el coeficiente de atenuación lineal");
    
    pt->Draw();
    
    // Guardar resultados
    c1->Print("../results/thickness_study.png");
    c1->Print("../results/thickness_study.pdf");
    
    // Mostrar resumen en consola
    cout << "\n===== RESUMEN DE RESULTADOS =====\n" << endl;
    cout << "Material: " << material << endl;
    cout << "Coeficiente de atenuación promedio: " << average_mu << " cm^-1" << endl;
    cout << "Coeficiente de atenuación por ajuste: " << mu_fit << " cm^-1" << endl;
    cout << "Coeficiente de atenuación teórico: " << mu_teorico << " cm^-1" << endl;
    cout << "Diferencia con teórico: " << fabs(mu_fit-mu_teorico)/mu_teorico*100 << "%" << endl;
    cout << "Espesor de semirreducción: " << log(2)/mu_fit << " cm" << endl;
    cout << "Espesor de reducción 1/10: " << log(10)/mu_fit << " cm" << endl;
    
    // Generar archivo CSV de resultados
    ofstream outcsv("../results/thickness_study_results.csv");
    outcsv << "Espesor (cm),Transmisión,ln(Transmisión),Coef. Atenuación (cm^-1)\n";
    
    for (int i = 0; i < nPoints; i++) {
        outcsv << thickness[i] << "," 
               << transmission[i] << "," 
               << lnTransmission[i] << "," 
               << attCoeff[i] << "\n";
    }
    outcsv.close();
    
    cout << "\nResultados guardados en:" << endl;
    cout << "- ../results/thickness_study.png" << endl;
    cout << "- ../results/thickness_study.pdf" << endl;
    cout << "- ../results/thickness_study_results.csv" << endl;
}