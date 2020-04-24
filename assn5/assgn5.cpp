#include "assgn5.h"

//tokeniser
pair<string, int> tokenize(string str, int index, char delim)
{
	string token = "";
	if(index >= str.size())
		return (make_pair(token, -2));
	for( ; index < str.size(); index++)
	{
		if(str[index] == delim)
			return (make_pair(token, index+1));
		token += str[index];
	}
	return ( make_pair(token, -1)) ;
}

//function to check if it is terminal
int check_terminal(string a)
{
	for(int i=0; i< terminals.size(); i++)
		if(a == terminals[i])
			return 1;

	return 0;	
}

//recursive function to find first
void find_first(string non_terminal, int index)
{
	pair<string, int> token = tokenize(production[index], 0, '-');
	token.second++;
	int cnt = 1;
	do
	{
		token = tokenize(production[index], token.second, '|');
		
		int flag =0;
		pair<string, int> term = make_pair("", 0);
		do
		{
			term = tokenize(token.first, term.second, ' ');
			if(term.second == -2)
				break;
			if(check_terminal(term.first) || term.first == "Epsilon")
			{
				first1[non_terminal].push_back( make_pair( term.first, cnt));
				break;
			}
			else 
			{
				int pos;
				for(int i=0; i<non_terminals.size(); i++)
					if(term.first == non_terminals[i])
					{
						pos = i;
						break;
					}	
				if(!vis[term.first])
					find_first(term.first, pos);
				for(int i=0; i< first1[term.first].size(); i++)
				{

					flag = 0;
					if(first1[term.first][i].first != "Epsilon")
						first1[non_terminal].push_back( make_pair(first1[term.first][i].first, cnt) );
					else
					{
						flag =1;

						if(term.second == -1)
						{
							first1[non_terminal].push_back( make_pair("Epsilon", cnt));
							flag = 0;				
						}
					}
				}	
			}
			if(!flag)
				break;
		}while(term.second != -1);
		cnt++;	
	}while(token.second != -1);
		
	vis[non_terminal] = 1;
}

//function to check if element is not visited
bool notVisited(string a, string b)
{
	for(int i=0; i<follow[a].size(); i++)
		if(follow[a][i] == b)
			return false;

	return true;	
}


void findFollow(string non_terminal)
{
	
	for(int i=0; i<no_of_rules; i++)
	{
		pair<string, int> token = tokenize(production[i], 0, '-');
		string par= token.first;
		token.second++;

		do
		{
			token = tokenize(production[i], token.second, '|');
			if(token.first == "Epsilon")
				break;
			int flag = 0;
			pair<string, int> term = make_pair("", 0);
			do
			{
				
				term = tokenize(token.first, term.second, ' ');
				

				if(term.second == -2)
					break;
				if(check_terminal(term.first) && !flag)
					continue;
				else if(check_terminal(term.first) && flag)
				{
					if(notVisited(non_terminal, term.first))
						follow[non_terminal].push_back(term.first);
					break;
				}
				
				if(term.first == non_terminal)
				{
					
					if(term.second == -1)
					{
						
						if(par == non_terminal)
							break;
						if(!vis[par])
							findFollow(par);
						for(int k=0; k< follow[par].size(); k++)
							if(notVisited(non_terminal, follow[par][k]))
								follow[non_terminal].push_back(follow[par][k]);
						break;
					}
					else
						flag =1;
				}
				else if(flag)
				{
					flag=1;
					for(int ii=0; ii< first1[term.first].size(); ii++)
					{
						flag = 0;
						if(first1[term.first][ii].first != "Epsilon" && notVisited(non_terminal, first1[term.first][ii].first))
							follow[non_terminal].push_back( first1[term.first][ii].first );
						else
						{
							flag =1;
							
							if(term.second == -1)
							{
								if(!vis[par])
									findFollow(par);
								for(int k=0; k< follow[par].size(); k++)
									if(notVisited(non_terminal, follow[par][k]))
										follow[non_terminal].push_back(follow[par][k]);
								flag = 0;				
							}
						}
					}
				}
				

			}while(term.second != -1);
		
		}while(token.second != -1);	
			
	}
	
	vis[non_terminal] = 1;
}

string putEntry(int index, int x)
{
	
		pair<string, int> token = tokenize(production[index], 0, '-');
		token.second++;
		int cnt = 1;
		do
		{
			token = tokenize(production[index], token.second, '|');
			
			if(cnt == x)
				return token.first;
			cnt++;
		}while(token.second != -1);
	
		return NULL;
}

