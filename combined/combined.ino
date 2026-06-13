#include "painlessMesh.h"

#define MESH_PREFIX "FarmMesh"
#define MESH_PASSWORD "12345678"
#define MESH_PORT 5555

Scheduler userScheduler;
painlessMesh mesh;

String nodeName = "COLLAR";

void sendMessage();

Task taskSendMessage(TASK_SECOND * 2, TASK_FOREVER, &sendMessage);

void sendMessage() {

  String msg = nodeName + " Node ID: " + String(mesh.getNodeId());

  mesh.sendBroadcast(msg);

  Serial.println("Sent: " + msg);
}

void receivedCallback(uint32_t from, String &msg) {

  Serial.printf("Received from %u msg=%s\n",
                from,
                msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {

  Serial.printf("New Connection: %u\n", nodeId);
}

void setup() {

  Serial.begin(115200);

  mesh.setDebugMsgTypes(ERROR | STARTUP);

  mesh.init(MESH_PREFIX,
            MESH_PASSWORD,
            &userScheduler,
            MESH_PORT);

  mesh.onReceive(&receivedCallback);

  mesh.onNewConnection(&newConnectionCallback);

  userScheduler.addTask(taskSendMessage);

  taskSendMessage.enable();

  Serial.println("Collar Node Started");
}

void loop() {

  mesh.update();
}
