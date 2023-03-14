#include "ilcplex/ilocplex.h";  
#include <iostream>
#include<fstream>;
#include<chrono>;
#include<random>;
#include "RandGen.h"
#include"TP_Data.h"


using namespace std;
typedef IloArray<IloNumVarArray> NumVar2D;
typedef IloArray<NumVar2D> NumVar3D;
#define all(x) (x).begin(), (x).end()


void cplex(int nT, int nS, int nH, int** HT, vector<vector<int>> CH) {

	// Criando o ambiente
	IloEnv env;
	IloModel Model(env, "Pasa");
	auto start = chrono::high_resolution_clock::now();

	try
	{

#pragma endregion

#pragma endregion

#pragma region Variavel de decisao

		NumVar2D x(env, nS);
		NumVar2D y(env, nS);
		NumVar2D z(env, nS);

		for (int s = 0; s < nS; s++) {
			x[s] = IloNumVarArray(env, nT, 0, 1, ILOINT);
			y[s] = IloNumVarArray(env, nH - 1, 0, 1, ILOINT);
			z[s] = IloNumVarArray(env, nH, 0, 1, ILOINT);
		}

#pragma endregion

#pragma region Funcao Objetivo

		IloExpr exp0(env);

		for (int s = 0; s < nS; s++)
		{
			for (int h = 0; h < nH - 1; h++)
			{
				exp0 += y[s][h];
			}

		}
		Model.add(IloMinimize(env, exp0));

#pragma endregion

#pragma region Restri��es

		//toda turma deve ser alocada � sala de aula
		for (int t = 0; t < nT; t++)
		{
			IloExpr exp1(env);
			for (int s = 0; s < nS; s++)
			{
				exp1 += x[s][t];
			}

			Model.add(exp1 == 1);
		}

		// zsh, diz se a sala est� sendo usada no horario i
		for (int s = 0; s < nS; s++)
		{
			for (int h = 0; h < nH; h++) // dado um horario i, se tem um conjunto de aulas 
			{
				if (CH[h][0] == -1) { // flag para quando n�o conflito de turmas no hor�rio h
					Model.add(0 == z[s][h]);
				}
				else {
					IloExpr exp2(env);
					for (int t = 0; t < CH[h].size(); t++)
					{
						exp2 += x[s][CH[h][t]]; // s� uma dessas pode t� na sala s;
					}
					Model.add(exp2 == z[s][h]);
				}


			}
		}

		// rela��o entre a var�avel z e y
		for (int s = 0; s < nS; s++)
		{
			for (int h = 0; h < nH - 1; h++)
			{
				IloExpr exp4(env);
				int _h = h + 1;
				exp4 = z[s][_h] - z[s][h];
				Model.add(exp4 <= y[s][h]);
			}
		}

#pragma endregion

#pragma region solver
		IloCplex cplex(Model);
		cplex.setOut(env.getNullStream());
		cplex.setParam(IloCplex::TiLim, 120);

		if (!cplex.solve()) {

			cout << cplex.getStatus() << endl;

		}

		double obj = cplex.getObjValue();
		auto end = chrono::high_resolution_clock::now();
		auto Elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
		cout << "\t Elapsed Time(ms): " << Elapsed.count() << endl;
		cout << "\n\n\tCusto otimo " << obj << endl;

#pragma endregion

#pragma region Arquivo de Saida


		ofstream fid;
		ofstream fid2;
		

		string FileName = "Pasa_" + std::to_string(nT) + "_" + std::to_string(nS) + "_" + std::to_string(nH) + ".txt";

		
		fid2.open("Pasa_out_SH.txt");
		fid.open(FileName);

		fid << "\n\n\n\tProblem Size (nT x nS x nH):" << nT << "x" << nS << "x" << nH << endl;
		fid << "\tCPU Time(ms): " << Elapsed.count() / 100 << endl;
		fid << "\tObj Value: " << obj << endl << endl;


		fid << "\t\t\t" << "Turmas x Horarios" << " " << endl;

		for (int t = 0; t < nT; t++)
		{
			for (int h = 0; h < nH; h++)
			{

				fid << "\t\t\t" << HT[h][t] << " ";

			}
			fid << endl;
		}
		fid << endl << endl << endl << endl;

		fid << "\t\t\t" << "Salas x horarios" << endl << endl << endl;
		for (int s = 0; s < nS; s++)
		{
			for (int h = 0; h < nH; h++)
			{
				double zval = cplex.getValue(z[s][h]);
				if (zval == 1.0) {
					fid << "\t\t\t" << 1 << " ";
					fid2  << 1 << " ";

				}
				else {
					fid << "\t\t\t" << 0 << " ";
					fid2  << 0 << " ";
				}
				

			}
			fid << endl;
			fid2 << endl;
		}
		fid << endl << endl;
		for (int s = 0; s < nS; s++)
		{
			for (int t = 0; t < nT; t++)
			{
				double Xval = cplex.getValue(x[s][t]);
				if (Xval > 0)
				{
					fid << "\t\t\t X[" << s << "][" << t << "] = " << Xval << endl;
				}
			}
		}


		fid << endl << endl;

		fid.close();
		
		fid2.close();

#pragma endregion


	}
	catch (const IloException& e)
	{
		cerr << "Exception caught: " << e << endl;
	}
	catch (...)
	{
		cerr << "Unknown exception caught!" << endl;
	}

	env.end();
	return;

}

int main()
{

	int aux;
	int nH;
	int nS;
	int nT;
	int** HT;
	ifstream pasa_HT;
	ifstream pasa_in;

	pasa_HT.open("Pasa_HT.txt");
	pasa_in.open("Pasa_in.txt");

	if (!pasa_HT.is_open() || !pasa_in.is_open()) {
		cout << "Erro ao abrir o arquivo.\n";
	}

	pasa_in >> nT >> nS >> nH;

	HT = new int* [nH](); // matriz horarios x turmas
	vector<vector<int>> CH;

	for (int h = 0; h < nH; h++)
	{
		HT[h] = new int[nT];
		vector<int> v;
		for (int t = 0; t < nT; t++)
		{
			pasa_HT >> aux;
			HT[h][t] = aux;

			if (aux == 1) {
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

	cplex(nT, nS, nH, HT, CH);

}

