#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <cstdlib>

using namespace std;

double sommaElementi(const vector<double> &vect) 
{
	double sum=0;
	for(int i=0; i<vect.size(); i++)
		sum += vect.at(i);  
	return sum;
}

double calcolaMedia(const vector<double> &vect) 
{
	return sommaElementi(vect)/vect.size();
}

double sqs(const vector<double> &vect) //somma quadrati scarti da media
{
	double SqScarti=0;
	double media=calcolaMedia(vect);
    	for(int i=0; i<vect.size(); i++)
    	{
		SqScarti += pow((vect.at(i)-media), 2);
    	}
	return SqScarti;
}

void creaTabella(const vector<double> &vect, int col)
{
	int righe = (vect.size() / col)+1;
		
	ofstream file;
	file.precision(9);
	file.open("TeXTabella.txt");
	file << "\\begin{tabular}{";
	for(int i=0; i<col; i++)
		file << "|c|c|";
	file << "}\n";
	
	for(int i=0; i<col; i++)
	{
		file << " Misura & Valore ";
		if(i!=col-1)
			file << "&";
		else
			file << " \\\\ \\hline \n";
	}
	
	for(int i=0; i<righe; i++)
	{
		for(int j=0; j<col; j++)
		{
			int point = i+righe*j;
			if(point>=vect.size())
				file << " " << " & " << " ";
			else
				file << point+1 << " & " << vect.at(point);
				
			if(j!=col-1)
				file << " & ";
			else
				file << " \\\\ \\hline \n";
		}
	}
	file << "\\end{tabular}";

}

int main() {

	string nomeFile;
	string line;
	int cTabella;
	vector<double> dati;
	cout << "~~~~~~ELABORATORE DATI, CREA TABELLA~~~~~~\n\n";
	cout << "Inserisci il nome del file da cui leggere i dati:\n";
	cin >> nomeFile;

	cout << "numero colonne Tabella:\n";
	cin >> cTabella;
	
	ifstream dataset(nomeFile);
	if (dataset.is_open())
	{
	    while(getline(dataset,line))
	    {
			dati.push_back(stod(line));
	    }
		dataset.close();
		
		double media = calcolaMedia(dati);
		double sqm = sqrt( sqs(dati)/dati.size() ); //scarto quad. medio
		double eqm = sqrt( sqs(dati)/(dati.size()-1) ); //errore quadratico medio
    	double errdm = eqm / sqrt(dati.size());	//errore della media
    	
    	double minimo = *min_element(dati.begin(), dati.end());
    	double massimo = *max_element(dati.begin(), dati.end());

 		printf("~~~~~~Analisi dati:~~~~~~\n");
		printf("%-15s %8s %8s %8s %8s %8s %8s\n", "dimensione", "massimo", "minimo", "media", "ErrM", "SQM", "ErrQM");

		printf("%-15lu %8f %8f %8f %8f %8f %8f\n", dati.size(), massimo, minimo, media, errdm, sqm, eqm);
		
		ofstream report;
		report.open("report.txt");
		
		report << "~~~~~~Analisi dati:~~~~~~\n\n" << "Dimensione: " << dati.size() << "\n" << "Massimo: " << massimo << "\n" 
		<< "Minimo: " << minimo << "\n" << "Media: " << media << "\n" << "Errore Media: " << errdm << "\n" << "ErroreQuadMedio: " << eqm << "\n"
		<< "ScartoQuadMedio: " << sqm << "\n";
		
		report << "TeX:\n\n";
		report << "\\begin{tabular}{cc}\n\\toprule\n";
		report << "$N$ & "<< dati.size() <<" \\\\ \\hline\n";
		report << "min & "<< minimo <<" \\\\ \\hline\n";
		report << "max & "<< massimo <<" \\\\ \\hline\n";
		report << "$\\bar{A}$ & "<< media <<" \\\\ \\hline\n";
		report << "$\\sigma_{\\bar{A}}$ & "<< errdm <<" \\\\ \\hline\n";
		report << "$\\sigma_{A}$ & "<< eqm <<" \\\\ \\hline\n";
		report << "$S$ & "<< sqm <<" \\\\ \n";
		report << "\\bottomrule\n";
		report << "\\end{tabular}\n";
		
		report.close();
		
		creaTabella(dati, cTabella);
		
  	}
  	else 
  	{
  		cout << "Errore I/O\n"; 
  	}

  	return 0;
}
