#ifndef TEMPERATURE_CONVERSION_H
#define TEMPERATURE_CONVERSION_H

#include <mosquittopp.h>
#include <string>

class mqtt_tempconv : public mosqpp::mosquittopp
{
	public:
		mqtt_tempconv(void (*onMsg)(std::string, std::string));
		~mqtt_tempconv();

		void on_connect(int rc);
		void on_message(const struct mosquitto_message *message);
		void on_subscribe(int mid, int qos_count, const int *granted_qos);
		
		bool isReady();
};

#endif
