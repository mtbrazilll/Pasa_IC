#include"TP_Data.h"




int ProblemData::nS;
int ProblemData::nT;
int ProblemData::nH;
int ProblemData::seed;

std::vector<std::vector<int>>  ProblemData::CH;
int** ProblemData::HT;



void ProblemData::Populate_Parameters(RandLib RL)
{

	nH++; // hor�rio virtual
	std::vector<int> filaTurmas;
	HT = new int* [nH](); // matriz hor�rios x turmas

	std::vector<int> v = { -1 }; // ch[0] = {-1}
	CH.push_back(v);


	// inicializa a matriz binária horários x turmas
	for (int h = 0; h < nH; h++)
	{
		HT[h] = new int[nT];
		for (int t = 0; t < nT; t++)
		{
			HT[h][t] = 0;
		}
	}

	// gera uma fila de turmas 
	for (int t = 0; t < nT; t++)
	{
		int qnt = RL.randint(1, 4);// quantidadae de vezes que turma vai ficar na fila
		for (int i = 0; i < qnt; i++)
		{
			filaTurmas.push_back(t); 
		}
	}



	int h = 1; 

	// enquanto tiver turma não alocada  
	while (!filaTurmas.empty()) {

		int aux, j1, j2;
		int n = filaTurmas.size();

		//embaralha a fila de turmas
		if (filaTurmas.size() > 1) {
			for (int i = 0; i < n; i++) {
				j1 = RL.randint(0, n - 1);
				j2 = RL.randint(0, n - 1);
				while (j1 == j2) j2 = RL.randint(0, n - 1);
				aux = filaTurmas[j1];
				filaTurmas[j1] = filaTurmas[j2];
				filaTurmas[j2] = aux;
			}
		}


		if (h > nH - 1) h = h % nH; // h fica preso entre [0,nH-1]
		if (h == 0) h = 1; // não existe aula no horário h = 0
		int escolha = RL.randint(2, nS / 3); // quantidade de turmas alocadas no horário h

		for (int i = 0; i < escolha; i++)
		{
			int t = filaTurmas[filaTurmas.size() - 1];
			filaTurmas.pop_back();
			HT[h][t] = 1;
			if (filaTurmas.empty()) break;
		}
		h++;
	}



	for (int h = 1; h < nH; h++)
	{
		std::vector<int> v;
		for (int t = 0; t < nT; t++)
		{
			if (HT[h][t] == 1) {
				v.push_back(t);
			}
		}
		if (!v.empty()) {
			CH.push_back(v);
		}
		else {
			v.push_back(-1);
			CH.push_back(v);
		}
	}
}

/*
void ProblemData::Populate_Parameters(RandLib RL)
{


	HT = new int* [nH]();
	nH++;

	HT[0] = new int[nT];
	for (int t = 0; t < nT; t++)
	{
		HT[0][t] = 0;
	}


	std::vector<int> v = { -1 };
	CH.push_back(v);

	for (int h = 1; h < nH; h++)
	{
		HT[h] = new int[nT];
		std::vector<int> v;
		for (int t = 0; t < nT; t++)
		{
			HT[h][t] = RL.randint(0, 1);
			if (HT[h][t] == 1)
			{
				v.push_back(t);
			}
		}
		if (!v.empty()) {
			CH.push_back(v);
		}
		else {
			v.push_back(-1);
			CH.push_back(v);
		}
	}

}
*/