// Script de ROOT para analizar datos de atenuación de gamma
// Uso: root -l analyze_attenuation.C

void analyze_attenuation(const char *filename = "gamma_attenuation_run0.root")
{

    // Configurar estilo
    gROOT->SetStyle("Plain");
    gStyle->SetOptStat(1111);
    gStyle->SetPalette(1);

    // Abrir archivo
    TFile *file = TFile::Open(filename);
    if (!file || file->IsZombie())
    {
        cout << "Error: No se puede abrir " << filename << endl;
        return;
    }

    cout << "Analizando archivo: " << filename << endl;

    // Obtener Tree y Histograma
    TTree *tree = (TTree *)file->Get("attenuationData");
    TH1F *hist = (TH1F *)file->Get("attenuationCoeff");

    if (!tree)
    {
        cout << "Error: No se encontró el Tree 'attenuationData'" << endl;
        file->Close();
        return;
    }

    // Variables para leer del Tree
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

    // Crear canvas
    TCanvas *c1 = new TCanvas("c1", "Analisis de Atenuacion Gamma", 1200, 800);
    c1->Divide(2, 2);

    // Panel 1: Información del run
    c1->cd(1);
    tree->GetEntry(0); // Leer primera entrada

    // Crear texto con información
    TLatex latex;
    latex.SetNDC();
    latex.SetTextSize(0.05);
    latex.DrawLatex(0.1, 0.9, Form("Run ID: %d", runID));
    latex.DrawLatex(0.1, 0.8, Form("Material: %s", material));
    latex.DrawLatex(0.1, 0.7, Form("Espesor: %.2f cm", thickness));
    latex.DrawLatex(0.1, 0.6, Form("Eventos totales: %d", totalEvents));
    latex.DrawLatex(0.1, 0.5, Form("Eventos transmitidos: %d", transmittedEvents));
    latex.DrawLatex(0.1, 0.4, Form("Razon de transmision: %.4f", transmissionRatio));
    latex.DrawLatex(0.1, 0.3, Form("Coef. atenuacion: %.3f cm^{-1}", attenuationCoeff));

    // Panel 2: Histograma de coeficientes (si existe)
    c1->cd(2);
    if (hist)
    {
        hist->SetLineColor(kBlue);
        hist->SetFillColor(kCyan - 10); // Color azul claro
        hist->SetTitle("Distribucion del Coeficiente de Atenuacion");
        hist->GetXaxis()->SetTitle("Coeficiente (cm^{-1})");
        hist->GetYaxis()->SetTitle("Frecuencia");
        hist->Draw();
    }
    else
    {
        latex.SetTextAlign(22);
        latex.DrawLatex(0.5, 0.5, "Histograma no disponible");
    }

    // Panel 3: Gráfica de transmisión vs espesor (si hay múltiples runs)
    c1->cd(3);
    Int_t nEntries = tree->GetEntries();
    if (nEntries > 1)
    {
        TGraph *graph = new TGraph();
        for (Int_t i = 0; i < nEntries; i++)
        {
            tree->GetEntry(i);
            graph->SetPoint(i, thickness, transmissionRatio);
        }
        graph->SetMarkerStyle(20);
        graph->SetMarkerColor(kRed);
        graph->SetTitle("Transmision vs Espesor");
        graph->GetXaxis()->SetTitle("Espesor (cm)");
        graph->GetYaxis()->SetTitle("Razon de Transmision");
        graph->Draw("AP");
    }
    else
    {
        latex.DrawLatex(0.5, 0.5, "Se necesitan multiples runs\npara esta grafica");
    }

    // Panel 4: Ley de Beer-Lambert teórica
    c1->cd(4);
    if (nEntries >= 1)
    {
        tree->GetEntry(0);
        Double_t mu = attenuationCoeff;

        TF1 *beer = new TF1("beer", "exp(-[0]*x)", 0, thickness * 2);
        beer->SetParameter(0, mu);
        beer->SetLineColor(kGreen);
        beer->SetLineWidth(2);
        beer->SetTitle("Ley de Beer-Lambert");
        beer->GetXaxis()->SetTitle("Espesor (cm)");
        beer->GetYaxis()->SetTitle("I/I_0");
        beer->Draw();

        // Agregar punto experimental
        TGraph *expPoint = new TGraph(1);
        expPoint->SetPoint(0, thickness, transmissionRatio);
        expPoint->SetMarkerStyle(20);
        expPoint->SetMarkerColor(kRed);
        expPoint->SetMarkerSize(1.5);
        expPoint->Draw("P same");

        TLegend *leg = new TLegend(0.6, 0.7, 0.9, 0.9);
        leg->AddEntry(beer, Form("Teorico (#mu=%.3f)", mu), "l");
        leg->AddEntry(expPoint, "Experimental", "p");
        leg->Draw();
    }

    // Guardar como imagen
    c1->SaveAs("attenuation_analysis.png");
    c1->SaveAs("attenuation_analysis.pdf");

    cout << "Analisis completado. Graficas guardadas como:" << endl;
    cout << "- attenuation_analysis.png" << endl;
    cout << "- attenuation_analysis.pdf" << endl;

    file->Close();
}

