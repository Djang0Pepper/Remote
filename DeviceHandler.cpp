/*
  DeviceHandler.cpp - Work as device loader and save device configuration
  Saul bertuccio 9 feb 2017
  Released into the public domain.
*/

#include "DeviceHandler.h"

const String DeviceHandler::DATA_DIR = "/data/";

DeviceHandler::DeviceHandler():
  is_opened(SPIFFS.begin()),
  device(NULL)
{}


DeviceHandler::~DeviceHandler() {
  delete device;
}

boolean DeviceHandler::setDevice( const String &device_name, const String &device_type ) {

  if ( device && (device->getName() == device_name) )
    return true;

  if (device)
    delete device;

  if (device = loadDeviceFile( device_name )) {

    return DeviceHandler::saveDeviceFile( device );
    
  } else if (device = new Device(device_name, device_type)) {

    if (!DeviceHandler::saveDeviceFile( device )) {
      delete device;
      device = NULL;
    } else {
      // Invalidate device counter
      DeviceHandler::recountDevices(true);
    }

  }
  return device != NULL;
}

boolean DeviceHandler::setDevice( const String &device_name ) {

  if ( device && (device->getName() == device_name) )
    return true;

  if (device)
    delete device;

  device = loadDeviceFile( device_name );
  return device != NULL;
}

boolean DeviceHandler::addDeviceKey( String * attr ) {

  if (!device) return false;
  return device->addKey( attr )
    && DeviceHandler::saveDeviceFile( device );
}

boolean DeviceHandler::deleteDeviceKey( String &kname ) {

  if (!device) return false;
  return device->removeKey( kname )
    && DeviceHandler::saveDeviceFile( device );
}


Device & DeviceHandler::getDevice() { return *device; }

boolean DeviceHandler::renameDevice( const String &new_name) {

  if (!device) return false;

  if (DeviceHandler::existsDeviceFile( new_name ))
    return false;

  if (DeviceHandler::renameDeviceFile( device->getName(), new_name)) {
    device->setName(new_name);
    return true;
  }
  return false;
}

boolean DeviceHandler::renameDeviceFile( const String &old_name, const String &new_name) {
  return SPIFFS.rename(DATA_DIR + old_name, DATA_DIR + new_name);
};

boolean DeviceHandler::deleteDevice() {

  if (!device) return false;

  bool r = true;
  if (DeviceHandler::existsDeviceFile( device->getName() ))
    r = DeviceHandler::deleteDeviceFile( device->getName() );

  // Invalidate device counter
  DeviceHandler::recountDevices(true);

  if (r) { 
    delete device;
    device = NULL;
  }
  return r;
}

boolean DeviceHandler::deleteDeviceFile( const String &device_name ) { return SPIFFS.remove(DATA_DIR + device_name); }

boolean DeviceHandler::saveDevice() {

  if (!device)
    return false;

  return DeviceHandler::saveDeviceFile( device );
}

Device * DeviceHandler::loadDeviceFile( const String &device_name ) {

  String file_name = DATA_DIR + device_name;
  
  if (!SPIFFS.exists(file_name)) {
    Serial.print("Erreur: impossible de charger le fichier : ");
    Serial.println(file_name);
    return NULL;
  }

  File f = SPIFFS.open( file_name, "r");

  if (!f) {
    Serial.println("Erreur lors de l'ouverture du fichier de périphérique");
    return NULL;
  }

  // First line is device type
  String line = f.readStringUntil(DeviceHandler::EOL);

  if (line == "") {
    f.close();
    Serial.println("Impossible de lire le type de périphérique");
    return NULL;
  }

  Device * d = new Device( device_name, line );

  if (!d) {
    f.close();
    return NULL;
  }

  int num = d->getKeysPropertyNum();

  String * k_attr = new String[num];

  while ( f.available() ) {
    
    line = f.readStringUntil(DeviceHandler::EOL);

    if (line.length() == 1)
      continue;

    int s = -1,  e = 0, i = 0;

    while ( i < num ) {

      e = line.indexOf(DeviceHandler::SEP, ++s);
      if ( e == -1 )
        break;
      k_attr[i++] = line.substring(s, e);
      s = e;
    }

    if (i == num)
      d->addKey(k_attr);
  }

  delete[] k_attr;
  f.close();
  return d;
}

boolean DeviceHandler::saveDeviceFile( Device * device ) {

  String fname = DATA_DIR + device->getName();
  File f = SPIFFS.open(fname, "w");

  if (!f) {
    Serial.print("Erreur: le fichier n'a pas pu être ouvert pour l'écriture ");
    Serial.println(fname);
    return false;
  }

  // Salvo il tipo di device
  f.print( device->getType() + DeviceHandler::EOL);

  int attr_num = device->getKeysPropertyNum();
  Key * k = device->getKeys();

  while (k) {
    String line = "";
    for (int i = 0; i < attr_num; i++) {
        line += k->getPropertyById(i) + DeviceHandler::SEP;
    }
    line += DeviceHandler::EOL;
    f.print(line);
    k = k->getNext();
  }

  f.close();

  return true;
}

boolean DeviceHandler::existsDeviceFile( const String &device_name ) { return SPIFFS.exists(DATA_DIR + device_name); }

String * DeviceHandler::getDevicesName() {

  String * names = NULL;

  int c = getDevicesNum();

  if (c <= 0) return NULL;
    
  names = new String[c];

  Dir dir = SPIFFS.openDir(DATA_DIR);
  int l = DATA_DIR.length();

  while (dir.next())
    names[--c] = dir.fileName().substring(l);

  return names;

}

int DeviceHandler::getDevicesNum() { return recountDevices(false); }

int DeviceHandler::recountDevices(bool force) {

  static int device_num = -1;

  if (device_num >= 0 && !force)
    return device_num;

  Dir dir = SPIFFS.openDir(DATA_DIR);
  device_num = 0;
  
  while (dir.next())
    device_num++;

  return device_num;

}

int DeviceHandler::getDeviceTypesNum() { return Device::TYPE_NUM; }
String * DeviceHandler::getDeviceTypes() { return Device::TYPES; }
String * DeviceHandler::getDeviceTypesDescription() { return Device::TYPES_DESCRIPTION; }
