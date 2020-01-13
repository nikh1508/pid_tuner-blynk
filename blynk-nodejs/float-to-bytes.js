// Refer : https://stackoverflow.com/questions/15935365/convert-float-to-bytes-in-javascript-without-float32array/16043259

var toBytes = function (val) {
    bytes = []
    for (var i = 0; i < 4; i++)
        bytes.push((val >> (i * 8)) & 0xFF);
    return bytes;
}
var methods = {
    toFloat32: function (value) {
        if (value == 0) {
            return new Array(4).fill(0);
        }
        var bytes = 0;
        switch (value) {
            case Number.POSITIVE_INFINITY:
                bytes = 0x7f800000;
                break;
            case Number.NEGATIVE_INFINITY:
                bytes = 0xff800000;
                break;
            case +0.0:
                bytes = 0x40000000;
                break;
            case -0.0:
                bytes = 0xc0000000;
                break;
            default:
                if (Number.isNaN(value)) {
                    bytes = 0x7fc00000;
                    break;
                }

                if (value <= -0.0) {
                    bytes = 0x80000000;
                    value = -value;
                }

                var exponent = Math.floor(Math.log(value) / Math.log(2));
                var significand = ((value / Math.pow(2, exponent)) * 0x00800000) | 0;

                exponent += 127;
                if (exponent >= 0xff) {
                    exponent = 0xff;
                    significand = 0;
                } else if (exponent < 0) exponent = 0;

                bytes = bytes | (exponent << 23);
                bytes = bytes | (significand & ~(-1 << 23));
                break;
        }
        return toBytes(bytes);  // LSB First
    },

    toFloat64: function (value) {
        if (byteOffset + 8 > this.byteLength)
            throw "Invalid byteOffset: Cannot write beyond view boundaries.";

        var hiWord = 0,
            loWord = 0;
        switch (value) {
            case Number.POSITIVE_INFINITY:
                hiWord = 0x7ff00000;
                break;
            case Number.NEGATIVE_INFINITY:
                hiWord = 0xfff00000;
                break;
            case +0.0:
                hiWord = 0x40000000;
                break;
            case -0.0:
                hiWord = 0xc0000000;
                break;
            default:
                if (Number.isNaN(value)) {
                    hiWord = 0x7ff80000;
                    break;
                }

                if (value <= -0.0) {
                    hiWord = 0x80000000;
                    value = -value;
                }

                var exponent = Math.floor(Math.log(value) / Math.log(2));
                var significand = Math.floor(
                    (value / Math.pow(2, exponent)) * Math.pow(2, 52)
                );

                loWord = significand & 0xffffffff;
                significand /= Math.pow(2, 32);

                exponent += 1023;
                if (exponent >= 0x7ff) {
                    exponent = 0x7ff;
                    significand = 0;
                } else if (exponent < 0) exponent = 0;

                hiWord = hiWord | (exponent << 20);
                hiWord = hiWord | (significand & ~(-1 << 20));
                break;
        }
        var _bytes = toBytes(loWord);
        _bytes.concat(toBytes(hiWord));
        return _bytes;
    }
};

module.exports = methods;