// Función para analizar múltiples archivos
void analyze_multiple_runs()
{
    cout << "Buscando archivos ROOT..." << endl;

    // Crear lista de archivos
    TSystemDirectory dir(".", ".");
    TList *files = dir.GetListOfFiles();

    TCanvas *c1 = new TCanvas("c1", "Analisis Multiple Runs", 1000, 700);
    TGraph *transmissionGraph = new TGraph();
    TGraph *attenuationGraph = new TGraph();

    Int_t pointIndex = 0;

    if (files)
    {
        TSystemFile *file;
        TIter next(files);
        while ((file = (TSystemFile *)next()))
        {
            TString filename = file->GetName();
            if (filename.Contains("gamma_attenuation_run") && filename.EndsWith(".root"))
            {
                cout << "Procesando: " << filename << endl;

                TFile *rootFile = TFile::Open(filename);
                if (rootFile && !rootFile->IsZombie())
                {
                    TTree *tree = (TTree *)rootFile->Get("attenuationData");
                    if (tree && tree->GetEntries() > 0)
                    {
                        Float_t thickness, transmissionRatio, attenuationCoeff;
                        tree->SetBranchAddress("thickness", &thickness);
                        tree->SetBranchAddress("transmissionRatio", &transmissionRatio);
                        tree->SetBranchAddress("attenuationCoeff", &attenuationCoeff);

                        tree->GetEntry(0);
                        transmissionGraph->SetPoint(pointIndex, thickness, transmissionRatio);
                        attenuationGraph->SetPoint(pointIndex, thickness, attenuationCoeff);
                        pointIndex++;
                    }
                    rootFile->Close();
                }
            }
        }
    }

    if (pointIndex > 0)
    {
        c1->Divide(1, 2);

        c1->cd(1);
        transmissionGraph->SetMarkerStyle(20);
        transmissionGraph->SetMarkerColor(kBlue);
        transmissionGraph->SetTitle("Transmision vs Espesor");
        transmissionGraph->GetXaxis()->SetTitle("Espesor (cm)");
        transmissionGraph->GetYaxis()->SetTitle("Razon de Transmision");
        transmissionGraph->Draw("AP");

        c1->cd(2);
        attenuationGraph->SetMarkerStyle(20);
        attenuationGraph->SetMarkerColor(kRed);
        attenuationGraph->SetTitle("Coeficiente de Atenuacion vs Espesor");
        attenuationGraph->GetXaxis()->SetTitle("Espesor (cm)");
        attenuationGraph->GetYaxis()->SetTitle("Coeficiente (cm^{-1})");
        attenuationGraph->Draw("AP");

        c1->SaveAs("multiple_runs_analysis.png");
        cout << "Analisis multiple guardado como: multiple_runs_analysis.png" << endl;
    }
    else
    {
        cout << "No se encontraron archivos ROOT validos." << endl;
    }
}