#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>

#include "temperature_conversion.h"
#include <mosquittopp.h>
#include <fstream>

void (*onIncomingMessage)(std::string, std::string);

mqtt_tempconv::mqtt_tempconv(void (*onMsg)(std::string, std::string)) : mosquittopp("dumb id")
{
	int keepalive = 60;
	
	onIncomingMessage = onMsg;

	printf("Got to first part here\n");
	std::ifstream filein("userpass.txt");
	std::string user;
	std::string pass;
	
	printf("Reading line\n");
	std::getline(filein, user);
	std::getline(filein, pass);
	
	printf("setting user and pass\n");
	username_pw_set(user.c_str(), pass.c_str());
	
	/* Connect immediately. This could also be done by calling
	 * mqtt_tempconv->connect(). */
	tls_set("/etc/ssl/certs/ca-certificates.crt");
	printf("Connecting\n");
	connect("m13.cloudmqtt.com", 22183, keepalive);
	printf("Home sweet home\n");
};

mqtt_tempconv::~mqtt_tempconv()
{
}

void mqtt_tempconv::on_connect(int rc)
{
	printf("Connected with code %d.\n", rc);
	if(rc == 0){
		/* Only attempt to subscribe on a successful connect. */
		subscribe(NULL, "iot/down");
	}
}

void split(const std::string &s, char delim, std::vector<std::string> &elems) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
}


std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, elems);
    return elems;
}

void mqtt_tempconv::on_message(const struct mosquitto_message *message)
{
	char buf[51];
	printf("Got: %s\n", message->payload);
	
	if(!strcmp(message->topic, "iot/down")){
		memset(buf, 0, 51*sizeof(char));
		
		memcpy(buf, message->payload, 50*sizeof(char));
		printf("Got: %s\n", buf);
		
		std::vector<std::string> splits = split(std::string(buf),  '\n');
		if (splits.size == 2)
		{
			onIncomingMessage(splits[0], splits[1]);
		}
		//temp_celsius = atof(buf);
		//temp_farenheit = temp_celsius*9.0/5.0 + 32.0;
		//publish(NULL, "temperature/farenheit", strlen(buf), buf);
	}
/*
	if(!strcmp(message->topic, "test/cool")){
		memset(buf, 0, 51*sizeof(char));
		
		memcpy(buf, message->payload, 50*sizeof(char));
		//temp_celsius = atof(buf);
		//temp_farenheit = temp_celsius*9.0/5.0 + 32.0;
		printf("Got: %s\n", buf);
		//publish(NULL, "temperature/farenheit", strlen(buf), buf);
	}*/
}

void mqtt_tempconv::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
	printf("Subscription succeeded.\n");
}

