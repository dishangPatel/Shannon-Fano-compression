#include<iostream>
#include<vector>
#include<fstream>
#include<algorithm>
#include<map>
#include<math.h>
#include<bitset>
using namespace std;

struct letter
{
	int f;
	char c;
	string code;
};
int redundant=0;
string convertToBinary(int val)
{
	if(val==0) return "0";
	string bits="",tmp="";
	while (val > 0) {  
		if(val % 2) tmp+='1';
        else tmp+='0';
       	val = val / 2;  
    }
    for(int i=tmp.length()-1;i>-1;i--)
    {
    	bits+=tmp[i];
	}
	return bits;
}

void decompress(vector<letter> &data)
{
	map<string,char> codeToChar;
	for(int i=0;i<data.size();i++)
	{
		codeToChar.insert(pair<string,char>(data[i].code,data[i].c));	
	}

	fstream r;
	r.open("compressed.txt",fstream::in);
	fstream w;
	w.open("binCompress.txt",fstream::out);
	if(!w.is_open()||!r.is_open())
	{
		cout<<"not opened";
		return; 
	} 
	
		int v; char k; string b;
		while(r>>noskipws>>k)
		{
			v=(int)k;
			if(v<0) v+=256;
			b=convertToBinary(v);
			bitset<8> bit(b);
			for(int i=7;i>-1;i--)
			{
				w<<bit[i];	
			}	
		}
		cout<<"\nredundant bits stored : "<<redundant;
		long pos=w.tellp();
		w.seekp(pos-redundant);
		while(redundant--){
			w<<" ";
		}
	w.close();
	r.close();	


	string bits="";
	char c;

	ifstream fr;
	fr.open("binCompress.txt",fstream::in);
	if(fr.is_open())
	{
		ofstream fw;
		fw.open("decoded.txt",fstream::trunc|fstream::out);
		if(fw.is_open())
		{
			while(fr.get(c))
			{
				bits+=c;
				if(codeToChar.find(bits)!=codeToChar.end())
				{
					fw<<codeToChar[bits];
					bits="";	
				}	
			}
			fw.close();
		}
		else
		{
			cout<<"could not open file";
			}
			fr.close();	
	}
	else{
		cout<<"could not read file";
	}
	
	
}

void compress()
{
	char c;
	int i,p=7,sum=0;
	fstream fr,fw;
	fr.open("binCompress.txt",fstream::in);
	fw.open("compressed.txt",fstream::out|fstream::trunc);
	if(!fr) return;
	while(fr.get(c))
	{
		
	  	if(p==0)
	  	{
	  		p=7; sum+=((c-48)*1); fw.put((char)sum); sum=0;	 	
		}
		else
		{
			sum+=((c-48)*pow(2,p)); p--;
		}
	}
	if(sum!=0)
	{
		fw.put((char)sum); sum=0;
	}
	if(p==7){
		redundant=0;
	}
	else
	{
		redundant=p+1;
	}
	fw.close();
	fr.close();
	
	int k=remove("binCompress.txt");
}

void encode(vector<char> temp,vector<letter> data)
{
	fstream cf;
	cf.open("binCompress.txt",fstream::trunc|fstream::out);
	if(!cf)
	{
		cout<<"file not found";
		return;
	}
	map<char,letter> hash;
	map<char,letter>::iterator itr;
	for(int i=0;i<data.size();i++)
	{
		hash.insert(pair<char,letter>(data[i].c,data[i]));
	}
	letter tmp;
	char ch;
	for(int i=0;i<temp.size();i++)
	{
		ch=temp[i];
		itr=hash.find(ch);
		tmp=itr->second;
		if(itr!=hash.end())
		{
			for(int j=0;j<tmp.code.length();j++)
			{
				cf<<tmp.code[j];
			}
		}
	}
	
	cf.close();
}

void divide(vector<letter> &data,int l,int h)
{
	
	if(l==h) {return;}
	else
	{
		int lp=l,rp=h;
		int sumL=data[lp].f,sumR=data[rp].f;
		
		while(lp+1<rp)
		{
			if(sumL>sumR)
			{
				sumR+=data[--rp].f;
			}
			else
			{
				sumL+=data[++lp].f;
			}
		}
		
		for(int i=l;i<=lp;i++)
		{
			data[i].code+="0";
		}
		
		for(int j=rp;j<=h;j++)
		{	
			data[j].code+="1";
		}	
		
		divide(data,l,lp);
		divide(data,rp,h);			
	}
}

void printTable(vector<letter> &data)
{
	cout<<"";
	cout<<"\n\n";
	cout<<"Table of encoding\n";
	for(int i=0;i<data.size();i++)
	{
		cout<<data[i].c<<" "<<data[i].f<<" ";
		for(int j=0;j<data[i].code.length();j++) 
		{
			cout<<data[i].code[j];
		}
		cout<<"\n";
	}
}

bool mySort(letter l1,letter l2){return l1.f>l2.f?true:false;}

int main()
{
//------------------------variable declaration section--------------//
	int frequency[256]={0}; //frequency[26]
	vector<char> temp;
	char c;
	fstream file;
	
//-----------------------read data from file------------------------//	
	
	file.open("test.txt");
	if(!file) {cout<<"input file doesnt available"; return 0;}
	
	while(file.get(c))
	{	
		temp.push_back(c);
		cout<<c;	
	}	
	file.close();
	
//-----------------------apply algorithm - generate encodes----------------------------//		
							//get frequency of all letters
			
	for(int i=0;i<temp.size();i++)
	{
		frequency[temp[i]]++;
	}

	letter l;
	vector<letter> data;
	int sum;
	for(int i=0;i<256;i++)
	{
		if(frequency[i]!=0) { 
		l.f=frequency[i];
		l.c=(char)(i);
		data.push_back(l);	
		}
	}
	sort(data.begin(),data.end(),mySort);
	divide(data,0,data.size()-1); 
	printTable(data);
	encode(temp,data);
	compress();	
	decompress(data);
	return 0;
}
