#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <list>
#include <cstdint>
#include <climits>
using namespace std;
bool breadtFirstSearch(int **residualGraph, int s, int t, int source[],int V)
{

	bool *visited = new bool[V];
	for (int i = 0; i < V; i++) visited[i] = false;
	queue <int> nodes;
	nodes.push(s);
	visited[s] = true;
	source[s] = -1;

	while (!nodes.empty())
	{
		int u = nodes.front();
		nodes.pop();

		for (int v = 0; v < V; v++)
		{
			if (visited[v] == false && residualGraph[u][v] > 0)
			{
				nodes.push(v);
				source[v] = u;
				visited[v] = true;
			}
		}
	}
	if (visited[t] == true)
	{
		return true;
	}

	return false;
}

int fordFulkerson(int **graph, int s, int t,int V)
{
	int u, v;
	int **residualGraph = new int*[V];
	for (int u = 0; u < V; u++) residualGraph[u] = new int[V];

	
	for (u = 0; u < V; u++)
		for (v = 0; v < V; v++)
			residualGraph[u][v] = graph[u][v];

	int *source = new int[V];  

	int maxFlow = 0;  
	bool flag = false;

	while (breadtFirstSearch(residualGraph, s, t, source,V)  == true)
	{
		int pathFlow = INT_MAX;
		for (v = t; v != s; v = source[v])
		{
			u = source[v];
			pathFlow = min(pathFlow, residualGraph[u][v]);
		}
		for (v = t; v != s; v = source[v])
		{
			u = source[v];
			residualGraph[v][u] += pathFlow;
			residualGraph[u][v] -= pathFlow;
		}

		maxFlow += pathFlow;

	}
	//cout<< "max flow: "<< maxFlow <<endl;
	return maxFlow;
}

queue<int> Graph(vector<pair<int, int>> team, vector<pair<string, int>> couples,int subMatches, queue <int> result)
{
	int n = team.size() + couples.size() + 2;
	int** adj = new int*[n];
	//cout <<"n: "<< n <<endl;
	for (int i = 0; i < n; i++)
	{
		adj[i] = new int[n];
	}
	
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			adj[i][j] = 0;
		}
	}

	for (int j = 1; j < couples.size() + 1; j++)
	{
		adj[0][j] = couples[j-1].second;
	}

	for (int j = couples.size()+1; j < n - 1; j++)
	{
		adj[j][n - 1] = team[j - couples.size()-1].second;
	}

	for (int j = 1; j < couples.size() + 1; j++)
	{
		size_t find = couples[j - 1].first.find('-');
		int firstPart = atoi(couples[j - 1].first.substr(0, find).c_str());
		int secondPart = atoi(couples[j - 1].first.substr(find+1).c_str());
		
		for (int i = 0; i < team.size(); i++)
		{
			if (team[i].first == firstPart)
			{
				adj[j][couples.size() + i+1] = couples[j - 1].second;
			}
			if (team[i].first == secondPart)
			{
				adj[j][couples.size() + i+1] = couples[j - 1].second;
			}
		}
	}

	/*for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			cout << adj[i][j] << ", ";
		}
		cout << endl;
	}*/

	int max_flow = fordFulkerson(adj, 0, n - 1, n);
	/*cout << "Sub matches:" << subMatches << endl;*/
	if (max_flow == subMatches)
	{
		//cout << "1" << endl;
		result.push(1);
	}
	else
	{
		//cout << "0" << endl;
		result.push(0);
	}

	return result;
}

queue<string> readTxt(string textName) // Read TXT File and Return All Characters in Queue
{
	queue<string> txtFile;
	ifstream file;
	string str;	
	file.open(textName);
	while (getline(file, str)) {
		txtFile.push(str);
		/*cout << str << endl;*/
	}

	file.close();
	return txtFile;
}

