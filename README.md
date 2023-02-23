# Generic Arduino proxy for Can Deserilization

This code is supposed to be a temporary way to read from can and send the info to the banana pi or embedded computer that will read the info and display it graphically, it also has a secondary function of sending some telemetry but that will be studied

## Dependencies

The code was primarily developed with a arduino uno genuino or a custom at90can based board in mind. But by the tests only the first one seems to respond well. For now you will need Ardubson, and some other "default" libraries.

On linux there seems to be a conflict between arduino_pins.h existing or not, as of now the person who is primarily writting this has'nt had the time to fix the issue.
