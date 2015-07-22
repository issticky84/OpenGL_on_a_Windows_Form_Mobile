#include "stdafx.h"
#include "ReadCSV.h"
#include <string.h>
#include <fstream>


void ReadCSV::clear()
{
	raw_data.clear();
	attribute_index.clear();
}

void ReadCSV::read_raw_data(char file_name[])
{
	vector<string> title_name;

	FILE *csv_file;
	csv_file = fopen(file_name,"r");
	if(!csv_file) 
	{
		cout << "Can't open config file!" << endl;
		system("pause");
		exit(1);
	}

	char line[LENGTH];
	char *token;
	int i,j;
	i = j = 0;
	fgets(line,LENGTH,csv_file); //ignore sep=
	fgets(line,LENGTH,csv_file); //ignore title
	
	token = strtok(line,";");
	while(token!=NULL)
	{
		title_name.push_back(token);
		token = strtok(NULL,";");
	}
	for(int i=0;i<title_name.size();i++)
	{
		//cout << title_name[i] << " ";
		if(title_name[i].compare("GRAVITY X (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("GRAVITY Y (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("GRAVITY Z (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("LINEAR ACCELERATION X (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("LINEAR ACCELERATION Y (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("LINEAR ACCELERATION Z (m/s²)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("GYROSCOPE X (rad/s)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("GYROSCOPE Y (rad/s)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("GYROSCOPE Z (rad/s)")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("LOCATION Latitude : ")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("LOCATION Longitude : ")==0){ attribute_index.push_back(i); }
		else if(title_name[i].compare("YYYY-MO-DD HH-MI-SS_SSS\n")==0){ time_index = i; }
	}

	//for(int i=0;i<attribute_index.size();i++)
	//	cout << attribute_index[i] << endl;

	cout << "time_index " << time_index << endl;

	while(!feof(csv_file))
	{
		fgets(line,LENGTH,csv_file);
		token = strtok(line,";");
		raw_data.push_back(vector<float> (1));
		//printf("%s ",token);
		while(token!=NULL)
		{
			raw_data.back().push_back(atof(token));
			//token = strtok(NULL," ;:");
			token = strtok(NULL," ;:");
		}
	}

	//for(int i=0;i<33;i++)
	//{
	//	cout << raw_data[0][i] << " ";
	//}
	//cout << endl;
	//cout << raw_data[0][1] << " " << raw_data[0][29] << " " << raw_data[0][30] << " " << raw_data[0][31] << " " << raw_data[0][32]  << " " << raw_data[0][33] << endl;
	//cout << raw_data[0][1] << " " << raw_data[0][27] << " " << raw_data[0][28] << " " << raw_data[0][29] << " " << raw_data[0][30]  << " " << raw_data[0][31] << endl;
	//cout << raw_data[0][1] << " " << raw_data[0][31] << " " << raw_data[0][32] << " " << raw_data[0][33] << " " << raw_data[0][34]  << " " << raw_data[0][35] << endl;
	//29:Year,30:Hour,31:Minute,32:second,33:millisecond

	cout << "Csv Data Size: " << raw_data.size() <<endl;
	//cout << raw_data[0].size() << endl;

	raw_data_size = raw_data.size();

	fclose(csv_file);
}