int main()
{
	ifstream file;
	ifstream file2;
	file.open("grammar.txt");
	
	string line;
	cout<<endl;

	//Reading from input file and store it
	while ( getline (file,line) ) 
	{
		production.push_back(line);
	}
	no_of_rules = production.size();

	pair<string, int> token;
	for(int i=0; i< no_of_rules; i++)
	{
		token = tokenize(production[i], 0, '-');
		non_terminals.push_back( token.first );
	}

	//storing all the terminals 
	for(int i=0; i< no_of_rules; i++)
	{
		pair<string, int> token = tokenize(production[i], 0, '-');
		token.second++;

		do
		{
			token = tokenize(production[i], token.second, '|');
			
			pair<string, int> term = make_pair("", 0);
			do
			{
				term = tokenize(token.first, term.second, ' ');
				if(term.second == -2)
					break;
				int f=0;
				for(int i=0; i<non_terminals.size(); i++)
					if(term.first == non_terminals[i])
					{
						f=1; 
						break;
					}	

				if(!f && term.first != "Epsilon" && !check_terminal(term.first))
					terminals.push_back(term.first);	

			}while(term.second != -1);
		}while(token.second != -1);
	}

	for(int i=0; i< no_of_rules; i++)
	{
		if( !vis[non_terminals[i]] )
			find_first(non_terminals[i], i) ;
	}	
	vis.clear();	
	
	//find follow for the grammar
	follow[non_terminals[0]].push_back("$");
	for(int i=0; i< no_of_rules; i++)
	{
		if(!vis[non_terminals[i]])
			findFollow(non_terminals[i]);
		
	}
	
	map< string, vector< pair <string, int> > > :: iterator itr;
    cout<<"FIRST SET of Given Grammar\n------------------------------------------\n";	
	for(itr = first1.begin(); itr!= first1.end(); itr++)
	{
		cout<<"FIRST ("<< itr->first<<") = { ";
		for(int i= 0; i< itr->second.size()-1; i++)
			cout<<itr->second[i].first<<", ";
		cout<<itr->second[ itr->second.size()-1 ].first << " }";
		
		cout<<endl;
	}
	cout<<endl;
	map< string, vector< string> > :: iterator itr2;
    cout<<"Follow SET of Given Grammar\n------------------------------------------\n";	
	for(itr2 = follow.begin(); itr2 != follow.end(); itr2++)
	{
		
		cout<<"FOLLOW ("<<itr2->first<<") = { ";
		for(int i= 0; i< itr2->second.size()-1; i++)
			cout<<itr2->second[i]<<", ";
		cout<<itr2->second[ itr2->second.size()-1 ] <<" }";
		
		cout<<endl;
	}
	
	
	//p table
	terminals.push_back("$");
	for(int i=0; i< no_of_rules; i++)
	{
		int has_epsi = 0;
		
		for(int j=0; j< terminals.size(); j++)
		{
			int f=0;
			for(int k=0; k< first1[non_terminals[i]].size(); k++)
			{
				
				if(first1[non_terminals[i]][k].first == terminals[j])
				{	
					string y = putEntry(i, first1[non_terminals[i]][k].second);
					
					parser_table[i].push_back(y);
					f=1;
					
				}
				if(first1[non_terminals[i]][k].first == "Epsilon")
					has_epsi = 1;
			}
			if(has_epsi && !f)
			{
				for(int k = 0; k< follow[non_terminals[i]].size(); k++)
				{
					if(follow[non_terminals[i]][k] == terminals[j])
					{
						parser_table[i].push_back("Epsilon"); 
						f=1;
						break;
					}	
				}
			}	
			if(!f)
				parser_table[i].push_back(" ");
		}
		
	}

	
	string str;
	cout<<"\n";
	
	

	file2.open("temp.txt");
	std::vector<string> program;

	while ( getline (file2,str) ) 
	{
		program.push_back(str);
	}


	program.push_back("$");
	vector<string> stck;
	stck.push_back("$");
	stck.push_back(non_terminals[0]);
	int i=0, err = 0;
	cout<<"Parsing the Source \n------------------------------------------\n";
	while(i < production.size() && stck.size() > 0)
	{

		pair<string, int> term = make_pair("", 0);
		term = tokenize(program[i], term.second, ' ');
		
		
		
		do
		{

			cout << "Term from rep. input \t->  "<< term.first << " " << endl;
			string y = stck.back();
			cout << "Term from Stack\t\t->  " << y << endl;

			stck.pop_back();
			if(check_terminal(y) || y[0] == '$')
			{
				if(y == term.first)
				{
					term = tokenize(program[i], term.second, ' ');
					if(term.second == -2 )
					{
						//i++;
						break;
					}
					continue;
				}
				else
				{
					err=1;
					cout<<"Error\n";
					break;
				}

			}
			int j,k;

			for( k=0; k<terminals.size(); k++)
				if(terminals[k] == term.first)
					break;
			for( j=0; j<non_terminals.size(); j++)
				if(non_terminals[j] == y)
					break;

			string z = parser_table[j][k];

			if(z == "Epsilon")
				continue;
			string ss="";
			std::vector<string> temp;
			for(j= 0; j < z.size(); j++)
			{
				if(z[j] == ' ')
				{
					temp.push_back(ss);
					ss = "";
				}	

				else
					ss += z[j];
			}
			temp.push_back(ss);
			for(j= temp.size()-1; j>=0; j--)
			{


				stck.push_back(temp[j]);
			}	
		}while(term.second != -2);
		i++;
	}
	
	if(!err)
		cout<<"Input string is succesfully parsed\n";
	else
		cout<<"Opps! Error Occured\n";
	

	file.close();
	file2.close();
	return 0;
}		







