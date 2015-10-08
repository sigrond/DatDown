#include<iostream>
#include<curl/curl.h>
#include<fstream>
using namespace std;
static string buffer;
static int writer(char *data, size_t size, size_t nmemb,std::string *buffer)
{
  int result = 0;
  if (buffer != NULL)
  {
    // Append the data to the buffer
    buffer->append(data, size * nmemb);
    // How much did we write?
    result = size * nmemb;
  }
  return result;
}
int main(int argc, char* argv[])
{
    bool debug=false;
    string arg;
    for(int i=0; i<argc; i++)
    {
        arg=argv[i];
        if(arg=="-debug")
        {
            debug=true;
        }
    }
	CURL *curl;
	CURLcode res;
	curl=curl_easy_init();
	if(curl)
	cout<<"init succesfull!"<<endl;
	else
	return 0;
	curl_easy_setopt(curl, CURLOPT_URL, "http://www.aviationweather.gov/adds/metars/?station_ids=EPWA&std_trans=standard&chk_metars=on&hoursStr=most+recent+only&submitmet=Submit");

	curl_easy_setopt(curl, CURLOPT_HEADER, 0);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
	res=curl_easy_perform(curl);
	curl_easy_cleanup(curl);
	cout<<"download done!"<<endl;
	if(debug)
    {
        cout<<buffer<<endl;
    }


	size_t num;
	num=buffer.find(">EPWA");
	if(num==string::npos)
	return 0;
	if(debug)
	cout<<buffer[num+5]<<"+"<<(int)buffer[num+5]<<endl;
	int li=0;
	while(true)
	{
		num++;//32space
		if((int)buffer[num]==32)//6q
		{
		    li++;
            if(buffer[num+1]=='Q' && li>3)
            break;
		}
	}
	if(debug)
	cout<<buffer[num+1]<<endl;
	num++;
	string st="";
	fstream f;
	f.open("cisnienie.txt", ios::out);
	while((int)buffer[num+1]>46 && (int)buffer[num+1]<58)
	{
	    if(debug)
            cout<<(int)buffer[num+1]<<" "<<buffer[num+1]<<endl;
		num++;
		st+=buffer[num];
	}
	if(debug)
        cout<<st<<endl;
	f<<st;
	f.close();
	cout<<"file saved"<<endl;

	//system("pause");
	return 0;
}
