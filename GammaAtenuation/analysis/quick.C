// Script ROOT simple - un comando y listo
void quick()
{

    // Abrir archivo y obtener datos
    TFile *file = TFile::Open("data_run0.root");
    if (!file)
    {
        cout << "Error: No se puede abrir el archivo ROOT" << endl;
        return;
    }

    TTree *tree = (TTree *)file->Get("data");
    if (!tree)
    {
        cout << "Error: No se encontraron datos" << endl;
        return;
    } // Leer los datos
    Int_t runID, totalEvents, transmittedEvents;
    Char_t material[50];
    Float_t thickness, transmissionRatio, attenuationCoeff;

    tree->SetBranchAddress("runID", &runID);
    tree->SetBranchAddress("material", material);
    tree->SetBranchAddress("thickness", &thickness);
    tree->SetBranchAddress("totalEvents", &totalEvents);
    tree->SetBranchAddress("transmittedEvents", &transmittedEvents);
    tree->SetBranchAddress("transmissionRatio", &transmissionRatio);
    tree->SetBranchAddress("attenuationCoeff", &attenuationCoeff);

    tree->GetEntry(0);

    // Mostrar resultados
    cout << "\n========== RESULTADOS ==========" << endl;
    cout << "Material: " << material << endl;
    cout << "Espesor: " << thickness << " cm" << endl;
    cout << "Eventos totales: " << totalEvents << endl;
    cout << "Eventos transmitidos: " << transmittedEvents << endl;
    cout << "Transmision: " << transmissionRatio << endl;
    cout << "Coef. atenuacion: " << attenuationCoeff << " cm^-1" << endl;
    cout << "==============================\n"
         << endl;

    // Crear gráficas simples
    TCanvas *c = new TCanvas("c", "Resultados", 800, 400);
    c->Divide(2, 1);

    // Panel 1: Info
    c->cd(1);
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.08);
    latex.DrawLatex(0.1, 0.9, Form("Material: %s", material));
    latex.DrawLatex(0.1, 0.8, Form("Espesor: %.1f cm", thickness));
    latex.DrawLatex(0.1, 0.7, Form("Eventos: %d", totalEvents));
    latex.DrawLatex(0.1, 0.6, Form("Transmitidos: %d", transmittedEvents));
    latex.DrawLatex(0.1, 0.5, Form("Transmision: %.4f", transmissionRatio));
    latex.DrawLatex(0.1, 0.4, Form("μ = %.2f cm^{-1}", attenuationCoeff));

    // Panel 2: Gráfica Beer-Lambert
    c->cd(2);
    TF1 *beer = new TF1("beer", "exp(-[0]*x)", 0, thickness * 2);
    beer->SetParameter(0, attenuationCoeff < 100 ? attenuationCoeff : 1.0);
    beer->SetLineColor(kBlue);
    beer->SetLineWidth(2);
    beer->SetTitle("Ley Beer-Lambert;Espesor (cm);I/I_0");
    beer->Draw();

    // Punto experimental
    TGraph *point = new TGraph(1);
    point->SetPoint(0, thickness, transmissionRatio);
    point->SetMarkerStyle(20);
    point->SetMarkerColor(kRed);
    point->SetMarkerSize(2);
    point->Draw("P same");

    TLegend *leg = new TLegend(0.6, 0.8, 0.9, 0.9);
    leg->AddEntry(beer, "Teorico", "l");
    leg->AddEntry(point, "Experimental", "p");
    leg->Draw();

    c->Draw();

    file->Close();
    cout << "¡Listo! Gráficas mostradas." << endl;
    cout << "Las gráficas permanecen abiertas." << endl;
    cout << "Escribe '.q' para salir de ROOT." << endl;
}