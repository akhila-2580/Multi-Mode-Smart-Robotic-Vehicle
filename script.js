let currentMode = "M";


function sendCommand(command) {

    if (window.Android) {
        Android.sendCommand(command);
    }
}


function stopGesture() {

    if (window.Android) {
        Android.stopGesture();
    }
}


function startGesture() {

    if (window.Android) {
        Android.startGesture();
    }
}


/* CONNECT */

document.getElementById("connectBtn").addEventListener("click", function () {

    if (window.Android) {
        Android.connect();
    }

});


/* MANUAL */

document.getElementById("manualMode").addEventListener("click", function () {

    currentMode = "M";

    stopGesture();

    sendCommand("M");

    document.getElementById("status").innerText =
        "Bluetooth: Manual Mode";

    document.getElementById("voiceControls").style.display = "none";

    document.querySelector(".gestureInfo").style.display = "none";

    document.getElementById("followInfo").style.display = "none";

});


/* AUTO */

document.getElementById("autoMode").addEventListener("click", function () {

    currentMode = "A";

    stopGesture();

    sendCommand("A");

    document.getElementById("status").innerText =
        "Bluetooth: Auto Mode";

    document.getElementById("voiceControls").style.display = "none";

    document.querySelector(".gestureInfo").style.display = "none";

    document.getElementById("followInfo").style.display = "none";

});


/* VOICE */

document.getElementById("voiceMode").addEventListener("click", function () {

    currentMode = "V";

    stopGesture();

    sendCommand("V");

    document.getElementById("status").innerText =
        "Bluetooth: Voice Mode";

    document.getElementById("voiceControls").style.display = "block";

    document.querySelector(".gestureInfo").style.display = "none";

    document.getElementById("followInfo").style.display = "none";

});


/* GESTURE */

document.getElementById("gestureMode").addEventListener("click", function () {

    currentMode = "G";

    sendCommand("G");

    document.getElementById("status").innerText =
        "Bluetooth: Gesture Mode";

    document.getElementById("voiceControls").style.display = "none";

    document.querySelector(".gestureInfo").style.display = "block";

    document.getElementById("followInfo").style.display = "none";

    startGesture();

});


/* HUMAN FOLLOW */

document.getElementById("followMode").addEventListener("click", function () {

    currentMode = "H";

    stopGesture();

    sendCommand("H");

    document.getElementById("status").innerText =
        "Bluetooth: Human Follow Mode";

    document.getElementById("voiceControls").style.display = "none";

    document.querySelector(".gestureInfo").style.display = "none";

    document.getElementById("followInfo").style.display = "block";

});


/* HEADLIGHT ON */

document.getElementById("headlightOn").addEventListener("click", function () {

    sendCommand("K");

    document.getElementById("status").innerText =
        "Headlights ON";

});


/* HEADLIGHT OFF */

document.getElementById("headlightOff").addEventListener("click", function () {

    sendCommand("k");

    document.getElementById("status").innerText =
        "Headlights OFF";

});


/* MANUAL FORWARD */

document.getElementById("forward").addEventListener("click", function () {

    if (currentMode === "M") {
        sendCommand("F");
    }

});


/* MANUAL BACKWARD */

document.getElementById("backward").addEventListener("click", function () {

    if (currentMode === "M") {
        sendCommand("B");
    }

});


/* MANUAL LEFT */

document.getElementById("left").addEventListener("click", function () {

    if (currentMode === "M") {
        sendCommand("L");
    }

});


/* MANUAL RIGHT */

document.getElementById("right").addEventListener("click", function () {

    if (currentMode === "M") {
        sendCommand("R");
    }

});


/* STOP */

document.getElementById("stop").addEventListener("click", function () {

    sendCommand("S");

});


/* VOICE RECOGNITION */

const SpeechRecognition =
    window.SpeechRecognition ||
    window.webkitSpeechRecognition;

let recognition = null;


if (SpeechRecognition) {

    recognition = new SpeechRecognition();

    recognition.lang = "en-IN";

    recognition.continuous = false;

    recognition.interimResults = false;


    recognition.onstart = function () {

        document.getElementById("micButton").innerText =
            "🎙 LISTENING...";

        document.getElementById("voiceText").innerText =
            "Listening...";

    };


    recognition.onresult = function (event) {

        const speech =
            event.results[0][0].transcript.toLowerCase();


        document.getElementById("voiceText").innerText =
            "Heard: " + speech;


        if (speech.includes("forward")) {

            sendCommand("F");

        }

        else if (
            speech.includes("backward") ||
            speech.includes("back")
        ) {

            sendCommand("B");

        }

        else if (speech.includes("left")) {

            sendCommand("L");

        }

        else if (speech.includes("right")) {

            sendCommand("R");

        }

        else if (
            speech.includes("stop") ||
            speech.includes("halt")
        ) {

            sendCommand("S");

        }

        else {

            document.getElementById("voiceText").innerText =
                "Command not recognized";

        }

    };


    recognition.onerror = function () {

        document.getElementById("voiceText").innerText =
            "Voice recognition error";

    };


    recognition.onend = function () {

        document.getElementById("micButton").innerText =
            "🎙 SPEAK";

    };

}


/* MICROPHONE */

document.getElementById("micButton").addEventListener("click", function () {

    if (currentMode !== "V") {

        document.getElementById("status").innerText =
            "Select Voice Mode first";

        return;

    }


    if (recognition) {

        document.getElementById("micButton").innerText =
            "🎙 LISTENING...";

        recognition.start();

    }

    else {

        document.getElementById("voiceText").innerText =
            "Speech recognition not supported";

    }

});