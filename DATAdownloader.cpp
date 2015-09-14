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
int main()
{
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


	size_t num;
	num=buffer.find(">EPWA");
	if(num==string::npos)
	return 0;
	//cout<<buffer[num+5]<<"+"<<(int)buffer[num+5]<<endl;
	int li=0;
	while(true)
	{
		num++;//32space
		if((int)buffer[num]==32)//6q
		li++;
		if(li==6)
		break;
	}
	//cout<<buffer[num+1]<<endl;
	num++;
	string st="";
	fstream f;
	f.open("cisnienie.txt", ios::out);
	while((int)buffer[num+1]>46 && (int)buffer[num+1]<58)
	{
		num++;
		st+=buffer[num];
	}
	f<<st;
	f.close();

	//system("pause");
	return 0;
}