int main(int argc, char* argv[])
{	
	queue<string> txtFile;
	queue<string> tempFile;
	queue<int>result;

	txtFile = readTxt(argv[1]);
	tempFile = txtFile;

	int teamNumber = atoi(txtFile.front().c_str());
	txtFile.pop();

	int* scores = new int[teamNumber];
	int* sumRemaining = new int[teamNumber];
	int** remainingMatches = new int*[teamNumber];
	int* winningChance = new int[teamNumber];

	for (int i = 0; i < teamNumber; i++)
	{
		remainingMatches[i] = new int[teamNumber];
	}

	tempFile.pop();
	tempFile.pop();
	int line = 0;
	while (!tempFile.empty())
	{
		int column = 0;
		string str = tempFile.front();
		for (int i = 0; i < str.length(); i++)
		{
			if (str[i] != ' ')
			{
				remainingMatches[line][column] = str[i] - '0';
				column++;
			}
		}
		tempFile.pop();
		line++;
	}

	for (int i = 0; i < teamNumber; i++)
	{
		sumRemaining[i] = 0;
	}

	string temp = txtFile.front();

	int counter = 0;
	for (int i = 0; i < temp.length(); i++)
	{
		if (temp[i] != ' ')
		{
			scores[counter] = temp[i] - '0';
			counter++;
		}
	}
	txtFile.pop();

	counter = 0;
	while (!txtFile.empty()) {
		string str = txtFile.front();
		for (int i = 0; i < str.length(); i++)
		{
			if(str[i] != ' ') sumRemaining[counter] += str[i] - '0';
		}
		txtFile.pop();
		counter++;
	}

	/*for (int i = 0; i < teamNumber; i++)
	{
		cout << scores[i] << " ";
	}
	cout << endl;

	for (int i = 0; i < teamNumber; i++)
	{
		cout << sumRemaining[i] << " ";
	}
	cout << endl;

	for (int i = 0; i < teamNumber; i++)
	{
		for (int j = 0; j < teamNumber; j++)
		{
			cout << remainingMatches[i][j] << " ";
		}
		cout << endl;
	}*/
	
	for (int k = 0; k < teamNumber; k++)
	{
		int subMatches = 0;
		vector<pair<string, int>> couples;
		vector<pair<int, int>> team;
		for (int i = 0; i < teamNumber; i++)
		{
			for (int j = i; j < teamNumber; j++)
			{
				if (remainingMatches[i][j] != 0 && k!=i && k!=j)
				{
					string coName = to_string(i) + '-' + to_string(j);
					couples.emplace_back(coName, remainingMatches[i][j]);
				}
			}
		}

		/*for (int i = 0; i < couples.size(); i++)
		{
			cout << couples[i].first << " " << couples[i].second << endl;
		}*/

		int *sumTemp = new int[teamNumber];
		for (int i = 0; i < teamNumber; i++)
		{
			sumTemp[i] = sumRemaining[i] - remainingMatches[i][k];
		}

		int max = scores[k] + sumRemaining[k];

		for (int i = 0; i < teamNumber; i++)
		{
			if (i != k && (sumTemp[i] != 0))
			{
				team.emplace_back(i, max- scores[i]); //max-scores[i]
			}
		}


		/*for (int i = 0; i < team.size(); i++)
		{
			cout << team[i].first << " " << team[i].second << endl;
		}*/
		for (int i = 0; i < teamNumber; i++)
		{
			for (int j = i; j < teamNumber; j++)
			{
				if (i != k && j != k)
				{
					subMatches += remainingMatches[i][j];
				}
			}
		}
		result = Graph(team, couples,subMatches,result);
		//getchar(); /*Creta Graph*/
	}
	
	string output = "";
	while (!result.empty())
	{
		if (result.size() == 1)
		{
			output += to_string(result.front());
			result.pop();
		}
		else
		{
			output += to_string(result.front()) + " ";
			result.pop();
		}
	}

	string arg1(argv[1]);

	arg1.replace(0,2, "out");
	string filename = arg1;
	ofstream file1(filename);
	if (file1.fail())
	{
		cerr << "!Error opening " << filename << endl;
		return EXIT_FAILURE;
	}

	file1 << output << endl;

	return 1;
}