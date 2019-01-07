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

bool isInteger(double number)
{
	return ceil(number)==floor(number);
}

double rightDistance(double starter)
{
	int cnt=0;
	while(!isInteger(starter))
	{
		starter *= 10;
		cnt++;
	}
	return 2/ pow(10, cnt);
}

int main() {

	string nomeFile;
	string line;
	int bins;
	double epsilon;
	vector<double> dati;
	cout << "~~~~~~GRAFICATORE~~~~~~\n\n";
	cout << "Inserisci il nome del file da cui leggere i dati:\n";
	cin >> nomeFile;
	
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
		
		cout << "Numero bins, -1 per predefinito (regola di Sturge):\n";
		cin >> bins;
		if(bins<1)
			bins = ceil(1 + log10(dati.size())/log10(2)); //regola di Sturge
		
		epsilon = max(rightDistance(minimo), rightDistance(massimo));
		
		ofstream script;
		script.open("Istogramma.gplot");
		script << "dimC="<< dati.size() << "\n";
		script << "eqm="<< eqm << "\n";
		script << "min="<< minimo << "\n";
		script << "max="<< massimo << "\n";
		script << "media="<< media << "\n";
		script << "n="<< bins << "\n";
		script << "width=(max-min)/n \n";
		script << "hist(x,width)=width*floor(x/width)+width/2.0\n" <<
		"set terminal png\nset output \"grafico.png\"\n" <<
		"set xrange [min-"<< epsilon <<":max+" <<epsilon <<"]\nset autoscale y\n" <<
		"set xlabel sprintf(\"Valore; #bins:\%d, width:\%.5f \", n, width)\nset ylabel \"Frequenza\"\n" <<
		"set boxwidth width\nset style fill solid 0.5\n" <<
		"set grid ytics lt 0 lw 1 lc rgb \"#bbbbbb\"\nset grid xtics lt 0 lw 1 lc rgb \"#bbbbbb\"\n" <<
		"plot \""<< nomeFile <<"\" u (hist($1,width)):(1.0) smooth freq w boxes lc rgb\"green\" notitle, ((dimC*width)/(eqm*sqrt(2*pi)))*exp(-0.5*((x-media)/eqm)**2) title \"Gaussiana\" with lines linestyle 1";
		script.close();
		
		system("gnuplot Istogramma.gplot");
		//system("rm Istogramma.gplot");
  	}
  	else 
  	{
  		cout << "Errore I/O\n"; 
  	}

  	return 0;
}
