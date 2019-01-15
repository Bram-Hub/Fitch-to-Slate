#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

string symbols;
string convert(string clause);
string removenots(string clause);

bool isop(char a)
{
	if(a=='=')
		return true;
	if(a=='&')
		return true;
	if(a=='|')
		return true;
	if(a=='>')
		return true;
	if(a=='=')
		return true;
	if(a=='=')
		return true;
	if(a=='=')
		return true;
	if(a=='=')
		return true;
	return false;
}

bool isalpha(char a)
{
	if(a>64)
		if(a<91)
			return true;
	if(a>96)
		if(a<123)
			return true;
	return false;
}

string ifcombine(string subclause1,string subclause2)
{
	string clause;
	clause="(!";
	clause.append(subclause1);
	clause.append("|");
	clause.append(subclause2);
	clause.append(")");
	return clause;
}

string ifofcombine(string subclause1,string subclause2)
{
	string clause;
	clause="((";
	clause.append(subclause1);
	clause.append(">");
	clause.append(subclause2);
	clause.append(")&(");
	clause.append(subclause2);
	clause.append(">");
	clause.append(subclause1);
	clause.append("))");
	return clause;
}

string andcombine(string subclause1,string subclause2)
{
	string clause;
	clause="!(!";
	clause.append(subclause1);
	clause.append("|!");
	clause.append(subclause2);
	clause.append(")");
	return clause;
}

string orcombine(string subclause1,string subclause2)
{
	int x;
	for(x=0;x<(int)symbols.size();x++)
	{
		if(symbols[x]=='&')
		{
			string clause;
			clause="!(!";
			clause.append(subclause1);
			clause.append("&!");
			clause.append(subclause2);
			clause.append(")");
			return clause;
		}
	}
	for(x=0;x<(int)symbols.size();x++)
	{
		if(symbols[x]='>')
		{
			string clause;
			clause="(!";
			clause.append(subclause1);
			clause.append(">");
			clause.append(subclause2);
			clause.append(")");
			return clause;
		}
	}
}

string mash(char op, string subclause1, string subclause2)//put clauses back together
{
	string clause;
	for(int x=0;x<(int)symbols.size();x++)//check to make sure the operation isn't an acceptable operation
	{
		if(op==symbols[x])//put the clause back into it's original form.
		{
			clause="(";
			clause.append(subclause1);
			clause.push_back(op);
			clause.append(subclause2);
			clause.push_back(')');
			return clause;
		}
	}
	if(op=='=')
		return removenots(convert(ifofcombine(subclause1,subclause2)));
	if(op=='>')
		return removenots(convert(ifcombine(subclause1,subclause2)));
	if(op=='&')
		return removenots(convert(andcombine(subclause1,subclause2)));
	if(op=='|')
		return removenots(convert(orcombine(subclause1,subclause2)));
	//	if(op=='^')
	//		return nandcombine(subclause1,subclause2);
	return "Something is wrong";
}

string removenots(string clause)
{
	int x=0;
	string temp;
	while(x!=(int)clause.size()-1)
	{
		for(x=0;x<(int)clause.size()-1;x++)
		{
			if(clause[x]=='!')
				if(clause[x+1]=='!')
				{
					temp=clause.substr(0,x);
					temp.append(clause.substr(x+2));
					clause=temp;
					x=(int)clause.size();
				}
		}
	}
	return clause;
}

string convert(string clause)//recursive conversion function, clause should be !(!a|!b)
{
	clause=removenots(clause);
	string subclause1;
	string subclause2;
	string temp;
	bool opfound=false;
	int offset=0;
	int depth=0;
	for(int x=0;x<(int)clause.size();x++)//look for the key operation
	{
		if(clause[x]=='(')//if we come across parenthesis we are going down a level of depth
			depth++;
		if(clause[x]==')')//if we come across parenthesis we are going up a level of depth
			depth--;
		if(depth==1)//if we are on the main level we can look for the operation at hand
			if(isop(clause[x]))
			{
				offset=x;
				if(clause[0]=='!')
				{
					temp="!";
					subclause1=removenots(convert(clause.substr(2,x-2)));
					subclause2=removenots(convert(clause.substr(x+1,clause.size()-(x+2))));
				}
				else
				{
					temp.erase();
					subclause1=removenots(convert(clause.substr(1,x-1)));
					subclause2=removenots(convert(clause.substr(x+1,clause.size()-(x+2))));
				}
				opfound=true;
			}
	}
	temp.append(mash(clause[offset],subclause1,subclause2));
	//subclause1=removenots(subclause1);
	//subclause2=removenots(subclause2);
	if(opfound)//if we found an operation we need to put the subclauses back together
		return temp;//mash them together
	return clause;//otherwise we've reached base case
}

int main()
{
	ifstream in;
	in.open("input.txt");
	string sentence;
	vector<string> sentences;
	in >> symbols;//read in symbols
	while(in >> sentence)//read in sentences
	{
		sentences.push_back(sentence);
	}
	in.close();//close input file
	cout << "Symbols to use: ";
	int x;
	for(x=0;x<(int)symbols.size()-1;x++)//display symbols to use
		cout << symbols[x] << ",";
	cout << symbols[x] << endl;
	for(x=0;x<(int)sentences.size();x++)//display the converted sentences
	{
		cout << "Sentence " << x+1 << " " << convert(sentences[x]) << endl;
	}
	cout << "Type DONE and hit enter to exit" << endl;
	cin >> sentence;
	return 0;
}
