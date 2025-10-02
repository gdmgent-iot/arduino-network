const docentIP = "192.168.1.114";
const brokerUrl = "ws://" + docentIP;
const brokerPort = 9001;

// const brokerUrl = "ws://test.mosquitto.org";
// const brokerPort = 8080;

const client = mqtt.connect(`${brokerUrl}:${brokerPort}`);
const topic = "dps1/nm/alert";

// DOM elements
const $listReceived = document.getElementById("messagesList");
const $btnSend = document.getElementById("sendBtn");
const $inputMessage = document.getElementById("mqttMessage");

// function to add messages to the list
function addMessage(message) {
  const listItem = document.createElement("li");
  listItem.className = "list-group-item"; // bootstrap stuling
  listItem.textContent = message;
  $listReceived.appendChild(listItem);
}

client.on("connect", () => {
  addMessage("🚨 - We zijn verbonden met de broker");

  client.subscribe(topic, (err) => {
    if (err) addMessage("❗️ - Er ging iets mis");
    else addMessage("Geabonneerd op het topic");

    client.publish(topic, "Hallo van Frederick");
  });
});

client.on("message", (topic, message) => {
  addMessage("📩 - " + topic + " // " + message);
});

$btnSend.addEventListener("click", () => {
  const message = $inputMessage.value;
  client.publish(topic, message);
  addMessage("📤 - " + message);
  $inputMessage.value = "";
});
