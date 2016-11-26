#include <cstdio>
#include <cstring>

#include "temperature_conversion.h"
#include <mosquittopp.h>
#include <fstream>

mqtt_tempconv::mqtt_tempconv() : mosquittopp("dumb id")
{
	int keepalive = 60;

	printf("Got to first part here\n");
	std::ifstream filein("userpass.txt");
	std::string user;
	std::string pass;
	
	printf("Reading line\n");
	std::getline(filein, user);
	std::getline(filein, pass);
	
	printf("setting user and pass %s %s\n", user.c_str(), pass.c_str());
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
		subscribe(NULL, "test/cool");
	}
}

void mqtt_tempconv::on_message(const struct mosquitto_message *message)
{
	double temp_celsius, temp_farenheit;
	char buf[51];

	if(!strcmp(message->topic, "test/cool")){
		memset(buf, 0, 51*sizeof(char));
		/* Copy N-1 bytes to ensure always 0 terminated. */
		memcpy(buf, message->payload, 50*sizeof(char));
		//temp_celsius = atof(buf);
		//temp_farenheit = temp_celsius*9.0/5.0 + 32.0;
		printf(buf);
		//publish(NULL, "temperature/farenheit", strlen(buf), buf);
	}
}

void mqtt_tempconv::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
	printf("Subscription succeeded.\n");
}

