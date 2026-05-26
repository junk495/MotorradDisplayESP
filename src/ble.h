#ifndef BLE_H
#define BLE_H

#include <Arduino.h>
#include <BLE2902.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <vector>

BLEServer* pServer      = NULL;
bool deviceConnected    = false;
bool oldDeviceConnected = false;

#define SERVICE_UUID     "ec91d7ab-e87c-48d5-adfa-cc4b2951298a"
#define CHA_SETTINGS     "9d37a346-63d3-4df6-8eee-f0242949f59f"
#define CHA_NAV          "0b11deef-1563-447f-aece-d3dfeb1c1f20"
#define CHA_NAV_TBT_ICON "d4d8fcca-16b2-4b8e-8ed5-90137c44a8ad"
#define CHA_GPS_SPEED    "98b6073a-5cf3-4e73-b6d3-f8e05fa018a9"

extern void onCharacteristicWrite(const String& uuid, uint8_t* data, size_t length);

struct CharacteristicConfig {
	String name;
	String uuid;
	BLECharacteristic* bleCharacteristic;

	CharacteristicConfig(String n, String u) : name(n), uuid(u), bleCharacteristic(nullptr) {
	}
};

struct ServiceConfig {
	String name;
	String uuid;
	std::vector<CharacteristicConfig> characteristics;
	BLEService* bleService;

	ServiceConfig(String n, String u, std::vector<CharacteristicConfig> c)
	: name(n), uuid(u), characteristics(c), bleService(nullptr) {
	}
};

struct MyBleServer {
	BLEServer* bleServer = nullptr;
	std::vector<ServiceConfig> services;

	ServiceConfig* findServiceByUuid(const String& uuid) {
		for (auto& item : services) {
			if (item.uuid == uuid)
				return &item;
		}
		return nullptr;
	}

	CharacteristicConfig* findCharacteristicByUuid(const String& uuid) {
		for (auto& service : services) {
			for (auto& item : service.characteristics) {
				if (item.uuid == uuid)
					return &item;
			}
		}
		return nullptr;
	}
};

MyBleServer server;

class ServerCallbacks : public BLEServerCallbacks {
	void onConnect(BLEServer* pServer) override {
		deviceConnected = true;
		Serial.println("Device connected");
	}

	void onDisconnect(BLEServer* pServer) override {
		deviceConnected = false;
		Serial.println("Device disconnected, start advertising...");
		pServer->getAdvertising()->start();
	}
};

class CharacteristicWriteCallbacks : public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic* pCharacteristic) override {
		std::string uuid_str = std::string(pCharacteristic->getUUID().toString().c_str());
		String uuid          = String(uuid_str.c_str());

		const auto characteristicInfo = server.findCharacteristicByUuid(uuid);

		if (!characteristicInfo) {
			Serial.print("Error: No characteristic found with UUID: ");
			Serial.println(uuid);
			return;
		}

		Serial.print("Value written to ");
		Serial.print(characteristicInfo->name);
		Serial.print(": ");

		std::string rxValue = pCharacteristic->getValue();
		Serial.println(rxValue.c_str());

		onCharacteristicWrite(uuid, pCharacteristic->getData(), pCharacteristic->getLength());
	}
};

void initBle() {
	server.services.push_back(
	ServiceConfig("CATDRIVE", SERVICE_UUID,
	              {CharacteristicConfig("SETTINGS", CHA_SETTINGS), CharacteristicConfig("NAV", CHA_NAV),
	               CharacteristicConfig("NAV TBT ICON", CHA_NAV_TBT_ICON), CharacteristicConfig("GPS SPEED", CHA_GPS_SPEED)}));

	BLEDevice::init("CatDrive");
	Serial.println(BLEDevice::getMTU());
	BLEDevice::setMTU(240);

	server.bleServer = BLEDevice::createServer();
	server.bleServer->setCallbacks(new ServerCallbacks());
	CharacteristicWriteCallbacks* writeCallbacks = new CharacteristicWriteCallbacks();

	for (auto& serviceConfig : server.services) {
		// Create service with proper BLEUUID
		BLEUUID serviceUuid(serviceConfig.uuid.c_str());
		serviceConfig.bleService = server.bleServer->createService(serviceUuid);

		for (auto& characteristicConfig : serviceConfig.characteristics) {
			const auto property = BLECharacteristic::PROPERTY_WRITE;

			BLEUUID charUuid(characteristicConfig.uuid.c_str());
			characteristicConfig.bleCharacteristic = serviceConfig.bleService->createCharacteristic(charUuid, property);
			characteristicConfig.bleCharacteristic->setCallbacks(writeCallbacks);

			// Add descriptor
			BLEDescriptor* pDesc = new BLEDescriptor((uint16_t)0x2901);
			pDesc->setValue(characteristicConfig.name.c_str());
			characteristicConfig.bleCharacteristic->addDescriptor(pDesc);
		}

		serviceConfig.bleService->start();
	}

	server.bleServer->getAdvertising()->start();
}

void notifyCharacteristic(const String& uuid, uint8_t* data, size_t length) {
	auto characteristicInfo = server.findCharacteristicByUuid(uuid);
	if (!characteristicInfo || !characteristicInfo->bleCharacteristic) {
		Serial.print("Error: No characteristic found with UUID: ");
		Serial.println(uuid);
		return;
	}

	BLECharacteristic* pCharacteristic = characteristicInfo->bleCharacteristic;
	pCharacteristic->setValue(data, length);
	pCharacteristic->notify();
}
#endif