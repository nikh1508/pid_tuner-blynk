const serial = require('serialport');
const arduino = new serial('/dev/ttyAMA0', { baudRate: 115200 });

const startMarker = 254
const endMarker = 255
const specialByte = 253

var methods = {
    port: arduino,
    encodeToString: function (data) {
        var outBytes = [startMarker];
        for (var i = 0; i < data.length; i++) {
            var val = data[i];
            if (val >= specialByte) {
                outBytes.push(specialByte);
                outBytes.push(val - specialByte);
            }
            else
                outBytes.push(val);
        }
        outBytes.push(endMarker);
        return outBytes;
    },

    sendToArduino: function (toSend) {
        arduino.write(this.encodeToString(toSend));

    }
};

module.exports = methods;