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

int buracos(vector<vector<int>> SH) {

	int contador = 0;
	for (auto s : SH) {
		for (int i = 0; i < s.size() - 1; i++) {
			if (s[i] == -1 && s[i + 1] != -1) {
				contador++;
			}
		}
	}

	return contador;
}



int main()
{
	// Criando o ambiente
	IloEnv env;
	IloModel Model(env, "Pasa");
	auto start = chrono::high_resolution_clock::now();

	try
	{

#pragma region Defini��o  dos Dados


		ProblemData::nT = 140;
		ProblemData::nS = 15;
		ProblemData::nH = 80;


		ProblemData::seed = 40;
		RandLib RL(ProblemData::seed);
		ProblemData::Populate_Parameters(RL);

#pragma endregion

#pragma region Recep��o dos Dados

		int nS = ProblemData::nS;
		int nT = ProblemData::nT;
		int nH = ProblemData::nH;
		int seed = ProblemData::seed;


		int** HT = ProblemData::HT;
		std::vector<std::vector<int>>CH = ProblemData::CH;


#pragma endregion

#pragma region Vari�vel de decis�o
		NumVar2D x(env, nS);
		NumVar2D y(env, nS);
		NumVar2D z(env, nS);

		for (int s = 0; s < nS; s++) {
			x[s] = IloNumVarArray(env, nT, 0, 1, ILOINT);
			y[s] = IloNumVarArray(env, nH - 1, 0, 1, ILOINT);
			z[s] = IloNumVarArray(env, nH, 0, 1, ILOINT);
		}

#pragma endregion

#pragma region Fun��o Objetivo

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
				if (CH[h][0] == -1) {
					Model.add(0 == z[s][h]);
				}
				else {
					IloExpr exp3(env);
					for (int t = 0; t < CH[h].size(); t++)
					{
						exp3 += x[s][CH[h][t]];
					}
					// -1 � a flag pra quando n�o existe nenhuma aula naquele hor�rio 
					Model.add(exp3 == z[s][h]);
				}


			}
		}


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
		//cplex.setOut(env.getNullStream());
		cplex.setParam(IloCplex::TiLim, 420);

		//cplex.exportModel("tabela_horarios.lp");
		if (!cplex.solve()) {
			//env.error() << "Falhou ao otmizar o problema" << endl;
			//throw(-1);
			cout << cplex.getStatus() << endl;

		}

		double obj = cplex.getObjValue();
		auto end = chrono::high_resolution_clock::now();
		auto Elapsed = chrono::duration_cast<chrono::milliseconds>(end - start);
		cout << "\t Elapsed Time(ms): " << Elapsed.count() << endl;
		cout << "\n\n\tCusto otimo " << obj << endl;

#pragma endregion

#pragma region Arquivo de Sa�da


		ofstream fid;
		string FileName = "Pasa_" + std::to_string(nT) + "_" + std::to_string(nS) + "_" + std::to_string(nH - 1) + ".txt";

		fid.open(FileName);
		fid << "\n\n\n\tProblem Size (nT x nS x nH):" << nT << "x" << nS << "x" << nH - 1 << endl;
		fid << "\tCPU Time(ms): " << Elapsed.count() / 100 << endl;
		fid << "\tObj Value: " << obj << endl << endl;


		fid << "\t\t\t" << "Turmas x Horários" << " " << endl;

		for (int t = 0; t < nT; t++)
		{
			for (int h = 0; h < nH; h++)
			{

				fid << "\t\t\t" << HT[h][t] << " ";

			}
			fid << endl;
		}
		fid << endl << endl << endl << endl;

		fid << "\t\t\t" << "Salas x horários" << endl << endl << endl;

		for (int s = 0; s < nS; s++)
		{
			for (int h = 0; h < nH; h++)
			{
				double zval = cplex.getValue(z[s][h]);
				fid << "\t\t\t" << zval << " ";

			}
			fid << endl;
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

		fid.close();

		ofstream fid2;
		fid2.open("TP_Results.txt", std::ios::app);
		fid2 << "\n\t" << nT;
		fid2 << "-" << nS;
		fid2 << "-" << nH - 1;
		fid2 << "-" << ProblemData::seed;

		fid2 << "\t\t" << obj;
		fid2 << "\t" << Elapsed.count() / 100;
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
	return 0;
}

